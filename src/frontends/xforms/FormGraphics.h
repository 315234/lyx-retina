// -*- C++ -*-
/**
 * \file FormGraphics.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Baruch Even
 * \author Herbert Voss
 *
 * Full author contact details are available in file CREDITS
 */

#ifndef FORMGRAPHICS_H
#define FORMGRAPHICS_H


#include "FormBase.h"

#include <boost/scoped_ptr.hpp>

#include <vector>

class ControlGraphics;
struct FD_graphics;
struct FD_graphics_file;
struct FD_graphics_bbox;
struct FD_graphics_extra;

/** This class provides an XForms implementation of the Graphics Dialog.
 */
class FormGraphics : public FormCB<ControlGraphics, FormDB<FD_graphics> > {
public:
	///
	FormGraphics();
private:

	/** Redraw the form (on receipt of a Signal indicating, for example,
	    that the xforms colours have been re-mapped). */
	virtual void redraw();
	/// Set the Params variable for the Controller.
	virtual void apply();
	/// Build the dialog.
	virtual void build();
	/// Update dialog before/whilst showing it.
	virtual void update();
	/// Update the BoundingBox info.
	void updateBB(string const & filename, string const & bb_inset);
	/// Filter the inputs on callback from xforms
	virtual ButtonPolicy::SMInput input(FL_OBJECT *, long);

	/// Real GUI implementation.
	boost::scoped_ptr<FD_graphics_file> file_;
	///
	boost::scoped_ptr<FD_graphics_bbox> bbox_;
	///
	boost::scoped_ptr<FD_graphics_extra> extra_;

	/// Store the LaTeX names for the rotation origins.
	std::vector<string> origins_;
};

#endif  // FORMGRAPHICS_H
