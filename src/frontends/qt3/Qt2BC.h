// -*- C++ -*-
/**
 * \file Qt2BC.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Allan Rae
 * \author Angus Leeming
 * \author Baruch Even
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef QT2BC_H
#define QT2BC_H

#include "BCView.h"
#include "gettext.h"

class QWidget;
class QButton;

namespace lyx {
namespace frontend {

/** General purpose button controller for up to four buttons.
    Controls the activation of the OK, Apply and Cancel buttons.
    Actually supports 4 buttons in all and it's up to the user to decide on
    the activation policy and which buttons correspond to which output of the
    state machine.
*/
class Qt2BC : public GuiBC<QButton, QWidget> {
public:
	///
	Qt2BC(ButtonController const &,
	      lyx::docstring const & = _("Cancel"),
	      lyx::docstring const & = _("Close"));
private:
	/// Updates the button sensitivity (enabled/disabled)
	void setButtonEnabled(QButton *, bool enabled) const;

	/// Updates the widget sensitivity (enabled/disabled)
	void setWidgetEnabled(QWidget *, bool enabled) const;

	/// Set the label on the button
	void setButtonLabel(QButton *, lyx::docstring const & label) const;
};

} // namespace frontend
} // namespace lyx

#endif // QT2BC_H
