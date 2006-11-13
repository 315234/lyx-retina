// -*- C++ -*-
/**
 * \file TocModel.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Abdelrazak Younes
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef TOCMODEL_H
#define TOCMODEL_H

#include "TocBackend.h"

#include "qt_helpers.h"

#include <QStandardItemModel>

#include <map>

namespace lyx {
namespace frontend {

class TocModel: public QStandardItemModel {
	Q_OBJECT

public:
	///
	TocModel() {}
	///
	TocModel(Toc const & toc);
	///
	~TocModel() {}
	///
	TocModel const & operator=(Toc const & toc);
	///
	void clear();
	///
	void populate(Toc const & toc);
	///
	TocIterator const tocIterator(QModelIndex const & index) const;
	///
	QModelIndex const modelIndex(TocIterator const & it) const;

private:
	///
	void populate(TocIterator & it,
		TocIterator const & end,
		QModelIndex const & parent);
	///
	typedef std::map<QModelIndex, TocIterator> TocMap;
	///
	typedef std::pair<QModelIndex, TocIterator> TocPair;
	///
	typedef std::map<TocIterator, QModelIndex> ModelMap;
	///
	TocMap toc_map_;
	///
	ModelMap model_map_;
};

} // namespace frontend
} // namespace lyx

#endif // TOCMODEL_H
