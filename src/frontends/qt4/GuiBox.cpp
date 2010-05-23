/**
 * \file GuiBox.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Jürgen Vigna (Minipage stuff)
 * \author Martin Vermeer
 * \author Jürgen Spitzmüller
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>

#include "GuiBox.h"

#include "FuncRequest.h"
#include "LengthCombo.h"
#include "Length.h"
#include "qt_helpers.h"
#include "Validator.h"

#include "insets/InsetBox.h"

#include "support/gettext.h"
#include "support/foreach.h"
#include "support/lstrings.h"

#include <QPushButton>
#include <QLineEdit>

#ifdef IN
#undef IN
#endif

using namespace std;


namespace lyx {
namespace frontend {

static QStringList boxGuiIds()
{
	return QStringList()
		<< "Frameless" << "Boxed"
		<< "ovalbox" << "Ovalbox"
		<< "Shadowbox" << "Shaded"
		<< "Doublebox";
}


static QStringList boxGuiNames()
{
	return QStringList()
		<< qt_("No frame") << qt_("Simple rectangular frame")
		<< qt_("Oval frame, thin") << qt_("Oval frame, thick")
		<< qt_("Drop shadow") << qt_("Shaded background")
		<< qt_("Double rectangular frame");
}


static QStringList boxGuiSpecialLengthIds()
{
	return QStringList() << "height" << "depth"
		<< "totalheight" << "width";
}


static QStringList boxGuiSpecialLengthNames()
{
	return QStringList() << qt_("Height") << qt_("Depth")
		<< qt_("Total Height") << qt_("Width");
}


GuiBox::GuiBox(QWidget * parent) : InsetParamsWidget(parent)
{
	setupUi(this);

	// fill the box type choice
	ids_ = boxGuiIds();
	gui_names_ = boxGuiNames();
	foreach (QString const & str, gui_names_)
		typeCO->addItem(str);

	// add the special units to the height choice
	// width needs different handling
	ids_spec_ = boxGuiSpecialLengthIds();
	gui_names_spec_ = boxGuiSpecialLengthNames();
	for (int i = 0; i != ids_spec_.size(); ++i)
		heightUnitsLC->addItem(gui_names_spec_[i], ids_spec_[i]);

	connect(widthED, SIGNAL(textChanged(QString)), this, SIGNAL(changed()));
	connect(widthUnitsLC, SIGNAL(selectionChanged(lyx::Length::UNIT)),
		this, SIGNAL(changed()));
	connect(valignCO, SIGNAL(highlighted(QString)), this, SIGNAL(changed()));
	connect(heightED, SIGNAL(textChanged(QString)), this, SIGNAL(changed()));
	connect(heightUnitsLC, SIGNAL(selectionChanged(lyx::Length::UNIT)),
		this, SIGNAL(changed()));
	connect(halignCO, SIGNAL(activated(int)), this, SIGNAL(changed()));
	connect(ialignCO, SIGNAL(activated(int)), this, SIGNAL(changed()));

	heightED->setValidator(unsignedLengthValidator(heightED));
	widthED->setValidator(unsignedLengthValidator(widthED));

	// initialize the length validator
	addCheckedWidget(widthED, widthLA);
	addCheckedWidget(heightED, heightCB);

	initDialog();
}


void GuiBox::on_innerBoxCO_activated(QString const & str)
{
	bool const ibox = (str != qt_("None"));
	int outer = typeCO->currentIndex();
	valignCO->setEnabled(ibox);
	ialignCO->setEnabled(ibox);
	halignCO->setEnabled(!ibox);
	heightCB->setEnabled(ibox);
	// if an outer box from the fancybox LaTeX-package has no inner box,
	// the width cannot be specified
	if (!ibox && outer != 0 && outer != 1) {
		widthED->setEnabled(false);
		widthUnitsLC->setEnabled(false);
	} else {
		widthED->setEnabled(true);
		widthUnitsLC->setEnabled(true);
	}
	pagebreakCB->setEnabled(!ibox && outer == 1);
	setSpecial(ibox);
	changed();
}


void GuiBox::on_typeCO_activated(int index)
{
	bool const frameless = (index == 0);
	if (frameless) {
		valignCO->setEnabled(true);
		ialignCO->setEnabled(true);
		halignCO->setEnabled(false);
		heightCB->setEnabled(true);
		setSpecial(true);
	}
	if (index != 1)
		pagebreakCB->setChecked(false);
	int itype = innerBoxCO->currentIndex();
	if (innerBoxCO->count() == 2)
		++itype;
	pagebreakCB->setEnabled(index == 1 && itype == 0);
	// if an outer box from the fancybox LaTeX-package has no inner box,
	// the width cannot be specified
	if (itype == 0
	    && (index == 2 || index == 3 || index == 4 || index == 6)) {
		widthED->setEnabled(false);
		widthUnitsLC->setEnabled(false);
	} else {
		widthED->setEnabled(index != 5);
		widthUnitsLC->setEnabled(index != 5);
	}
	setInnerType(frameless, itype);
	changed();
}


void GuiBox::initDialog()
{
	setInnerType(true, 2);
	widthED->setText("100");
	widthUnitsLC->setCurrentItem(Length::PCW);
	heightCB->setCheckState(Qt::Checked);
	heightED->setText("1");
	heightUnitsLC->setCurrentItem("totalheight");
}


void GuiBox::on_heightCB_stateChanged(int state)
{
	bool const enable = (innerBoxCO->currentText() != qt_("None"))
		&& (state == Qt::Checked);
	heightED->setEnabled(enable);
	heightUnitsLC->setEnabled(enable);
	changed();
}


void GuiBox::on_pagebreakCB_stateChanged()
{
	bool pbreak = (pagebreakCB->checkState() == Qt::Checked);
	innerBoxCO->setEnabled(!pbreak);
	widthED->setEnabled(!pbreak);
	widthUnitsLC->setEnabled(!pbreak);
	if (!pbreak) {
		on_typeCO_activated(typeCO->currentIndex());
		return;
	}
	valignCO->setEnabled(false);
	ialignCO->setEnabled(false);
	halignCO->setEnabled(false);
	heightCB->setEnabled(false);
	heightED->setEnabled(false);
	heightUnitsLC->setEnabled(false);
	setSpecial(false);
	changed();
}


void GuiBox::paramsToDialog(Inset const * inset)
{
	InsetBox const * box = static_cast<InsetBox const *>(inset);
	InsetBoxParams const & params = box->params();
	QString type = toqstr(params.type);
	if (type == "Framed") {
		pagebreakCB->setChecked(true);
		type = "Boxed";
	} else {
		pagebreakCB->setChecked(false);
	}

	pagebreakCB->setEnabled(type == "Boxed" && !params.inner_box);

	for (int i = 0; i != gui_names_.size(); ++i) {
		if (type == ids_[i])
			typeCO->setCurrentIndex(i);
	}

	// default: minipage
	int inner_type = 2;
	if (!params.inner_box)
		// none
		inner_type = 0;
	if (params.use_parbox)
		// parbox
		inner_type = 1;
	bool frameless = (params.type == "Frameless");
	setInnerType(frameless, inner_type);

	char c = params.pos;
	valignCO->setCurrentIndex(string("tcb").find(c, 0));
	c = params.inner_pos;
	ialignCO->setCurrentIndex(string("tcbs").find(c, 0));
	c = params.hor_pos;
	halignCO->setCurrentIndex(string("lcrs").find(c, 0));

	bool ibox = params.inner_box;
	valignCO->setEnabled(ibox);
	ialignCO->setEnabled(ibox);
	halignCO->setEnabled(!ibox);
	setSpecial(ibox);

	Length::UNIT const default_unit = Length::defaultUnit();

	lengthToWidgets(widthED, widthUnitsLC,
		(params.width).asString(), default_unit);

	QString const special = toqstr(params.special);
	if (!special.isEmpty() && special != "none")
		widthUnitsLC->setCurrentItem(special);

	lengthToWidgets(heightED, heightUnitsLC,
		(params.height).asString(), default_unit);
	
	QString const height_special = toqstr(params.height_special);
	if (!height_special.isEmpty() && height_special != "none")
		heightUnitsLC->setCurrentItem(height_special);
	// set no optional height if the value is the default "1\height"
	// (special units like \height are handled as "in",
	// FIXME: this is a very bad UI, this check box should be disabled in
	// this case, not forced to 'unchecked' state.
	if (height_special == "totalheight" && params.height == Length("1in"))
		heightCB->setCheckState(Qt::Unchecked);
	else
		heightCB->setCheckState(Qt::Checked);

	heightCB->setEnabled(ibox);
}


docstring GuiBox::dialogToParams() const
{
	bool const pagebreak =
		pagebreakCB->isEnabled() && pagebreakCB->isChecked();
	string box_type;
	if (pagebreak)
		box_type = "Framed";
	else
		box_type = fromqstr(ids_[typeCO->currentIndex()]);

	InsetBoxParams params(box_type);
	params.inner_box =
		(!pagebreak && innerBoxCO->currentText() != qt_("None"));
	params.use_parbox =
		(!pagebreak && innerBoxCO->currentText() == qt_("Parbox"));

	params.pos = "tcb"[valignCO->currentIndex()];
	params.inner_pos = "tcbs"[ialignCO->currentIndex()];
	params.hor_pos = "lcrs"[halignCO->currentIndex()];

	QString unit =
		widthUnitsLC->itemData(widthUnitsLC->currentIndex()).toString();
	QString value = widthED->text();
	if (ids_spec_.contains(unit) && !isValidLength(fromqstr(value))) {
		params.special = fromqstr(unit);
		// Note: the unit is simply ignored in this case
		params.width = Length(value.toDouble(), Length::IN);
	} else {
		params.special = "none";
		params.width = Length(widgetsToLength(widthED, widthUnitsLC));
	}

	// the height parameter is omitted if the value
	// is "1in" and "Total Height" is used as unit.
	// 1in + "Total Height" means "1\height" which is the LaTeX default
	// if no height is given
	if (heightCB->checkState() == Qt::Unchecked) {
		params.height = Length("1in");
		params.height_special = "totalheight";
	} else {
		unit = heightUnitsLC->itemData(heightUnitsLC->currentIndex()).toString();
		value = heightED->text();
		if (ids_spec_.contains(unit) && !isValidLength(fromqstr(value))) {
			params.height_special = fromqstr(unit);
			// Note: the unit is simply ignored in this case
			params.height = Length(value.toDouble(), Length::IN);
		} else {
			params.height_special = "none";
			params.height =
				Length(widgetsToLength(heightED, heightUnitsLC));
		}
	}
	return from_ascii(InsetBox::params2string(params));
}


void GuiBox::setSpecial(bool ibox)
{
	QString const last_item =
		widthUnitsLC->itemData(heightUnitsLC->currentIndex()).toString();

	// check if the widget contains the special units
	bool const has_special = (widthUnitsLC->findData("totalheight") != -1);
	// insert 'em if needed...
	if (!ibox && !has_special) {
		for (int i = 1; i < ids_spec_.size(); ++i)
			widthUnitsLC->addItem(gui_names_spec_[i], ids_spec_[i]);
	// ... or remove 'em if needed
	} else if (ibox && has_special) {
		for (int i = 1; i < ids_spec_.size(); ++i) {
			int n = widthUnitsLC->findData(ids_spec_[i]);
			if (n != -1)
				widthUnitsLC->removeItem(n);
		}
	}
	// restore selected text, if possible
	widthUnitsLC->setCurrentItem(last_item);
}


void GuiBox::setInnerType(bool frameless, int i)
{
	// with "frameless" boxes, inner box is mandatory (i.e. is the actual box)
	// we have to remove "none" then and adjust the combo
	if (frameless) {
		innerBoxCO->clear();
		innerBoxCO->addItem(qt_("Parbox"));
		innerBoxCO->addItem(qt_("Minipage"));
		if (i != 0)
			innerBoxCO->setCurrentIndex(i - 1);
		else
			innerBoxCO->setCurrentIndex(i);
	} else {
		innerBoxCO->clear();
		innerBoxCO->addItem(qt_("None"));
		innerBoxCO->addItem(qt_("Parbox"));
		innerBoxCO->addItem(qt_("Minipage"));
		innerBoxCO->setCurrentIndex(i);
	}
}

} // namespace frontend
} // namespace lyx


#include "moc_GuiBox.cpp"
