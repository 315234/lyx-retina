// -*- C++ -*-
/**
 * \file InsetCommandParams.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Angus Leeming
 * \author Georg Baum
 * \author Richard Heck
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef INSETCOMMANDPARAMS_H
#define INSETCOMMANDPARAMS_H

#include "InsetCode.h"
#include "support/docstring.h"

#include <iosfwd>
#include <string>
#include <vector>
#include <map>


namespace lyx {

class Lexer;

class ParamInfo {
public:
	/// Types of parameters
	/// WARNING: LATEX_KV_* `parameters' aren't really parameters at all
	/// but merely markers for where the keyval-type parameters should
	/// appear in the LaTeX output. ParamInfo::hasParam(name) therefore 
	/// returns FALSE if the corresponding `parameter' is of type
	/// LATEX_KV_*.
	/// It is assumed here that there is exactly one argument that accepts
	/// the key=value pairs.
	enum ParamType {
		LATEX_OPTIONAL,    /// normal optional argument
		LATEX_REQUIRED,    /// normal required argument
		LATEX_KV_OPTIONAL, /// optional argument that uses keyval
		LATEX_KV_REQUIRED, /// required argument that uses keyval
		LATEX_KEY,         /// a key to be used with keyval argument
		LYX_INTERNAL       /// a parameter used internally by LyX
	};
	///
	class ParamData {
	// No parameter may be named "preview", because that is a required
	// flag for all commands.
	public:
		///
		ParamData(std::string const &, ParamType);
		///
		std::string name() const { return name_; }
		///
		ParamType type() const { return type_; }
		/// whether this is a key for use with keyval
		bool isKey() const
			{ return type_ == LATEX_KEY; }
		/// whether this is an optional LaTeX argument
		inline bool isOptional() const;
		/// whether this is a keyval argument
		inline bool isKeyValArg() const;
#if 0
		//presently unused but perhaps useful at some point
		/// whether this is a required LaTeX argument
		bool isRequired() const
			{ return type_ == ParamInfo::LATEX_REQUIRED ||
					type_ == ParamInfo::LATEX_KV_REQUIRED; }
		/// whether this is a LaTeX argument
		inline bool isLaTeXArgument() const
			{ return isOptional() || isRequired(); }
#endif
		///
		bool operator==(ParamData const &) const;
		/// 
		bool operator!=(ParamData const & rhs) const
			{ return !(*this == rhs); }
	private:
		///
		std::string name_;
		///
		ParamType type_;
	};

	/// adds a new parameter
	void add(std::string const & name, ParamType type);
	///
	bool empty() const { return info_.empty(); }
	///
	size_t size() const { return info_.size(); }
	///
	typedef std::vector<ParamData>::const_iterator const_iterator;
	///
	const_iterator const begin() const { return info_.begin(); }
	///
	const_iterator const end() const { return info_.end(); }
	/// \return true if name corresponds to a parameter of some sort.
	/// \return false if the parameter does not exist at all of it it 
	/// corresponds to a `parameter' of type LATEX_KV_*; these do not 
	/// really represent parameters but just argument places.
	bool hasParam(std::string const & name) const;
	///
	ParamData const & operator[](std::string const & name) const;
	///
	bool operator==(ParamInfo const &) const;
private:
	///
	std::vector<ParamData> info_;
};


class InsetCommandParams {
public:
	/// Construct parameters for inset of type \p code.
	explicit InsetCommandParams(InsetCode code);
	/// Construct parameters for inset of type \p code with
	/// command name \p cmdName.
	explicit InsetCommandParams(InsetCode code,
			std::string const & cmdName);
	///
	std::string insetType() const { return insetName(insetCode_); }
	///
	InsetCode code() const { return insetCode_; }
	///
	void read(Lexer &);
	/// Parse the command
	///
	void write(std::ostream &) const;
	/// Build the complete LaTeX command
	docstring const getCommand() const;
	/// Return the command name
	std::string const & getCmdName() const { return cmdName_; }
	/// Set the name to \p n. This must be a known name. All parameters
	/// are cleared except those that exist also in the new command.
	/// What matters here is the parameter name, not position.
	void setCmdName(std::string const & n);
	/// FIXME Would be better removed, but is used in BufferView.cpp in 
	/// ways that make removal hard.
	docstring const getFirstNonOptParam() const;
	/// get parameter \p name
	/// WARNING: You cannot access LATEX_KV_* arguments in this way.
	/// LyX will assert if you attempt to do so.
	docstring const & operator[](std::string const & name) const;
	/// set parameter \p name
	/// WARNING: You cannot access LATEX_KV_* arguments in this way.
	/// LyX will assert if you attempt to do so.
	docstring & operator[](std::string const & name);
	///
	bool preview() const { return preview_; }
	///
	void preview(bool p) { preview_ = p; }
	/// Clear the values of all parameters
	void clear();

private:
	///
	/// Get information for inset type \p code.
	/// Returns 0 if the inset is not known.
	static ParamInfo const & findInfo(InsetCode code);
	/// Get information for \p code and command \p cmdName.
	/// Returns 0 if the combination is not known.
	/// Don't call this without first making sure the command name is
	/// acceptable to the inset.
	static ParamInfo const & findInfo(InsetCode code,
	                                    std::string const & cmdName);
	///
	static bool isCompatibleCommand(InsetCode code, std::string const & s);
	///
	std::string getDefaultCmd(InsetCode);
	///
	docstring makeKeyValArgument() const;
	/// checks whether we need to write an empty optional parameter
	/// \return true if a non-empty optional parameter follows ci
	bool writeEmptyOptional(ParamInfo::const_iterator ci) const;
	/// Description of all command properties
	ParamInfo info_;
	/// what kind of inset we're the parameters for
	InsetCode insetCode_;
	/// The name of this command as it appears in .lyx and .tex files
	std::string cmdName_;
	///
	// if we need to allow more than one value for a parameter, this
	// could be made a multimap. it may be that the only thing that
	// would then need changing is operator[].
	typedef std::map<std::string, docstring> ParamMap;
	/// The parameters, by name.
	ParamMap params_;
	///
	bool preview_;
	///
	friend bool operator==(InsetCommandParams const &,
			InsetCommandParams const &);
};

///
bool operator==(InsetCommandParams const &, InsetCommandParams const &);
///
bool operator!=(InsetCommandParams const &, InsetCommandParams const &);


} // namespace lyx

#endif
