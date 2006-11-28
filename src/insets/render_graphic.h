// -*- C++ -*-
/**
 * \file render_graphic.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Angus Leeming
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef RENDER_GRAPHIC_H
#define RENDER_GRAPHIC_H

#include "render_base.h"

#include "graphics/GraphicsLoader.h"
#include "graphics/GraphicsParams.h"


namespace lyx {


class RenderGraphic : public RenderBase
{
public:
	RenderGraphic(InsetBase const *);
	RenderGraphic(RenderGraphic const &, InsetBase const *);
	std::auto_ptr<RenderBase> clone(InsetBase const *) const;

	/// compute the size of the object returned in dim
	bool metrics(MetricsInfo & mi, Dimension & dim) const;
	/// draw inset
	void draw(PainterInfo & pi, int x, int y) const;

	/// Refresh the info about which file to display and how to display it.
	void update(graphics::Params const & params);

	/// equivalent to dynamic_cast
	virtual RenderGraphic * asGraphic() { return this; }

private:
	/// Not implemented.
	RenderGraphic & operator=(RenderGraphic const &);

	/// The stored data.
	graphics::Loader loader_;
	graphics::Params params_;
};



} // namespace lyx

#endif // NOT RENDER_GRAPHIC_H
