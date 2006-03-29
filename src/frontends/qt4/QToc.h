// -*- C++ -*-
/**
 * \file QToc.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author John Levon
 * \author Kalle Dalheimer
 * \author Abdelrazak Younes
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef QTOC_H
#define QTOC_H

#include "QDialogView.h"
#include "toc.h"

namespace lyx {
namespace frontend {

class ControlToc;
class QTocDialog;

//template<class W>
class QToc :
	public QController<ControlToc, QView<QTocDialog> >
{
public:
	friend class QTocDialog;

	QToc(Dialog &);

	/// return the toc list
	lyx::toc::Toc & get_toclist() { return toclist;}

	void moveUp();
	void moveDown();
	void moveIn();
	void moveOut();

private:

	/// select an entry
	void select(std::string const & text);

	virtual void apply() {}

	/// update dialog
	virtual void update_contents();

	/// build dialog
	virtual void build_dialog();

	/// the toc list
	lyx::toc::Toc toclist;
};

} // namespace frontend
} // namespace lyx

#endif // QTOC_H
