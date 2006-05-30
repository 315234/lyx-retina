// -*- C++ -*-
/**
 * \file lyx_gui.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author John Levon
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef LYX_GUI_H
#define LYX_GUI_H


#include "FuncStatus.h"

#include <boost/function.hpp>

#include <string>
#include <vector>

class Dialogs;
class LColor_color;
class LyXFont;
class LyXComm;
class LyXDataSocket;
class LyXServerSocket;
class FuncRequest;
namespace lyx {
struct RGBColor;
}

/// GUI interaction
namespace lyx_gui {

/// are we using the GUI at all
extern bool use_gui;

/// return a suitable serif font name (called from non-gui context too !)
std::string const roman_font_name();

/// return a suitable sans serif font name (called from non-gui context too !)
std::string const sans_font_name();

/// return a suitable monospaced font name (called from non-gui context too !)
std::string const typewriter_font_name();

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
void start(std::string const & batch, std::vector<std::string> const & files,
	        unsigned int width, unsigned int height, int posx, int posy);

/**
 * Synchronise all pending events.
 */
void sync_events();

/**
 * quit running LyX
 */
void exit();

/**
 * return the status flag for a given action. This can be used to tell
 * that a given lfun is not implemented by a frontend
 */
FuncStatus getStatus(FuncRequest const & ev);

/**
 * Given col, fills r, g, b in the range 0-255.
 * The function returns true if successful.
 * It returns false on failure and sets r, g, b to 0.
 */
bool getRGBColor(LColor_color col, lyx::RGBColor & rgbcol);

/** Eg, passing LColor::black returns "000000",
 *      passing LColor::white returns "ffffff".
 */
std::string const hexname(LColor_color col);

/**
 * update an altered GUI color
 */
void update_color(LColor_color col);

/**
 * update the font cache
 */
void update_fonts();

/**
 * is the given font available ?
 */
bool font_available(LyXFont const & font);

/**
 * add a callback for socket read notification
 * @param fd socket descriptor (file/socket/etc)
 */
void register_socket_callback(int fd, boost::function<void()> func);

/**
 * remove a I/O read callback
 * @param fd socket descriptor (file/socket/etc)
 */
void unregister_socket_callback(int fd);

} // namespace lyx_gui

#endif // LYX_GUI_H
