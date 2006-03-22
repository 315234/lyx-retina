// -*- C++ -*-
/**
 * \file QPrefs.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author John Levon
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef QPREFS_H
#define QPREFS_H

#include "QDialogView.h"
#include <vector>

class Converters;
class Formats;
class Movers;

namespace lyx {
namespace frontend {

class QPrefsDialog;
class ControlPrefs;

class QPrefs
	: public QController<ControlPrefs, QView<QPrefsDialog> >
{
public:
	QPrefs(Dialog &);

	Converters & converters();
	Formats & formats();
	Movers & movers();

private:
	/// Apply changes
	virtual void apply();

	/// update (do we need this?)
	virtual void update_contents();

	/// build the dialog
	virtual void build_dialog();

};

} // namespace frontend
} // namespace lyx

#endif // QPREFS_H
