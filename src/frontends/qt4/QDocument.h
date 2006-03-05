// -*- C++ -*-
/**
 * \file QDocument.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Edwin Leuven
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef QDOCUMENT_H
#define QDOCUMENT_H

#include "QDialogView.h"
#include "BranchList.h"
#include <boost/scoped_ptr.hpp>
#include <string>
#include <vector>

class LengthCombo;
class  QLineEdit;

namespace lyx {
namespace frontend {

class ControlDocument;
class QDocumentDialog;

class QDocument
	: public QController<ControlDocument, QView<QDocumentDialog> >
{
public:

	friend class QDocumentDialog;

	QDocument(Dialog &);

	void showPreamble();

private:
	/// Apply changes
	void apply();
	/// update
	void update_contents();
	/// build the dialog
	void build_dialog();
	/// save as default template
	void saveDocDefault();
	/// reset to default params
	void useClassDefaults();
};

} // namespace frontend
} // namespace lyx

#endif // QDOCUMENT_H
