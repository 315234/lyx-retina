/**
 * \file copy.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Lars Gullik Bj�nnes
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>

#include <fstream>

#include "support/FileName.h"
#include "support/lyxlib.h"

#ifdef HAVE_SYS_STAT_H
# include <sys/stat.h>
#endif
#ifdef HAVE_SYS_TYPES_H
# include <sys/types.h>
#endif


namespace lyx {


using std::ifstream;
using std::ofstream;
using std::ios;
using std::string;


bool lyx::support::chmod(FileName const & file, unsigned long int mode)
{
#if defined (HAVE_CHMOD) && defined (HAVE_MODE_T)
	if (::chmod(file.toFilesystemEncoding().c_str(), mode_t(mode)) != 0)
		return false;
#else
# ifdef WITH_WARNINGS
#  warning "File permissions are ignored on this system."
# endif
#endif
	return true;
}


bool lyx::support::copy(FileName const & from, FileName const & to, unsigned long int mode)
{
	ifstream ifs(from.toFilesystemEncoding().c_str(), ios::binary | ios::in);
	if (!ifs)
		return false;

	if (mode != (unsigned long int)-1) {
		ofstream ofs(to.toFilesystemEncoding().c_str(), ios::binary | ios::out | ios::trunc);
		if (!ofs)
			return false;
		ofs.close();
		if (!support::chmod(to, mode))
			return false;
	}

	ofstream ofs(to.toFilesystemEncoding().c_str(), ios::binary | ios::out | ios::trunc);
	if (!ofs)
		return false;

	ofs << ifs.rdbuf();
	return ofs.good();
}


} // namespace lyx
