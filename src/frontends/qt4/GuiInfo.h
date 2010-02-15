// -*- C++ -*-
/**
 * \file GuiInfo.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Abdelrazak Younes
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef GUI_INFO_H
#define GUI_INFO_H

#include "InsetParamsWidget.h"
#include "ui_InfoUi.h"

namespace lyx {

class Inset;

namespace frontend {

class GuiInfo : public InsetParamsWidget, public Ui::InfoUi
{
	Q_OBJECT

public:
	GuiInfo(QWidget * parent = 0);

private:
	/// \name InsetParamsWidget inherited methods
	//@{
	InsetCode insetCode() { return INFO_CODE; }
	FuncCode creationCode() { return LFUN_INFO_INSERT; }
	void paramsToDialog(Inset const *);
	docstring dialogToParams() const;
	//@}
};

} // namespace frontend
} // namespace lyx

#endif // GUI_INFO_H
