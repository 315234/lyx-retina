// -*- C++ -*-
/**
 * \file GuiClipboard.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author unknown
 * \author John Levon
 * \author Abdelrazak Younes
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef GUICLIPBOARD_H
#define GUICLIPBOARD_H

#include "frontends/Clipboard.h"

#include <QMimeData>
#include <QObject>
#include <QStringList>

namespace lyx {
namespace frontend {

class QMacPasteboardMimeGraphics;

/**
 *  \class CacheMimeData 
 * 
 *  This class is used in order to query the clipboard only once on 
 *  startup and once each time the contents of the clipboard changes.
 */
class CacheMimeData : public QMimeData
{
	Q_OBJECT
public:
	// LyX calls "on_dataChanged" on startup, so it is not necessary to
	// query the clipboard here.
	CacheMimeData()
	{}

	/// reads the clipboard and updates the cached_formats_
	void update();
	/// returns the cached list of formats supported by the object
	virtual QStringList formats() const { return cached_formats_; }
	/// reads the clipboard and returns the data
	QByteArray data(QString const & mimeType) const;

private:
	/// the cached list of formats supported by the object
	QStringList cached_formats_;
};


/**
 * The Qt4 version of the Clipboard.
 */
class GuiClipboard: public QObject, public Clipboard
{
	Q_OBJECT
public:
	GuiClipboard();

	/** Clipboard overloaded methods
	 */
	//@{
	std::string const getAsLyX() const;
	FileName getAsGraphics(Cursor const & cur, GraphicsType type) const;
	docstring const getAsText() const;
	void put(std::string const & lyx, docstring const & text);
	bool hasLyXContents() const;
	bool hasGraphicsContents(GraphicsType type = AnyGraphicsType) const;
	bool hasTextContents() const;
	bool isInternal() const;
	bool hasInternal() const;
	bool empty() const;
	//@}

	FileName getPastedGraphicsFileName(Cursor const & cur,
		Clipboard::GraphicsType & type) const;

private Q_SLOTS:
	void on_dataChanged();

private:
	bool text_clipboard_empty_;
	bool has_lyx_contents_;
	bool has_graphics_contents_;
	/// the cached mime data used to describe the information
	/// that can be stored in the clipboard
	CacheMimeData cache_;
};

QString const lyxMimeType();
QString const pdfMimeType();
QString const emfMimeType();
QString const wmfMimeType();

} // namespace frontend
} // namespace lyx

#endif // GUICLIPBOARD_H
