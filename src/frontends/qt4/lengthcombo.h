// -*- C++ -*-
/**
 * \file lengthcombo.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author John Levon
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef LENGTHCOMBO_H
#define LENGTHCOMBO_H


#include <QComboBox>

#include "vspace.h"

/**
 * A combo box for selecting LyXLength::UNIT types.
 */
class LengthCombo : public QComboBox {
	Q_OBJECT
public:
	LengthCombo(QWidget * parent);

	/// set the current item from unit
	virtual void setCurrentItem(LyXLength::UNIT unit);
	/// set the current item from int
	virtual void setCurrentItem(int item);
	/// get the current item
	LyXLength::UNIT currentLengthItem() const;
	/// enable the widget
	virtual void setEnabled(bool b);
	/// use the %-items?
	virtual void noPercents();

protected Q_SLOTS:
	virtual void has_activated(int index);
Q_SIGNALS:
	/// the current selection has changed
	void selectionChanged(LyXLength::UNIT unit);

};

#endif // LENGTHCOMBO_H
