// -*- C++ -*-
/**
 * \file ControlPrefs.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author John Levon
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef CONTROLPREFS_H
#define CONTROLPREFS_H

#include "Dialog.h"
#include "converter.h"
#include "format.h"
#include "lyxrc.h"
#include "mover.h"

#include <string>
#include <vector>

class LColor_color;

namespace lyx {
namespace frontend {

class ControlPrefs : public Dialog::Controller {
public:
	ControlPrefs(Dialog &);
	///
	virtual bool initialiseParams(std::string const &);
	///
	virtual void clearParams() {}
	///
	virtual void dispatchParams();
	///
	virtual bool isBufferDependent() const { return false; }

	LyXRC & rc() { return rc_; }
	LyXRC const & rc() const { return rc_; }

	Converters & converters() { return converters_; }
	Converters const & converters() const { return converters_; }

	Formats & formats() { return formats_; }
	Formats const & formats() const { return formats_; }

	Movers & movers() { return movers_; }
	Movers const & movers() const { return movers_; }

	/// various file pickers
	std::string const browsebind(std::string const & file) const;
	std::string const browseUI(std::string const & file) const;
	std::string const browsekbmap(std::string const & file) const;
	std::string const browsedict(std::string const & file) const;

	/// general browse
	std::string const browse(std::string const & file,
				 std::string const & title) const;

	/// browse directory
	std::string const browsedir(std::string const & path,
				    std::string const & title) const;

	/// set a color
	void setColor(LColor_color col, std::string const & hex);

	/// update the screen fonts after change
	void updateScreenFonts();

	/// adjust the prefs paper sizes
	PAPER_SIZE const toPaperSize(int i) const;
	/// adjust the prefs paper sizes
	int const fromPaperSize(PAPER_SIZE papersize) const;

private:
	/// temporary lyxrc
	LyXRC rc_;

	/// temporary converters
	Converters converters_;

	/// temporary formats
	Formats formats_;

	/// temporary movers
	Movers movers_;

	/// A list of colors to be dispatched
	std::vector<std::string> colors_;

	bool redraw_gui_;
	bool update_screen_font_;
};

} // namespace frontend
} // namespace lyx

#endif // CONTROLPREFS_H
