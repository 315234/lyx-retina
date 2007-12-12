/**
 * \file GraphicsLoader.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Angus Leeming
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>

#include "GraphicsLoader.h"

#include "GraphicsCacheItem.h"
#include "GraphicsImage.h"
#include "GraphicsParams.h"
#include "GraphicsCache.h"
#include "support/debug.h"

#include "support/Timeout.h"

#include <boost/bind.hpp>

#include <set>
#include <queue>

using namespace std;
using namespace lyx::support;

namespace lyx {
namespace graphics {


/////////////////////////////////////////////////////////////////////
//
// LoaderQueue
//
/////////////////////////////////////////////////////////////////////

class LoaderQueue {
public:
	/// Use this to request that the item is loaded.
	void touch(Cache::ItemPtr const & item);
	/// Query whether the clock is ticking.
	bool running() const;
	///get the and only instance of the class
	static LoaderQueue & get();
private:
	/// This class is a singleton class... use LoaderQueue::get() instead
	LoaderQueue();
	/// The in-progress loading queue (elements are unique here).
	std::list<Cache::ItemPtr> cache_queue_;
	/// Used to make the insertion of new elements faster.
	std::set<Cache::ItemPtr> cache_set_;
	/// Newly touched elements go here. loadNext moves them to cache_queue_
	std::queue<Cache::ItemPtr> bucket_;
	///
	Timeout timer;
	///
	bool running_;

	/** This is the 'threaded' method, that does the loading in the
	 *  background.
	 */
	void loadNext();
	///
	void startLoader();
	///
	void stopLoader();
};


//static int s_numimages_ = 5;
//static int s_millisecs_ = 500;

static int s_numimages_ = 10;
static int s_millisecs_ = 500;

LoaderQueue & LoaderQueue::get()
{
	static LoaderQueue singleton;
	return singleton;
}


void LoaderQueue::loadNext()
{
	LYXERR(Debug::GRAPHICS, "LoaderQueue: "
		<< cache_queue_.size() << " items in the queue");
	int counter = s_numimages_;
	while (cache_queue_.size() && counter--) {
		Cache::ItemPtr ptr = cache_queue_.front();
		cache_set_.erase(ptr);
		cache_queue_.pop_front();
		if (ptr->status() == WaitingToLoad)
			ptr->startLoading();
	}
	if (cache_queue_.size()) {
		startLoader();
	} else {
		stopLoader();
	}
}


LoaderQueue::LoaderQueue() : timer(s_millisecs_, Timeout::ONETIME),
			     running_(false)
{
	timer.timeout.connect(boost::bind(&LoaderQueue::loadNext, this));
}


void LoaderQueue::startLoader()
{
	LYXERR(Debug::GRAPHICS, "LoaderQueue: waking up");
	running_ = true ;
	timer.setTimeout(s_millisecs_);
	timer.start();
}


void LoaderQueue::stopLoader()
{
	timer.stop();
	running_ = false ;
	LYXERR(Debug::GRAPHICS, "LoaderQueue: I'm going to sleep");
}


bool LoaderQueue::running() const
{
	return running_ ;
}


void LoaderQueue::touch(Cache::ItemPtr const & item)
{
	if (! cache_set_.insert(item).second) {
		list<Cache::ItemPtr>::iterator
			it = cache_queue_.begin();
		list<Cache::ItemPtr>::iterator
			end = cache_queue_.end();

		it = std::find(it, end, item);
		if (it != end)
			cache_queue_.erase(it);
	}
	cache_queue_.push_front(item);
	if (!running_)
		startLoader();
}



/////////////////////////////////////////////////////////////////////
//
// GraphicsLoader
//
/////////////////////////////////////////////////////////////////////

typedef boost::shared_ptr<Image> ImagePtr;

class Loader::Impl : public boost::signals::trackable {
public:
	///
	Impl();
	///
	~Impl();
	///
	void resetFile(FileName const &);
	///
	void resetParams(Params const &);
	///
	void createPixmap();
	///
	void startLoading();
	///
	Params const & params() const { return params_; }

	/// The loading status of the image.
	ImageStatus status_;
	/** Must store a copy of the cached item to ensure that it is not
	 *  erased unexpectedly by the cache itself.
	 */
	Cache::ItemPtr cached_item_;
	/// We modify a local copy of the image once it is loaded.
	ImagePtr image_;
	/// This signal is emitted when the image loading status changes.
	boost::signal<void()> signal_;
	/// The connection of the signal StatusChanged 	
	boost::signals::connection sc_;

private:
	///
	void statusChanged();
	///
	void checkedLoading();

	///
	Params params_;
};


Loader::Loader()
	: pimpl_(new Impl)
{}


