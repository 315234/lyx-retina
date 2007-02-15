// -*- C++ -*-
/**
 * \file QDialogView.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Angus Leeming
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef QDIALOGVIEW_H
#define QDIALOGVIEW_H

#include "Dialog.h"

#include <boost/scoped_ptr.hpp>

#include <QApplication>
#include <QWidget>
#include <QObject>

namespace lyx {
namespace frontend {

class Qt2BC;

/** This class is an Qt2 GUI base class.
 */
class QDialogView : public QObject, public Dialog::View {
	Q_OBJECT
public:
	///
	QDialogView(Dialog &, docstring const &);
	///
	virtual ~QDialogView() {}
	///
	bool readOnly() const;

	/// the dialog has changed contents
	virtual void changed();

	///
	Qt2BC & bcview();

protected:
	/// build the actual dialog
	virtual void build_dialog() = 0;
	///
	virtual void build() = 0;
	/// Hide the dialog.
	virtual void hide();
	/// Create the dialog if necessary, update it and display it.
	virtual void show();
	/// update the dialog's contents
	virtual void update_contents() = 0;
	///
	virtual bool isVisible() const;

	/// is the dialog currently valid ?
	virtual bool isValid();

	/// are we updating ?
	bool updating_;

public Q_SLOTS:
	// dialog closed from WM
	void slotWMHide();

	// Restore button clicked
	void slotRestore();

	// OK button clicked
	void slotOK();

	// Apply button clicked
	void slotApply();

	// Close button clicked
	void slotClose();
private:
	/// Pointer to the actual instantiation of the Qt dialog
	virtual QWidget * form() const = 0;
};


template <class GUIDialog>
class QView: public QDialogView {
protected:
	QView(Dialog & p, docstring const & t): QDialogView(p, t)
	{}

	virtual ~QView() {}

	/// update the dialog
	virtual void update() {
		dialog_->setUpdatesEnabled(false);

		// protect the BC from unwarranted state transitions
		updating_ = true;
		update_contents();
		updating_ = false;

		dialog_->setUpdatesEnabled(true);
		dialog_->update();
	}

	/// Build the dialog
	virtual void build() {
		// protect the BC from unwarranted state transitions
		updating_ = true;
		build_dialog();
		updating_ = false;
	}

	/// Pointer to the actual instantiation of the Qt dialog
	virtual GUIDialog * form() const { return dialog_.get(); }

	/// Real GUI implementation.
	boost::scoped_ptr<GUIDialog> dialog_;
};


template <class Controller, class Base>
class QController: public Base
{
protected:
	///
	QController(Dialog & p, docstring const & t): Base(p, t)
	{}
public:
	/// The parent controller
	Controller & controller()
	{ return static_cast<Controller &>(this->getController()); }

	/// The parent controller
	Controller const & controller() const
	{ return static_cast<Controller const &>(this->getController()); }
};

} // namespace frontend
} // namespace lyx

#endif // QDIALOGVIEW_H
