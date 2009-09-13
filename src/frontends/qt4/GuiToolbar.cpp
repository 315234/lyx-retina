/**
 * \file qt4/GuiToolbar.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Lars Gullik Bjønnes
 * \author John Levon
 * \author Jean-Marc Lasgouttes
 * \author Angus Leeming
 * \author Stefan Schimanski
 * \author Abdelrazak Younes
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>

#include "GuiToolbar.h"

#include "Action.h"
#include "GuiApplication.h"
#include "GuiCommandBuffer.h"
#include "GuiView.h"
#include "IconPalette.h"
#include "InsertTableWidget.h"
#include "LayoutBox.h"
#include "qt_helpers.h"
#include "Toolbars.h"

#include "FuncRequest.h"
#include "FuncStatus.h"
#include "KeyMap.h"
#include "LyXFunc.h"
#include "LyXRC.h"

#include "support/debug.h"
#include "support/gettext.h"
#include "support/lstrings.h"

#include <QSettings>
#include <QShowEvent>
#include <QString>
#include <QToolBar>
#include <QToolButton>

#include "support/lassert.h"

using namespace std;
using namespace lyx::support;

namespace lyx {
namespace frontend {

GuiToolbar::GuiToolbar(ToolbarInfo const & tbinfo, GuiView & owner)
	: QToolBar(toqstr(tbinfo.gui_name), &owner), visibility_(0),
	  allowauto_(false), owner_(owner), command_buffer_(0),
	  tbinfo_(tbinfo), filled_(false)
{
	setIconSize(owner.iconSize());
	connect(&owner, SIGNAL(iconSizeChanged(QSize)), this,
		SLOT(setIconSize(QSize)));

	// Toolbar dragging is allowed.
	setMovable(true);
	// This is used by QMainWindow::restoreState for proper main window state
	// restauration.
	setObjectName(toqstr(tbinfo.name));
	restoreSession();
}


void GuiToolbar::fill()
{
	if (filled_)
		return;
	ToolbarInfo::item_iterator it = tbinfo_.items.begin();
	ToolbarInfo::item_iterator end = tbinfo_.items.end();
	for (; it != end; ++it)
		add(*it);	
	filled_ = true;
}


void GuiToolbar::showEvent(QShowEvent * ev)
{
	fill();
	ev->accept();
}


void GuiToolbar::setVisibility(int visibility)
{
	visibility_ = visibility;
	allowauto_ = visibility_ >= Toolbars::MATH;
}


Action * GuiToolbar::addItem(ToolbarItem const & item)
{
	QString text = toqstr(item.label_);
	// Get the keys bound to this action, but keep only the
	// first one later
	KeyMap::Bindings bindings = theTopLevelKeymap().findBindings(item.func_);
	if (bindings.size())
		text += " [" + toqstr(bindings.begin()->print(KeySequence::ForGui)) + "]";

	Action * act = new Action(&owner_, getIcon(item.func_, false),
		text, item.func_, text, this);
	actions_.append(act);
	return act;
}

namespace {

class PaletteButton : public QToolButton
{
private:
	GuiToolbar * bar_;
	ToolbarItem const & tbitem_;
	bool initialized_;
public:
	PaletteButton(GuiToolbar * bar, ToolbarItem const & item)
		: QToolButton(bar), bar_(bar), tbitem_(item), initialized_(false)
	{
		QString const label = qt_(to_ascii(tbitem_.label_));
		setToolTip(label);
		setStatusTip(label);
		setText(label);
		connect(bar_, SIGNAL(iconSizeChanged(QSize)),
			this, SLOT(setIconSize(QSize)));
		setCheckable(true);
		ToolbarInfo const * tbinfo = guiApp->toolbars().info(tbitem_.name_);
		if (tbinfo)
			// use the icon of first action for the toolbar button
			setIcon(getIcon(tbinfo->items.begin()->func_, true));
	}

	void mousePressEvent(QMouseEvent * e)
	{
		if (initialized_) {
			QToolButton::mousePressEvent(e);
			return;
		}

		initialized_ = true;

		ToolbarInfo const * tbinfo = guiApp->toolbars().info(tbitem_.name_);
		if (!tbinfo) {
			LYXERR0("Unknown toolbar " << tbitem_.name_);
			return;
		}
		IconPalette * panel = new IconPalette(this);
		QString const label = qt_(to_ascii(tbitem_.label_));
		panel->setWindowTitle(label);
		connect(this, SIGNAL(clicked(bool)), panel, SLOT(setVisible(bool)));
		connect(panel, SIGNAL(visible(bool)), this, SLOT(setChecked(bool)));
		ToolbarInfo::item_iterator it = tbinfo->items.begin();
		ToolbarInfo::item_iterator const end = tbinfo->items.end();
		for (; it != end; ++it)
			if (!getStatus(it->func_).unknown())
				panel->addButton(bar_->addItem(*it));

		QToolButton::mousePressEvent(e);
	}
};

}


MenuButton::MenuButton(GuiToolbar * bar, ToolbarItem const & item, bool const sticky)
	: QToolButton(bar), bar_(bar), tbitem_(item), initialized_(false)
{
	setPopupMode(QToolButton::InstantPopup);
	QString const label = qt_(to_ascii(tbitem_.label_));
	setToolTip(label);
	setStatusTip(label);
	setText(label);
	QString const name = toqstr(tbitem_.name_);
	FileName fname = libFileSearch("images/math/", name, "png");
	if (fname.exists())
	    setIcon(QIcon(getPixmap("images/math/", name, "png")));
	else
	    setIcon(QIcon(getPixmap("images/", name, "png")));
	if (sticky)
		connect(this, SIGNAL(triggered(QAction *)),
			this, SLOT(actionTriggered(QAction *)));
	connect(bar, SIGNAL(iconSizeChanged(QSize)),
		this, SLOT(setIconSize(QSize)));
}

void MenuButton::mousePressEvent(QMouseEvent * e)
{
	if (initialized_) {
		QToolButton::mousePressEvent(e);
		return;
	}

	initialized_ = true;

	QString const label = qt_(to_ascii(tbitem_.label_));
	ButtonMenu * m = new ButtonMenu(label, this);
	m->setWindowTitle(label);
	m->setTearOffEnabled(true);
	connect(bar_, SIGNAL(updated()), m, SLOT(updateParent()));
	ToolbarInfo const * tbinfo = guiApp->toolbars().info(tbitem_.name_);
	if (!tbinfo) {
		LYXERR0("Unknown toolbar " << tbitem_.name_);
		return;
	}
	ToolbarInfo::item_iterator it = tbinfo->items.begin();
	ToolbarInfo::item_iterator const end = tbinfo->items.end();
	for (; it != end; ++it)
		if (!getStatus(it->func_).unknown())
			m->add(bar_->addItem(*it));
	setMenu(m);

	QToolButton::mousePressEvent(e);
}


void MenuButton::actionTriggered(QAction * action)
{
	QToolButton::setDefaultAction(action);
	setPopupMode(QToolButton::DelayedPopup);
}


void GuiToolbar::add(ToolbarItem const & item)
{
	switch (item.type_) {
	case ToolbarItem::SEPARATOR:
		addSeparator();
		break;
	case ToolbarItem::LAYOUTS: {
		LayoutBox * layout = owner_.getLayoutDialog();
		QObject::connect(this, SIGNAL(iconSizeChanged(QSize)),
			layout, SLOT(setIconSize(QSize)));
		QAction * action = addWidget(layout);
		action->setVisible(true);
		break;
	}
	case ToolbarItem::MINIBUFFER:
		command_buffer_ = new GuiCommandBuffer(&owner_);
		addWidget(command_buffer_);
		/// \todo find a Qt4 equivalent to setHorizontalStretchable(true);
		//setHorizontalStretchable(true);
		break;
	case ToolbarItem::TABLEINSERT: {
		QToolButton * tb = new QToolButton;
		tb->setCheckable(true);
		tb->setIcon(getIcon(FuncRequest(LFUN_TABULAR_INSERT), true));
		QString const label = qt_(to_ascii(item.label_));
		tb->setToolTip(label);
		tb->setStatusTip(label);
		tb->setText(label);
		InsertTableWidget * iv = new InsertTableWidget(owner_, tb);
		connect(tb, SIGNAL(clicked(bool)), iv, SLOT(show(bool)));
		connect(iv, SIGNAL(visible(bool)), tb, SLOT(setChecked(bool)));
		connect(this, SIGNAL(updated()), iv, SLOT(updateParent()));
		addWidget(tb);
		break;
		}
	case ToolbarItem::ICONPALETTE:
		addWidget(new PaletteButton(this, item));
		break;

	case ToolbarItem::POPUPMENU: {
		addWidget(new MenuButton(this, item, false));
		break;
		}
	case ToolbarItem::STICKYPOPUPMENU: {
		addWidget(new MenuButton(this, item, true));
		break;
		}
	case ToolbarItem::COMMAND: {
		if (!getStatus(item.func_).unknown())
			addAction(addItem(item));
		break;
		}
	default:
		break;
	}
}


void GuiToolbar::update(bool in_math, bool in_table, bool in_review, 
	bool in_mathmacrotemplate)
{
	if (visibility_ & Toolbars::AUTO) {
		bool show_it = (in_math && (visibility_ & Toolbars::MATH))
			|| (in_table && (visibility_ & Toolbars::TABLE))
			|| (in_review && (visibility_ & Toolbars::REVIEW))
			|| (in_mathmacrotemplate && (visibility_ & Toolbars::MATHMACROTEMPLATE));
		setVisible(show_it);
	}

	// update visible toolbars only
	if (!isVisible())
		return;

	// This is a speed bottleneck because this is called on every keypress
	// and update calls getStatus, which copies the cursor at least two times
	for (int i = 0; i < actions_.size(); ++i)
		actions_[i]->update();

	LayoutBox * layout = owner_.getLayoutDialog();
	if (layout)
		layout->setEnabled(lyx::getStatus(FuncRequest(LFUN_LAYOUT)).enabled());

	// emit signal
	updated();
}


QString GuiToolbar::sessionKey() const
{
	return "views/" + QString::number(owner_.id()) + "/" + objectName();
}


void GuiToolbar::saveSession() const
{
	QSettings settings;
	settings.setValue(sessionKey() + "/visibility", visibility_);
}


void GuiToolbar::restoreSession()
{
	QSettings settings;
	setVisibility(settings.value(sessionKey() + "/visibility").toInt());
}


void GuiToolbar::toggle()
{
	docstring state;
	if (allowauto_) {
		if (!(visibility_ & Toolbars::AUTO)) {
			visibility_ |= Toolbars::AUTO;
			hide();
			state = _("auto");
		} else {
			visibility_ &= ~Toolbars::AUTO;
			if (isVisible()) {
				hide();
				state = _("off");
			} else {
				show();
				state = _("on");
			}
		}
	} else {
		if (isVisible()) {
			hide();
			state = _("off");
		} else {
			show();
			state = _("on");
		}
	}

	owner_.message(bformat(_("Toolbar \"%1$s\" state set to %2$s"),
		qstring_to_ucs4(windowTitle()), state));
}

} // namespace frontend
} // namespace lyx

#include "moc_GuiToolbar.cpp"
