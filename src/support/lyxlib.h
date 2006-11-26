// -*- C++ -*-
/**
 * \file lyxlib.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * A selection of useful system functions made
 * handy for C++ usage.
 *
 * \author Lars Gullik Bj�nnes
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef LYX_LIB_H
#define LYX_LIB_H

#include <string>


namespace lyx {
namespace support {

class FileName;

/// get the current working directory
std::string const getcwd();
/// change to a directory, 0 is returned on success.
int chdir(FileName const & name);
/// Change file permissions
bool chmod(FileName const & file, unsigned long int mode);
/**
 * rename a file, returns false if it fails.
 * It can handle renames across partitions.
 */
bool rename(FileName const & from, FileName const & to);
/// copy a file, returns false it it fails
bool copy(FileName const & from, FileName const & to,
          unsigned long int mode = (unsigned long int)-1);
/// generates a checksum of a file
unsigned long sum(FileName const & file);
/// FIXME: some point to this hmm ?
int kill(int pid, int sig);
/// FIXME: same here
void abort();
/// create the given directory with the given mode
int mkdir(FileName const & pathname, unsigned long int mode);
/// unlink the given file
int unlink(FileName const & file);
/// (securely) create a temporary file in the given dir with the given prefix
std::string const tempName(std::string const & dir = std::string(),
		      std::string const & mask = std::string());


/**
 * Returns true if var is approximately equal to number with allowed error
 * of 'error'.
 *
 * Usage: if (float_equal(var, number, 0.0001)) { }
 *
 * This will check if 'var' is approx. equal to 'number' with error of 1/1000
 */
inline bool float_equal(double var, double number, double error)
{
	return (number - error <= var && var <= number + error);
}

} // namespace support
} // namespace lyx

#endif /* LYX_LIB_H */
