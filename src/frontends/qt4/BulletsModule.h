// -*- C++ -*-
/**
 * \file BulletsModule.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Edwin Leuven
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef QBULLETSMODULE_H
#define QBULLETSMODULE_H


#include "ui/BulletsUi.h"
#include "Bullet.h"
#include <boost/array.hpp>

#include <QDialog>

class QBrowseBox;
class QPushButton;
class QComboBox;
class QPixmap;

class BulletsModule : public QDialog, public Ui::BulletsUi {
	Q_OBJECT
public:

	BulletsModule(QWidget * parent = 0, const char * name = 0, Qt::WFlags fl = 0);

	~BulletsModule();

	/// set a bullet
	void setBullet(int level, const Bullet & bullet);

	/// get bullet setting
	Bullet const & getBullet(int level) const;

signals:
	void changed();

protected slots:

	/// menu callbacks
	void standard(int row, int col);
	void maths(int row, int col);
	void ding1(int row, int col);
	void ding2(int row, int col);
	void ding3(int row, int col);
	void ding4(int row, int col);

	void updateSizes();

	void clicked1();
	void clicked2();
	void clicked3();
	void clicked4();

	void selected1();
	void selected2();
	void selected3();
	void selected4();

	/// set custom (text) bullet
	void setCustom();

private:
	QPixmap getPixmap(int font, int character);

	/// update GUI view
	void setBullet(QPushButton * pb, QComboBox * co, Bullet const & b);

	/// set from menu
	void setCurrentBullet(int font, int character);

	bool tmpbulletset;
	Bullet tmpbullet;

	/// store results
	boost::array<Bullet, 4> bullets_;

	QBrowseBox * standard_;
	QBrowseBox * maths_;
	QBrowseBox * ding1_;
	QBrowseBox * ding2_;
	QBrowseBox * ding3_;
	QBrowseBox * ding4_;
	Bullet * bullet_pressed_;
};

#endif // BULLETSMODULE_H
