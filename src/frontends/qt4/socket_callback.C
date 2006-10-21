/**
 * \file io_callback.C
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author unknown
 * \author John Levon
 * \author Jo�o Luis M. Assirati
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>

#include "socket_callback.h"


namespace lyx {

socket_callback::socket_callback(int fd, boost::function<void()> func)
	: func_(func)
{
	sn_.reset(new QSocketNotifier(fd, QSocketNotifier::Read, this));
	connect(sn_.get(), SIGNAL(activated(int)), this, SLOT(data_received()));
}


void socket_callback::data_received()
{
	func_();
}

} // namespace lyx

#include "socket_callback_moc.cpp"
