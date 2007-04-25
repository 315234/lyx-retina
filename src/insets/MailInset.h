// -*- C++ -*-
/**
 * \file MailInset.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Angus Leeming
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef MAILINSET_H
#define MAILINSET_H

#include <string>


namespace lyx {


class InsetBase;
class Buffer;
class BufferView;


class MailInset {
public:
	///
	virtual ~MailInset() {};
	///
	void showDialog(BufferView *) const;
	///
	void updateDialog(BufferView *) const;
	///
	void hideDialog() const;
	///
	virtual std::string const inset2string(Buffer const &) const = 0;

protected:
	///
	virtual InsetBase & inset() const = 0;
	///
	virtual std::string const & name() const = 0;
};

void print_mailer_error(std::string const & class_name,
			std::string const & data,
			int arg_id, std::string const & arg);



} // namespace lyx

#endif // MAILINSET_H
