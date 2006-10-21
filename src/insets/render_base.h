// -*- C++ -*-
/**
 * \file render_base.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Angus Leeming
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef RENDER_BASE_H
#define RENDER_BASE_H

#include "dimension.h"

#include <memory>


namespace lyx {

class InsetBase;
class MetricsInfo;
class PainterInfo;

class RenderButton;
class RenderGraphic;
class RenderPreview;
class RenderMonitoredPreview;

class RenderBase {
public:
	virtual ~RenderBase() {}

	virtual std::auto_ptr<RenderBase> clone(InsetBase const *) const = 0;

	/// compute the size of the object returned in dim
	virtual void metrics(MetricsInfo & mi, Dimension & dim) const = 0;
	/// draw inset and update (xo, yo)-cache
	virtual void draw(PainterInfo & pi, int x, int y) const = 0;

	/// equivalent to dynamic_cast
	virtual RenderButton * asButton() { return 0; }
	virtual RenderGraphic * asGraphic() { return 0; }
	virtual RenderPreview * asPreview() { return 0; }
	virtual RenderMonitoredPreview * asMonitoredPreview() { return 0; }

protected:
	RenderBase() {}
	RenderBase(RenderBase const &) {}
	RenderBase & operator=(RenderBase const &) { return *this; }

	/// Cached
	mutable Dimension dim_;
};


} // namespace lyx

#endif // NOT RENDER_BASE_H
