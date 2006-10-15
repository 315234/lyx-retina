// -*- C++ -*-
/**
 * \file NoGuiFontLoader.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Abdelrazak Younes
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef LYX_NO_GUI_FONTLOADER_H
#define LYX_NO_GUI_FONTLOADER_H

#include "frontends/FontLoader.h"

#include "frontends/NoGuiFontMetrics.h"

namespace lyx {
namespace frontend {

/// Dummy FontLoader for command-line output.
class NoGuiFontLoader: public FontLoader
{
public:
	///
	NoGuiFontLoader() {}
	///
	virtual ~NoGuiFontLoader() {}

	/// Update fonts after zoom, dpi, font names, or norm change
	virtual void update() {};

	/// Is the given font available ?
	virtual bool available(LyXFont const & f) { return false; };

	/// Get the Font metrics for this LyXFont
	virtual FontMetrics const & metrics(LyXFont const & f) { return metrics_; }

private:
	///
	NoGuiFontMetrics metrics_;
};

} // namespace frontend
} // namespace lyx

#endif // LYX_NO_GUI_FONTLOADER_H