Loader::Loader(FileName const & file, DisplayType type)
	: pimpl_(new Impl)
{
	reset(file, type);
}


Loader::Loader(FileName const & file, Params const & params)
	: pimpl_(new Impl)
{
	reset(file, params);
}


Loader::Loader(Loader const & other)
	: pimpl_(new Impl)
{
	Params const & params = other.pimpl_->params();
	reset(params.filename, params);
}


Loader::~Loader()
{
	delete pimpl_;
}


Loader & Loader::operator=(Loader const & other)
{
	if (this != &other) {
		Params const & params = other.pimpl_->params();
		reset(params.filename, params);
	}
	return *this;
}


void Loader::reset(FileName const & file, DisplayType type) const
{
	Params params;
	params.display = type;
	pimpl_->resetParams(params);

	pimpl_->resetFile(file);
	pimpl_->createPixmap();
}


void Loader::reset(FileName const & file, Params const & params) const
{
	pimpl_->resetParams(params);
	pimpl_->resetFile(file);
	pimpl_->createPixmap();
}


void Loader::reset(Params const & params) const
{
	pimpl_->resetParams(params);
	pimpl_->createPixmap();
}


void Loader::startLoading() const
{
	if (pimpl_->status_ != WaitingToLoad || !pimpl_->cached_item_.get())
		return;
	pimpl_->startLoading();
}


void Loader::startMonitoring() const
{
	if (!pimpl_->cached_item_.get())
		return;

	pimpl_->cached_item_->startMonitoring();
}


bool Loader::monitoring() const
{
	if (!pimpl_->cached_item_.get())
		return false;

	return pimpl_->cached_item_->monitoring();
}


unsigned long Loader::checksum() const
{
	if (!pimpl_->cached_item_.get())
		return 0;

	return pimpl_->cached_item_->checksum();
}


FileName const & Loader::filename() const
{
	static FileName const empty;
	return pimpl_->cached_item_.get() ?
		pimpl_->cached_item_->filename() : empty;
}


ImageStatus Loader::status() const
{
	return pimpl_->status_;
}


boost::signals::connection Loader::connect(slot_type const & slot) const
{
	return pimpl_->signal_.connect(slot);
}


Image const * Loader::image() const
{
	return pimpl_->image_.get();
}


Loader::Impl::Impl()
	: status_(WaitingToLoad)
{
}


Loader::Impl::~Impl()
{
	resetFile(FileName());
}


void Loader::Impl::resetFile(FileName const & file)
{
	FileName const old_file = cached_item_.get() ?
		cached_item_->filename() : FileName();

	if (file == old_file)
		return;

	// If monitoring() the current file, should continue to monitor the
	// new file.
	bool continue_monitoring = false;

	if (!old_file.empty()) {
		continue_monitoring = cached_item_->monitoring();
		// cached_item_ is going to be reset, so the connected
		// signal needs to be disconnected.
		sc_.disconnect();
		cached_item_.reset();
		Cache::get().remove(old_file);
	}

	status_ = cached_item_.get() ? cached_item_->status() : WaitingToLoad;
	image_.reset();

	if (cached_item_.get() || file.empty())
		return;

	Cache & gc = Cache::get();
	if (!gc.inCache(file))
		gc.add(file);

	// We /must/ make a local copy of this.
	cached_item_ = gc.item(file);
	status_ = cached_item_->status();

	if (continue_monitoring && !cached_item_->monitoring())
		cached_item_->startMonitoring();

	sc_ = cached_item_->connect(boost::bind(&Impl::statusChanged, this));
}


void Loader::Impl::resetParams(Params const & params)
{
	if (params == params_)
		return;

	params_ = params;
	status_ = cached_item_.get() ? cached_item_->status() : WaitingToLoad;
	image_.reset();
}


void Loader::Impl::statusChanged()
{
	status_ = cached_item_.get() ? cached_item_->status() : WaitingToLoad;
	createPixmap();
	signal_();
}


void Loader::Impl::createPixmap()
{
	if (!cached_item_.get() ||
	    params_.display == NoDisplay || status_ != Loaded)
		return;

	image_.reset(cached_item_->image()->clone());

	// These do nothing if there's nothing to do
	image_->clip(params_);
	image_->rotate(params_);
	image_->scale(params_);

	bool const success = image_->setPixmap(params_);

	if (success) {
		status_ = Ready;
	} else {
		image_.reset();
		status_ = ErrorGeneratingPixmap;
	}
}

void Loader::Impl::startLoading()
{
	if (status_ != WaitingToLoad)
		return;

	LoaderQueue::get().touch(cached_item_);
}


} // namespace graphics
} // namespace lyx
