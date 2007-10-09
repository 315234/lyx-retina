/**
 * \file GuiSendto.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Angus Leeming
 * \author J�rgen Spitzm�ller
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>

#include "GuiSendto.h"

#include "Buffer.h"
#include "Converter.h"
#include "Format.h"
#include "FuncRequest.h"
#include "LyXRC.h"
#include "qt_helpers.h"

#include "support/filetools.h"
#include "support/lstrings.h"

#include <QListWidget>
#include <QPushButton>
#include <QCloseEvent>

using std::vector;
using std::string;


namespace lyx {
namespace frontend {

using support::trim;

GuiSendTo::GuiSendTo(LyXView & lv)
	: GuiDialog(lv, "sendto")
{
	setupUi(this);
	setViewTitle(_("Send Document to Command"));

	connect(okPB, SIGNAL(clicked()), this, SLOT(slotOK()));
	connect(applyPB, SIGNAL(clicked()), this, SLOT(slotApply()));
	connect(closePB, SIGNAL(clicked()), this, SLOT(slotClose()));

	connect(formatLW, SIGNAL(itemClicked(QListWidgetItem *)),
		this, SLOT(slotFormatHighlighted(QListWidgetItem *)));
	connect(formatLW, SIGNAL(itemActivated(QListWidgetItem *)),
		this, SLOT(slotFormatSelected(QListWidgetItem *)));
	connect(formatLW, SIGNAL(itemClicked(QListWidgetItem *)),
		this, SLOT(changed_adaptor()));
	connect(commandCO, SIGNAL(textChanged(QString)),
		this, SLOT(changed_adaptor()));

	bc().setPolicy(ButtonPolicy::OkApplyCancelPolicy);
	bc().setOK(okPB);
	bc().setApply(applyPB);
	bc().setCancel(closePB);
}


void GuiSendTo::changed_adaptor()
{
	changed();
}


void GuiSendTo::closeEvent(QCloseEvent * e)
{
	slotClose();
	e->accept();
}


void GuiSendTo::updateContents()
{
	all_formats_ = allFormats();

	// Check whether the current contents of the browser will be
	// changed by loading the contents of formats
	vector<string> keys;
	keys.resize(all_formats_.size());

	vector<string>::iterator result = keys.begin();
	vector<Format const *>::const_iterator it  = all_formats_.begin();
	vector<Format const *>::const_iterator end = all_formats_.end();
	for (; it != end; ++it, ++result)
		*result = (*it)->prettyname();

	// Reload the browser
	formatLW->clear();

	for (vector<string>::const_iterator it = keys.begin();
	     it != keys.end(); ++it) {
		formatLW->addItem(toqstr(*it));
	}

	commandCO->addItem(toqstr(command_));
}


void GuiSendTo::applyView()
{
	int const line = formatLW->currentRow();

	if (line < 0 || line > int(formatLW->count()))
		return;

	format_ = all_formats_[line];
	command_ = trim(fromqstr(commandCO->currentText()));
}


bool GuiSendTo::isValid()
{
	int const line = formatLW->currentRow();

	if (line < 0 || line > int(formatLW->count()))
		return false;

	return formatLW->count() != 0 &&
		!commandCO->currentText().isEmpty();
}


bool GuiSendTo::initialiseParams(std::string const &)
{
	format_ = 0;
	command_ = lyxrc.custom_export_command;
	return true;
}


void GuiSendTo::dispatchParams()
{
	if (command_.empty() || !format_ || format_->name().empty())
		return;

	string const data = format_->name() + " " + command_;
	dispatch(FuncRequest(getLfun(), data));
}

// FIXME: Move to Converters?
vector<Format const *> GuiSendTo::allFormats() const
{
	// What formats can we output natively?
	vector<string> exports;
	exports.push_back("lyx");
	exports.push_back("text");

	if (buffer().isLatex())
		exports.push_back("latex");
	else if (buffer().isDocBook())
		exports.push_back("docbook");
	else if (buffer().isLiterate())
		exports.push_back("literate");

	// Loop over these native formats and ascertain what formats we
	// can convert to
	vector<Format const *> to;

	vector<string>::const_iterator ex_it  = exports.begin();
	vector<string>::const_iterator ex_end = exports.end();
	for (; ex_it != ex_end; ++ex_it) {
		// Start off with the native export format.
		// "formats" is LyX's list of recognised formats
		to.push_back(formats.getFormat(*ex_it));

		Formats::const_iterator fo_it  = formats.begin();
		Formats::const_iterator fo_end = formats.end();
		for (; fo_it != fo_end; ++fo_it) {
			// we need to hide the default graphic export formats
			// from the external menu, because we need them only
			// for the internal lyx-view and external latex run
			string const name = fo_it->name();
			if (name != "eps" && name != "xpm" && name != "png" &&
			    theConverters().isReachable(*ex_it, name))
				to.push_back(&(*fo_it));
		}
	}

	// Remove repeated formats.
	std::sort(to.begin(), to.end());
	to.erase(std::unique(to.begin(), to.end()), to.end());

	return to;
}


Dialog * createGuiSendTo(LyXView & lv) { return new GuiSendTo(lv); }


} // namespace frontend
} // namespace lyx

#include "GuiSendto_moc.cpp"
