// -*- C++ -*-
/**
 * \file QCharacter.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Edwin Leuven
 * \author John Levon
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef QCHARACTER_H_CONTROLLER
#define QCHARACTER_H_CONTROLLER

#include "QDialogView.h"
#include "ui/CharacterUi.h"
#include <QDialog>

#include "controllers/frontend_helpers.h"

#include <vector>

namespace lyx {

class Font;

namespace frontend {

class QCharacter;

class QCharacterDialog : public QDialog, public Ui::QCharacterUi {
	Q_OBJECT
public:
	QCharacterDialog(QCharacter * form);
protected:
	void closeEvent(QCloseEvent * e);
private:
	QCharacter * form_;
protected Q_SLOTS:
	void change_adaptor();
};


class ControlCharacter;

class QCharacter
	: public QController<ControlCharacter, QView<QCharacterDialog> >
{
public:
	friend class QCharacterDialog;

	QCharacter(Dialog &);
private:
	/// Apply changes
	virtual void apply();
	/// update
	virtual void update_contents();
	/// build the dialog
	virtual void build_dialog();

	std::vector<FamilyPair> family;
	std::vector<SeriesPair> series;
	std::vector<ShapePair>  shape;
	std::vector<SizePair>   size;
	std::vector<BarPair>    bar;
	std::vector<ColorPair>  color;
	std::vector<LanguagePair> language;
};

} // namespace frontend
} // namespace lyx

#endif // QCHARACTER_H
