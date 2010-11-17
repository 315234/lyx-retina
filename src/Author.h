// -*- C++ -*-
/**
 * \file Author.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author John Levon
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef AUTHOR_H
#define AUTHOR_H

#include "support/docstring.h"

#include <vector>


namespace lyx {

class Author {
public:
	///
	Author() {}
	///
	Author(docstring const & name, docstring const & email);
	///
	docstring name() const { return name_; }
	///
	docstring email() const { return email_; }
	///
	int bufferId() const { return buffer_id_; }
	///
	void setBufferId(int buffer_id) const { buffer_id_ = buffer_id; }
	///
	void setUsed(bool u) const { used_ = u; }
	///
	bool used() const { return used_; }
	///
	friend std::istream & operator>>(std::istream & os, Author & a);
	///
	friend std::ostream & operator<<(std::ostream & os, Author const & a);

private:
	/// The author's name
	docstring name_;
	/// The author's email address
	docstring email_;
	///
	mutable bool used_;
	/// The id of the author in the lyx-file
	mutable int buffer_id_;
};


class AuthorList {
public:
	///
	AuthorList();
	///
	int record(Author const & a);
	///
	void record(int id, Author const & a);
	///
	void recordCurrentAuthor(Author const & a);
	///
	Author const & get(int id) const;
	///
	typedef std::vector<Author> Authors;
	///
	void sort();
	///
	Authors::const_iterator begin() const;
	///
	Authors::const_iterator end() const;
	///
	friend
	std::ostream & operator<<(std::ostream & os, AuthorList const & a);
private:
	///
	int last_id_;
	///
	Authors authors_;
};

bool operator==(Author const & l, Author const & r);

std::ostream & operator<<(std::ostream & os, Author const & a);

std::istream & operator>>(std::istream & os, Author & a);


} // namespace lyx

#endif // AUTHOR_H
