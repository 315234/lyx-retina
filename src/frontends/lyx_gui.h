// -*- C++ -*-
/**
 * \file lyx_gui.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author John Levon
 *
 * Full author contact details are available in file CREDITS
 */

#ifndef LYX_GUI_H
#define LYX_GUI_H

#include "LColor.h"
#include "LString.h"
#include <vector>

class Dialogs;
class LyXFont;
class LyXComm;

/// GUI interaction
namespace lyx_gui {

/// parse command line and do basic initialisation
void parse_init(int & argc, char * argv[]);

/**
 * set up GUI parameters. At this point lyxrc may
 * be used.
 */
void parse_lyxrc();

/**
 * Start the main event loop, after executing the given
 * batch commands, and loading the given documents
 */
void start(string const & batch, std::vector<std::string> const & files);

/**
 * quit running LyX
 */
void exit();

/** Eg, passing LColor::black returns "000000",
 *      passing LColor::white returns "ffffff".
 */
string const hexname(LColor::color col);

/**
 * update an altered GUI color
 */
void update_color(LColor::color col);

/**
 * update the font cache
 */
void update_fonts();

/**
 * is the given font available ?
 */
bool font_available(LyXFont const & font);

/**
 * add a callback for I/O read notification
 */
void set_read_callback(int fd, LyXComm * comm);

/**
 * remove a I/O read callback
 */
void remove_read_callback(int fd);

} // namespace lyx_gui

#endif // LYX_GUI_H
