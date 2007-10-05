// -*- C++ -*-
/**
 * \file GuiShowFile.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author John Levon
 * \author Herbert Voß
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef GUISHOWFILE_H
#define GUISHOWFILE_H

#include "GuiDialog.h"
#include "ui_ShowFileUi.h"

#include "support/FileName.h"

namespace lyx {
namespace frontend {

class GuiShowFile : public GuiDialog, public Ui::ShowFileUi, public Controller
{
	Q_OBJECT

public:
	GuiShowFile(LyXView & lv);

private:
	///
	void closeEvent(QCloseEvent * e);
	/// parent controller
	Controller & controller() { return *this; }
	/// update
	void updateContents();
	///
	bool initialiseParams(std::string const & data);
	///
	void clearParams();
	///
	void dispatchParams() {}
	///
	bool isBufferDependent() const { return false; }

	///
	support::FileName filename_;
};

} // namespace frontend
} // namespace lyx

#endif // GUISHOWFILE_H
