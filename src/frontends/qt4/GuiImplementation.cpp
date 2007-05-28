// -*- C++ -*-
/**
 * \file GuiImplementation.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author John Levon
 * \author Abdelrazak Younes
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>

// This include must be declared before everything else because
// of boost/Qt/LyX clash...
#include "GuiView.h"

#include "GuiImplementation.h"
#include "GuiWorkArea.h"

#include "BufferView.h"
#include "BufferList.h"
#include "FuncRequest.h"
#include "LyXFunc.h"

#include <QApplication>

using boost::shared_ptr;


namespace
{
	template<class T>
	void updateIds(std::map<int, T*> const & stdmap, std::vector<int> & ids)
	{
		ids.clear();
		typename std::map<int, T*>::const_iterator it;
		for (it = stdmap.begin(); it != stdmap.end(); ++it)
			ids.push_back(it->first);
	}
}


namespace lyx {
namespace frontend {


GuiImplementation::GuiImplementation()
{
	view_ids_.clear();
	work_area_ids_.clear();
}


LyXView& GuiImplementation::createRegisteredView()
{
	updateIds(views_, view_ids_);
	int id = 0;
	while (views_.find(id) != views_.end())
		id++;
	views_.insert(std::pair<int, GuiView *>(id, new GuiView(id)));
	updateIds(views_, view_ids_);
	return *views_[id];
}


bool GuiImplementation::unregisterView(int id)
{
	updateIds(views_, view_ids_);
	BOOST_ASSERT(views_.find(id) != views_.end());
	BOOST_ASSERT(views_[id]);

	std::map<int, GuiView *>::iterator it;
	for (it = views_.begin(); it != views_.end(); ++it) {
		if (it->first == id) {
			std::vector<int> const & wa_ids = it->second->workAreaIds();
			for (size_t i = 0; i < wa_ids.size(); ++i)
				work_areas_.erase(wa_ids[i]);
			views_.erase(id);
			break;
		}
	}
	updateIds(views_, view_ids_);
	return true;
}


bool GuiImplementation::closeAllViews()
{
	updateIds(views_, view_ids_);
	if (views_.empty())
	{
		// quit in CloseEvent will not be triggert
		qApp->quit();
		return true;
	}

	std::map<int, GuiView*> const cmap = views_;
	std::map<int, GuiView*>::const_iterator it;
	for (it = cmap.begin(); it != cmap.end(); ++it)
	{
		// TODO: return false when close event was ignored
		//       e.g. quitWriteAll()->'Cancel'
		//       maybe we need something like 'bool closeView()'
		it->second->close();
		// unregisterd by the CloseEvent
	}

	views_.clear();
	work_areas_.clear();
	view_ids_.clear();
	work_area_ids_.clear();
	return true;
}


LyXView& GuiImplementation::view(int id) const
{
	BOOST_ASSERT(views_.find(id) != views_.end());
	return *views_.find(id)->second;
}


std::vector<int> const & GuiImplementation::workAreaIds()
{
	updateIds(work_areas_, work_area_ids_);
	return work_area_ids_;
}


int GuiImplementation::newWorkArea(unsigned int w, unsigned int h, int view_id)
{
	updateIds(views_, view_ids_);
	int id = 0;
	while (work_areas_.find(id) != work_areas_.end())
		id++;

	GuiView * view = views_[view_id];

	work_areas_.insert(std::pair<int, GuiWorkArea *>
					(id, new GuiWorkArea(w, h, id, *view)));

	// FIXME BufferView creation should be independant of WorkArea creation
	buffer_views_[id].reset(new BufferView);
	work_areas_[id]->setBufferView(buffer_views_[id].get());

	view->setWorkArea(work_areas_[id]);
	view->initTab(work_areas_[id]);

	return id;
}


WorkArea& GuiImplementation::workArea(int id)
{
	BOOST_ASSERT(work_areas_.find(id) != work_areas_.end());
	return *work_areas_[id];
}


} // namespace frontend
} // namespace lyx

#include "GuiImplementation_moc.cpp"
