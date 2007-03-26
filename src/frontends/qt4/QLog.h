// -*- C++ -*-
/**
 * \file QLog.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author John Levon
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef QLOG_H
#define QLOG_H

#include "QDialogView.h"
#include "QLogDialog.h"

#include <QSyntaxHighlighter>

namespace lyx {
namespace frontend {

class ControlLog;
class logHighlighter;

///
class QLog
	: public QController<ControlLog, QView<QLogDialog> >
{
public:
	///
	friend class QLogDialog;
	///
	QLog(Dialog &);
private:
	/// Apply changes
	virtual void apply() {}
	/// update
	virtual void update_contents();
	/// build the dialog
	virtual void build_dialog();
	/// log syntax highlighter
	logHighlighter * highlighter;

};


///
class logHighlighter : public QSyntaxHighlighter
{
	Q_OBJECT
	
public:
	logHighlighter(QTextDocument * parent);

protected:
	void highlightBlock(QString const & text);

private:
	QTextCharFormat infoFormat;
	QTextCharFormat warningFormat;
	QTextCharFormat errorFormat;
};


} // namespace frontend
} // namespace lyx

#endif // QLOG_H
