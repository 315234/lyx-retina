// -*- C++ -*-
/**
 * \file QParagraph.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Edwin Leuven
 * \author John Levon
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef QPARAGRAPH_H
#define QPARAGRAPH_H

#include "QDialogView.h"
#include "QParagraphDialog.h"

namespace lyx {
namespace frontend {

class ControlParagraph;

class QParagraph
	: public QController<ControlParagraph, QView<QParagraphDialog> >
{
public:
	friend class QParagraphDialog;

	QParagraph(Dialog &);
private:
	/// Apply changes
	virtual void apply();
	/// update
	virtual void update_contents();
	/// build the dialog
	virtual void build_dialog();
};

} // namespace frontend
} // namespace lyx

#endif // QPARAGRAPH_H
