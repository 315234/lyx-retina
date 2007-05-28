// -*- C++ -*-
/**
 * \file InsetListingsParams.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Bo Peng
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef INSETLISTINGSPARAMS_H
#define INSETLISTINGSPARAMS_H

#include <map>
#include <exception>
#include "Lexer.h"
#include "InsetCollapsable.h"

namespace lyx {

class InsetListingsParams {
public:
	///
	InsetListingsParams();

	///
	InsetListingsParams(std::string const &, bool in=false,
		InsetCollapsable::CollapseStatus s = InsetCollapsable::Open);

	/// write parameters to an ostream
	void write(std::ostream &) const;

	/// read parameters from an ostream
	void read(Lexer &);

	/// valid parameter string
	std::string params(std::string const & sep=",") const;

	/// add key=value to params_
	void addParam(std::string const & key, std::string const & value);

	/// add a few parameters
	void addParams(std::string const & par);

	/// set params_ with par, throw an exception if par is valid
	void setParams(std::string const & par);

	/// generate a parameter string that can be safely save and restored
	/// by lyx' lexer
	std::string encodedString() const;

	/// newline (\n) separated parameters. comma can be removed.
	/// One possible complication is that , may appear in option value.
	std::string separatedParams(bool keepComma = false) const;

	/// get parameter from encoded string
	void fromEncodedString(std::string const & par);

	///
	bool isInline() const { return inline_; }

	///
	bool isFloat() const;

	///
	InsetCollapsable::CollapseStatus status() const { return status_; }

	///
	void setInline(bool i) { inline_ = i; }

	/// get value of option \c param
	std::string getParamValue(std::string const & param) const;

	///
	void clear() { params_.clear(); }

private:
	/// inline or normal listings
	bool inline_;

	/// key-value pairs of the parameters
	std::map<std::string, std::string> params_;

	/// collapsable status
	InsetCollapsable::CollapseStatus status_;
};


class invalidParam : public std::exception {
public:
	invalidParam(docstring const & details)
					: details_(to_utf8(details))
	{}

	virtual const char * what() const throw() {
		return details_.c_str();
	}

	virtual ~invalidParam() throw() {}
private:
	std::string const details_;
};


} // namespace lyx

#endif
