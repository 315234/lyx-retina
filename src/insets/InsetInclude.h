// -*- C++ -*-
/**
 * \file InsetInclude.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Lars Gullik Bjønnes
 * \author Richard Heck (conversion to InsetCommand)
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef INSET_INCLUDE_H
#define INSET_INCLUDE_H

#include "InsetCommand.h"

#include "BiblioInfo.h"
#include "Counters.h"
#include "InsetCommandParams.h"
#include "RenderButton.h"

#include "support/FileNameList.h"

#include <boost/scoped_ptr.hpp>

namespace lyx {

class Buffer;
class Dimension;
class InsetLabel;
class LaTeXFeatures;
class RenderMonitoredPreview;

/// for including tex/lyx files
class InsetInclude : public InsetCommand {
public:
	///
	InsetInclude(Buffer * buf, InsetCommandParams const &);
	~InsetInclude();

	void setBuffer(Buffer & buffer);
	bool isLabeled() const { return true; }

	/// Override these InsetButton methods if Previewing
	void metrics(MetricsInfo & mi, Dimension & dim) const;
	///
	void draw(PainterInfo & pi, int x, int y) const;
	///
	DisplayType display() const;
	///
	InsetCode lyxCode() const { return INCLUDE_CODE; }

	///
	void setChildBuffer(Buffer * buffer);

	/** Fills \c keys
	 *  \param buffer the Buffer containing this inset.
	 *  \param keys the list of bibkeys in the child buffer.
	 *  \param it not used here
	 */
	void fillWithBibKeys(BiblioInfo & keys, InsetIterator const & it) const;
	
	/** Update the cache with all bibfiles in use of the child buffer
	 *  (including bibfiles of grandchild documents).
	 *  Does nothing if the child document is not loaded to prevent
	 *  automatic loading of all child documents upon loading the master.
	 *  \param buffer the Buffer containing this inset.
	 */
	void updateBibfilesCache();
	/** Return the cache with all bibfiles in use of the child buffer
	 *  (including bibfiles of grandchild documents).
	 *  Return an empty vector if the child doc is not loaded.
	 *  \param buffer the Buffer containing this inset.
	 */
	support::FileNameList const &
		getBibfilesCache() const;
	///
	bool hasSettings() const { return true; }
	///
	int latex(odocstream &, OutputParams const &) const;
	///
	int plaintext(odocstream &, OutputParams const &) const;
	///
	int docbook(odocstream &, OutputParams const &) const;
	///
	docstring xhtml(XHTMLStream &, OutputParams const &) const;
	///
	void validate(LaTeXFeatures &) const;
	///
	void addPreview(DocIterator const &, graphics::PreviewLoader &) const;
	///
	void addToToc(DocIterator const &);
	///
	void updateCommand();
	///
	void updateLabels(ParIterator const &, bool);
	///
	static ParamInfo const & findInfo(std::string const &);
	///
	static std::string defaultCommand() { return "include"; }
	///
	static bool isCompatibleCommand(std::string const & s);
	///
	docstring contextMenu(BufferView const & bv, int x, int y) const;
	/// \return the child buffer if the file is a LyX doc and could be loaded
	Buffer * getChildBuffer() const;
protected:
	InsetInclude(InsetInclude const &);
	///
	void doDispatch(Cursor & cur, FuncRequest & cmd);
	///
	bool getStatus(Cursor & cur, FuncRequest const & cmd, FuncStatus &) const;
private:
	Inset * clone() const { return new InsetInclude(*this); }

	/** Slot receiving a signal that the external file has changed
	 *  and the preview should be regenerated.
	 */
	void fileChanged() const;

	/// \return loaded Buffer or zero if the file loading did not proceed.
	Buffer * loadIfNeeded() const;
	/// launch external application
	void editIncluded(std::string const & file);
	/// set the parameters
	void setParams(InsetCommandParams const & params);
	/// get the text displayed on the button
	docstring screenLabel() const;
	/// holds the entity name that defines the file location (SGML)
	docstring const include_label;

	/// The pointer never changes although *preview_'s contents may.
	boost::scoped_ptr<RenderMonitoredPreview> const preview_;

	/// 
	mutable bool failedtoload_;
	/// cache
	mutable bool set_label_;
	mutable RenderButton button_;
	mutable docstring listings_label_;
	InsetLabel * label_;
	mutable Buffer * child_buffer_;
};


} // namespace lyx

#endif // INSET_INCLUDE_H
