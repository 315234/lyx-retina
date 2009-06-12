/**
 * \file InsetHyperlink.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author José Matos
 * \author Uwe Stöhr
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>

#include "InsetHyperlink.h"

#include "DispatchResult.h"
#include "FuncRequest.h"
#include "LaTeXFeatures.h"
#include "OutputParams.h"

#include "support/docstream.h"
#include "support/gettext.h"
#include "support/lstrings.h"

using namespace std;
using namespace lyx::support;

namespace lyx {


InsetHyperlink::InsetHyperlink(InsetCommandParams const & p)
	: InsetCommand(p, "href")
{}


ParamInfo const & InsetHyperlink::findInfo(string const & /* cmdName */)
{
	static ParamInfo param_info_;
	if (param_info_.empty()) {
		param_info_.add("name", ParamInfo::LATEX_OPTIONAL);
		param_info_.add("target", ParamInfo::LATEX_REQUIRED);
		param_info_.add("type", ParamInfo::LATEX_REQUIRED);
	}
	return param_info_;
}


docstring InsetHyperlink::screenLabel() const
{
	docstring const temp = from_ascii("Hyperlink: ");

	docstring url;

	url += getParam("name");
	if (url.empty())
		url += getParam("target");

	// elide if long
	if (url.length() > 30) {
		url = url.substr(0, 10) + "..."
			+ url.substr(url.length() - 17, url.length());
	}
	return temp + url;
}


int InsetHyperlink::latex(odocstream & os,
						  OutputParams const & runparams) const
{
	docstring url = getParam("target");
	docstring name = getParam("name");
	static docstring const backslash = from_ascii("\\");
	static docstring const braces = from_ascii("{}");
	static char_type const chars_url[2] = {'%', '#'};
	static char_type const chars_name[6] = {
		'&', '_', '$', '%', '#', '^'};

	// For the case there is no name given, the target is set as name.
	// Do this before !url.empty() and !name.empty() to handle characters
	// like the "%" correctly.
	if (name.empty())
		name = url;

	if (!url.empty()) {
		// Replace the "\" character by its ASCII code according to the
		// URL specifications because "\" is not allowed in URLs and by
		// \href. Only do this when the following character is not also
		// a "\", because "\\" is valid code
		for (size_t i = 0, pos;
			(pos = url.find('\\', i)) != string::npos;
			i = pos + 2) {
			if (url[pos + 1] != '\\')
				url.replace(pos, 1, from_ascii("%5C"));
		}

		// The characters in chars_url[] need to be escaped in the url
		// field because otherwise LaTeX will fail when the hyperlink is
		// within an argument of another command, e.g. in a \footnote. It
		// is important that they are escaped as "\#" and not as "\#{}".
		for (int k = 0;	k < 2; k++)
			for (size_t i = 0, pos;
				(pos = url.find(chars_url[k], i)) != string::npos;
				i = pos + 2)
				url.replace(pos, 1, backslash + chars_url[k]);
		
		// add "http://" when the type is web (type = empty)
		// and no "://" or "run:" is given
		docstring type = getParam("type");
		if (url.find(from_ascii("://")) == string::npos
			&& url.find(from_ascii("run:")) == string::npos
			&& type.empty())
			url = from_ascii("http://") + url;

	} // end if (!url.empty())

	// The characters in chars_name[] need to be changed to a command when
	// they are in the name field.
	if (!name.empty()) {
		// handle the "\" character, but only when the following character
		// is not also a "\", because "\\" is valid code
		docstring const textbackslash = from_ascii("\\textbackslash{}");
		for (size_t i = 0, pos;
			(pos = name.find('\\', i)) != string::npos;
			i = pos + 2) {
			if (name[pos + 1] != '\\')
				name.replace(pos, 1, textbackslash);
		}
		// The characters in chars_name[] need to be changed to a command
		// when they are in the name field.
		// Therefore the treatment of "\" must be the first thing
		for (int k = 0;	k < 6; k++)
			for (size_t i = 0, pos;
				(pos = name.find(chars_name[k], i)) != string::npos;
				i = pos + 2)
				name.replace(pos, 1, backslash + chars_name[k] + braces);

		// replace the tilde by the \sim character as suggested in the
		// LaTeX FAQ for URLs
		docstring const sim = from_ascii("$\\sim$");
		for (size_t i = 0, pos;
			(pos = name.find('~', i)) != string::npos;
			i = pos + 1)
			name.replace(pos, 1, sim);

	}  // end if (!name.empty())
	
	if (runparams.moving_arg)
		os << "\\protect";

	// output the ready \href command
	os << "\\href{" << getParam("type") << url << "}{" << name << '}';

	return 0;
}


int InsetHyperlink::plaintext(odocstream & os, OutputParams const &) const
{
	odocstringstream oss;

	oss << '[' << getParam("target");
	if (getParam("name").empty())
		oss << ']';
	else
		oss << "||" << getParam("name") << ']';

	docstring const str = oss.str();
	os << str;
	return str.size();
}


int InsetHyperlink::docbook(odocstream & os, OutputParams const &) const
{
	os << "<ulink url=\""
	   << subst(getParam("target"), from_ascii("&"), from_ascii("&amp;"))
	   << "\">"
	   << getParam("name")
	   << "</ulink>";
	return 0;
}


void InsetHyperlink::xhtml(odocstream & os, OutputParams const &) const
{
	os << "<a href=\""
			// FIXME Do we need to do more escaping than this?
	   << subst(getParam("target"), from_ascii("&"), from_ascii("&amp;"))
	   << "\">"
	   << getParam("name")
	   << "</a>";
}


void InsetHyperlink::tocString(odocstream & os) const
{
	plaintext(os, OutputParams(0));
}


void InsetHyperlink::validate(LaTeXFeatures & features) const
{
	features.require("hyperref");
}


} // namespace lyx
