/**
 * \file GuiIdListModel.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Richard Heck
 *
 * Some of this code is based upon qstringlistmodel.{h,cpp}, which is
 * part of the Qt toolkit, copyright (C) 1992-2007 Trolltech ASA, and
 * released under the General Public License.
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>

#include "GuiIdListModel.h"

using std::vector;

namespace lyx {
namespace frontend {

QVariant GuiIdListModel::data(QModelIndex const & index, int role) const
{
	int const row = index.row();
	if (!rowIsValid(row))
		return QVariant();
	if (role == Qt::DisplayRole || role == Qt::EditRole)
		return userData_[row].uiString;
	if (role == Qt::UserRole)
		return userData_[row].idString;
	return QVariant();
}


bool GuiIdListModel::setData (QModelIndex const & index, 
		const QVariant & value, int role)
{
	int const row = index.row();
	if (!rowIsValid(row))
		return false;
	if (role == Qt::DisplayRole || role == Qt::EditRole) {
		userData_[row].uiString = value;
		dataChanged(index, index);
		return true;
	}
	if (role == Qt::UserRole) {
		userData_[row].idString = value;
		dataChanged(index, index);
		return true;
	}
	return false;
}


bool GuiIdListModel::insertRows(int row, int count, 
		QModelIndex const & /*parent*/)
{
	if (!rowIsValid(row))
		return false;
	vector<OurData>::iterator it = userData_.begin() + row;
	OurData const v;
	beginInsertRows(QModelIndex(), row, row + count - 1);
	userData_.insert(it, count, v);
	endInsertRows();
	return true;
}


bool GuiIdListModel::removeRows(int row, int count, 
		QModelIndex const & /*parent*/)
{
	if (!rowIsValid(row) || row + count > int(userData_.size()) ||
	    count < 0)
		return false;
	if (count == 0)
		return true;
	vector<OurData>::iterator it = userData_.begin() + row;
	beginRemoveRows(QModelIndex(), row, row + count - 1);
	userData_.erase(it, it + count);	
	endRemoveRows();
	return true;
}


void GuiIdListModel::insertRow(int const i, QString const & uiString, 
		std::string const & idString)
{
	insertRows(i, 1);
	setUIString(i, uiString);
	setIDString(i, idString);
}


QMap<int, QVariant> GuiIdListModel::itemData(QModelIndex const & index ) const
{
	int const row = index.row();
	if (!rowIsValid(row))
		return QMap<int, QVariant>();
	QMap<int, QVariant> qm = QAbstractListModel::itemData(index);
	qm[Qt::UserRole] = userData_[row].idString;
	return qm;
}

#if 0
// The following functions are currently unused but are retained here in
//   case they should at some point be useful.
   
QStringList GuiIdListModel::getIDStringList() const
{
	QStringList qsl;
	vector<OurData>::const_iterator it  = userData_.begin();
	vector<OurData>::const_iterator end = userData_.end();
	for (; it != end; ++it)
		qsl.append(it->idString.toString());
	return qsl;
}


void GuiIdListModel::insertRow(int const i, QString const & uiString, 
		QString const & idString)
{
	insertRows(i, 1);
	setUIString(i, uiString);
	setIDString(i, idString);
}

bool GuiIdListModel::containsID(QVariant const & q) const
{
	vector<OurData>::const_iterator it  = userData_.begin();
	vector<OurData>::const_iterator end = userData_.end();
	for (; it != end; ++it)
		if (it->idString == q)
			return true;
	return false;
}
#endif

} // namespace frontend
} // namespace lyx

