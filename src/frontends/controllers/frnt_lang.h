// -*- C++ -*-
/**
 * \file frnt_lang.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Angus Leeming
 *
 * Full author contact details are available in file CREDITS.
 *
 * Ease the use of internationalised language strings in the dialogs.
 */

#ifndef FRNT_LANG_H
#define FRNT_LANG_H

#include "support/docstring.h"

#include <utility>
#include <vector>

namespace lyx {
namespace frontend {

///
typedef std::pair<docstring, std::string> LanguagePair;

/** If the caller is the character dialog, add "No change" and "Reset"
 *  to the vector.
 */
std::vector<LanguagePair> const getLanguageData(bool character_dlg);

} // namespace frontend
} // namespace lyx

#endif // FRNT_LANG_H
