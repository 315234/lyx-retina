// -*- C++ -*-
/**
 * \file GuiProgress.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Peter Kümmel
 * \author Pavel Sanda
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>

#include "GuiProgress.h"
#include "ui_ToggleWarningUi.h"

#include "qt_helpers.h"

#include "support/debug.h"
#include "support/Systemcall.h"

#include <QApplication>
#include <QTime>
#include <QMessageBox>
#include <QSettings>


namespace lyx {
namespace frontend {


class GuiToggleWarningDialog : public QDialog, public Ui::ToggleWarningUi
{
public:
	GuiToggleWarningDialog(QWidget * parent) : QDialog(parent)
	{
		Ui::ToggleWarningUi::setupUi(this);
		QDialog::setModal(true);
	}
};


GuiProgress::GuiProgress()
{
	connect(this, SIGNAL(processStarted(QString const &)), SLOT(doProcessStarted(QString const &)));
	connect(this, SIGNAL(processFinished(QString const &)), SLOT(doProcessFinished(QString const &)));
	connect(this, SIGNAL(appendMessage(QString const &)), SLOT(doAppendMessage(QString const &)));
	connect(this, SIGNAL(appendError(QString const &)), SLOT(doAppendError(QString const &)));
	connect(this, SIGNAL(clearMessages()), SLOT(doClearMessages()));
	
	// Alert interface
	connect(this, SIGNAL(warning(QString const &, QString const &)),
		SLOT(doWarning(QString const &, QString const &)));
	connect(this, SIGNAL(toggleWarning(QString const &, QString const &, QString const &)),
		SLOT(doToggleWarning(QString const &, QString const &, QString const &)));
	connect(this, SIGNAL(error(QString const &, QString const &)),
		SLOT(doError(QString const &, QString const &)));
	connect(this, SIGNAL(information(QString const &, QString const &)),
		SLOT(doInformation(QString const &, QString const &)));

	flushDelay_.setInterval(200);
	flushDelay_.setSingleShot(true);
	connect(&flushDelay_, SIGNAL(timeout()), this, SLOT(updateWithLyXErr()));
}


QString GuiProgress::currentTime()
{
	return QTime::currentTime().toString("hh:mm:ss:zzz") + "ms";
}


void GuiProgress::doProcessStarted(QString const & cmd)
{
	appendText(currentTime() + ": <" + cmd + "> started");
}


void GuiProgress::doProcessFinished(QString const & cmd)
{
	appendText(currentTime() + ": <" + cmd + "> done");
}


void GuiProgress::doAppendMessage(QString const & msg)
{
	appendText(msg);
}


void GuiProgress::doAppendError(QString const & msg)
{
	appendText(msg);
}


void GuiProgress::doClearMessages()
{
	clearMessageText();
}


void GuiProgress::lyxerrFlush()
{
	flushDelay_.start();
}


void GuiProgress::updateWithLyXErr()
{
	appendLyXErrMessage(toqstr(lyxerr_stream_.str()));
	lyxerr_stream_.str("");
}


void GuiProgress::lyxerrConnect()
{
	lyxerr.setSecond(&lyxerr_stream_);
}


void GuiProgress::lyxerrDisconnect()
{
	lyxerr.setSecond(0);
}


GuiProgress::~GuiProgress()
{
	lyxerrDisconnect();
}


void GuiProgress::appendText(QString const & text)
{
	if (!text.isEmpty())
		updateStatusBarMessage(text);
}


void GuiProgress::doWarning(QString const & title, QString const & message)
{
	QMessageBox::warning(qApp->focusWidget(), title, message);
}


void GuiProgress::doToggleWarning(QString const & title, QString const & msg, QString const & formatted)
{
	QSettings settings;
	if (settings.value("hidden_warnings/" + msg, false).toBool())
			return;

	GuiToggleWarningDialog * dlg =
		new GuiToggleWarningDialog(qApp->focusWidget());

	dlg->setWindowTitle(title);
	dlg->messageLA->setText(formatted);
	dlg->dontShowAgainCB->setChecked(false);

	if (dlg->exec() == QDialog::Accepted)
		if (dlg->dontShowAgainCB->isChecked())
			settings.setValue("hidden_warnings/"
				+ msg, true);
}


void GuiProgress::doError(QString const & title, QString const & message)
{
	QMessageBox::critical(qApp->focusWidget(), title, message);
}


void GuiProgress::doInformation(QString const & title, QString const & message)
{
	QMessageBox::information(qApp->focusWidget(), title, message);
}


} // namespace frontend
} // namespace lyx

#include "moc_GuiProgress.cpp"
