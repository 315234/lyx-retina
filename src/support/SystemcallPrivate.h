// -*- C++ -*-
/**
 * \file SystemcallPrivate.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Enrico Forestieri
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef SYSTEMCALLPRIVATE_H
#define SYSTEMCALLPRIVATE_H

#include <QObject>
#include <QProcess>

#include <string>

namespace lyx {
namespace support {

class Systemcall;

/**
 * Outputs to the console terminal the line buffered standard output and
 * error of a spawned process when there is a controlling terminal and 
 * stdout/stderr have not been redirected.
 */
class SystemcallPrivate : public QObject
{
	Q_OBJECT
public:
	SystemcallPrivate(std::string const & outfile);
	~SystemcallPrivate();

	/// Should the standard output be displayed?
	void setShowOut(bool val) { showout_ = val; }

	/// Should the standard error be displayed?
	void setShowErr(bool val) { showerr_ = val; }

	enum State {
		Starting,
		Running,
		Finished,
		Error
	};
	State state;

	bool waitWhile(State, bool processEvents, int timeout = -1);
	void startProcess(const QString& cmd);
	
	int exitCode();

	QString errorMessage() const;
	QString exitStatusMessage() const;

	void flush();

	QProcess* releaseProcess();
	
	static void killProcess(QProcess * p);

private:
	/// Pointer to the process to monitor.
	QProcess * proc_;
	/// Index to the standard output buffer.
	size_t outindex_;
	/// Index to the standard error buffer.
	size_t errindex_;
	///
	std::string outfile;
	/// Size of buffers.
	static size_t const bufsize_ = 200;
	/// Standard output buffer.
	char outdata_[bufsize_];
	/// Standard error buffer.
	char errdata_[bufsize_];
	/// 
	bool showout_;
	/// 
	bool showerr_;
	bool process_events;
	QString cmd_;

	void waitAndProcessEvents();
	void processEvents();

	void killProcess();

public Q_SLOTS:
	void stdOut();
	void stdErr();
	void processError(QProcess::ProcessError);
	void processStarted();
	void processFinished(int, QProcess::ExitStatus status);

Q_SIGNALS:

};

} // namespace support
} // namespace lyx

#endif // SYSTEMCALLPRIVATE_H
