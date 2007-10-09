// -*- C++ -*-
/**
 * \file GuiGraphics.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author John Levon
 * \author Herbert Vo�
 * \author Baruch Even
 * \author Angus Leeming
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef GUIGRAPHICS_H
#define GUIGRAPHICS_H

#include "GuiDialog.h"
#include "ui_GraphicsUi.h"

#include "support/docstring.h"
#include "insets/InsetGraphics.h"

#include <utility>
#include <vector>

class QString;

namespace lyx {

class InsetGraphics;
class InsetGraphicsParams;

namespace frontend {

class LyXView;

class GuiGraphics : public GuiDialog, public Ui::GraphicsUi
{
	Q_OBJECT

public:
	GuiGraphics(LyXView & lv);
	void setAutoText();

private Q_SLOTS:
	void change_adaptor();
	void change_bb();
	void on_browsePB_clicked();
	void on_getPB_clicked();
	void on_editPB_clicked();
	void on_filename_textChanged(const QString &);
	void on_scaleCB_toggled(bool);
	void on_WidthCB_toggled(bool);
	void on_HeightCB_toggled(bool);
	void on_angle_textChanged(const QString &);

private:
	void closeEvent(QCloseEvent * e);
	bool isValid();
	/// Apply changes
	void applyView();
	/// update
	void updateContents();
	/// get bounding box from file
	void getBB();

	/// Store the LaTeX names for the rotation origins.
	std::vector<std::string> origin_ltx;
	///
	bool initialiseParams(std::string const & data);
	/// clean-up on hide.
	void clearParams();
	/// clean-up on hide.
	void dispatchParams();
	///
	bool isBufferDependent() const { return true; }

	/// Browse for a file
	docstring const browse(docstring const &) const;
	/// Read the Bounding Box from a eps or ps-file
	std::string const readBB(std::string const & file);
	/// Control the bb
	bool bbChanged;
	/// test if file exist
	bool isFilenameValid(std::string const & fname) const;
	/// edit file
	void editGraphics();

private:
	///
	InsetGraphicsParams params_;
};


/// get the units for the bounding box
std::vector<std::string> const getBBUnits();

/// The (tranlated) GUI std::string and it's LaTeX equivalent.
typedef std::pair<docstring, std::string> RotationOriginPair;
///
std::vector<RotationOriginPair> getRotationOriginData();

} // namespace frontend
} // namespace lyx

#endif // GUIGRAPHICS_H
