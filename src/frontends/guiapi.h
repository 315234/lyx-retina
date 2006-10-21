// -*- C++ -*-
/**
 * \file guiapi.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Lars Gullik Bj�nnes
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef GUIAPI_H
#define GUIAPI_H

namespace lyx {

class Dialogs;

extern "C" {

void gui_show_dialog(Dialogs *, char const * name, char const * data);

} // extern "C"


} // namespace lyx

#endif // GUIAPI_H
