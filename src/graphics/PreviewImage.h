// -*- C++ -*-
/**
 *  \file PreviewImage.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Angus Leeming
 *
 * Full author contact details are available in file CREDITS
 */

#ifndef PREVIEWIMAGE_H
#define PREVIEWIMAGE_H

#include "LString.h"
#include <boost/scoped_ptr.hpp>

class Inset;
class BufferView;

namespace grfx {

class PreviewLoader;
class Image;

class PreviewImage {
public:
	/** ascent = height * ascent_frac
	 *  descent = height * (1 - ascent_frac)
	 */
	PreviewImage(PreviewLoader & parent,
		     string const & latex_snippet,
		     string const & bitmap_file,
		     double ascent_frac);
	///
	~PreviewImage();

	///
	string const & snippet() const;
	///
	int ascent() const;
	///
	int descent() const;
	///
	int width() const;

	/** If the image is not yet loaded (WaitingToLoad), then this method
	 *  triggers that.
	 */
	Image const * image() const;

private:
	/// Use the Pimpl idiom to hide the internals.
	class Impl;
	/// The pointer never changes although *pimpl_'s contents may.
	boost::scoped_ptr<Impl> const pimpl_;
};

} // namespace grfx

#endif // PREVIEWIMAGE_H
