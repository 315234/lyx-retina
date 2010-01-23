// -*- C++ -*-
/**
 * \file GuiAbout.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Kalle Dalheimer
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef GUIABOUT_H
#define GUIABOUT_H

#include "DialogView.h"

namespace lyx {
namespace frontend {

class GuiAbout : public DialogView
{
	Q_OBJECT

public:
	// Constructor
	GuiAbout(GuiView & lv);

private Q_SLOTS:
	void on_closePB_clicked();

private:
	/// Controller stuff
	///@{
	void updateView() {}
	void dispatchParams() {}
	bool isBufferDependent() const { return false; }
	///@}

	struct Private;
	Private * const d;
};

} // namespace frontend
} // namespace lyx

#endif // GUIABOUT_H
