// -*- C++ -*-
/**
 * \file Inset.heorem.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Lars Gullik Bj�nnes
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef Inset.heorem_H
#define Inset.heorem_H


#include "InsetCollapsable.h"


namespace lyx {

/** The theorem inset

*/
class Inset.heorem : public InsetCollapsable {
public:
	///
	Inset.heorem();
	///
	void write(Buffer const & buf, std::ostream & os) const;
	///
	Inset::Code lyxCode() const { return Inset::THEOREM_CODE; }
	///
	bool metrics(MetricsInfo &, Dimension &) const;
	///
	void draw(PainterInfo & pi, int x, int y) const;
	///
	int latex(Buffer const &, odocstream &,
		  OutputParams const &) const;
	///
	virtual docstring const editMessage() const;
private:
	virtual std::auto_ptr<Inset> doClone() const;

	///
	mutable unsigned int center_indent_;
};


} // namespace lyx

#endif
