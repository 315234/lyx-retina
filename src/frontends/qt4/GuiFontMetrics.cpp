/**
 * \file GuiFontMetrics.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author unknown
 * \author John Levon
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>

#include "GuiFontMetrics.h"

#include "qt_helpers.h"

#include "Language.h"
#include "Dimension.h"

#include "insets/Inset.h"

#include "support/lassert.h"

using namespace std;

namespace lyx {
namespace frontend {

namespace {
/**
 * Convert a UCS4 character into a QChar.
 * This is a hack (it does only make sense for the common part of the UCS4
 * and UTF16 encodings) and should not be used.
 * This does only exist because of performance reasons (a real conversion
 * using iconv is too slow on windows).
 *
 * This is no real conversion but a simple cast in reality. This is the reason
 * why this works well for symbol fonts used in mathed too, even though
 * these are not real ucs4 characters. These are codepoints in the
 * modern fonts used, nothing unicode related.
 * See comment in QLPainter::text() for more explanation.
 **/	
inline QChar const ucs4_to_qchar(char_type const ucs4)
{
	LASSERT(is_utf16(ucs4), /**/);
	return QChar(static_cast<unsigned short>(ucs4));
}
} // anon namespace


GuiFontMetrics::GuiFontMetrics(QFont const & font)
: metrics_(font, 0), smallcaps_metrics_(font), smallcaps_shape_(false)
{
}


GuiFontMetrics::GuiFontMetrics(QFont const & font, QFont const & smallcaps_font)
: metrics_(font, 0), smallcaps_metrics_(smallcaps_font), smallcaps_shape_(true)
{
}


int GuiFontMetrics::maxAscent() const
{
	return metrics_.ascent();
}


int GuiFontMetrics::maxDescent() const
{
	// We add 1 as the value returned by QT is different than X
	// See http://doc.trolltech.com/2.3/qfontmetrics.html#200b74
	return metrics_.descent() + 1;
}


int GuiFontMetrics::lbearing(char_type c) const
{
	if (!is_utf16(c))
		// FIXME: QFontMetrics::leftBearingdoes not support the
		//        full unicode range. Once it does, we could use:
		//return metrics_.leftBearing(toqstr(docstring(1, c)));
		return 0;

	return metrics_.leftBearing(ucs4_to_qchar(c));
}


namespace {
int const outOfLimitMetric = -10000;
}


int GuiFontMetrics::rbearing(char_type c) const
{
	int value = rbearing_cache_.value(c, outOfLimitMetric);
	if (value != outOfLimitMetric)
		return value;

	// Qt rbearing is from the right edge of the char's width().
	if (is_utf16(c)) {
		QChar sc = ucs4_to_qchar(c);
		value = width(c) - metrics_.rightBearing(sc);
	} else {
		// FIXME: QFontMetrics::leftBearing does not support the
		//        full unicode range. Once it does, we could use:
		// metrics_.rightBearing(toqstr(docstring(1, c)));
		value = width(c);
	}

	rbearing_cache_.insert(c, value);

	return value;
}


int GuiFontMetrics::smallcapsWidth(char_type c) const
{
	// FIXME: Optimisation probably needed: we don't use the width cache.
	if (is_utf16(c)) {
		QChar const qc = ucs4_to_qchar(c);
		QChar const uc = qc.toUpper();
		if (qc != uc)
			return smallcaps_metrics_.width(uc);
		else
			return metrics_.width(qc);
	} else {
		QString const s = toqstr(docstring(1, c));
		QString const us = s.toUpper();
		if (s != us)
			return smallcaps_metrics_.width(us);
		else
			return metrics_.width(s);
	}
}


int GuiFontMetrics::width(docstring const & s) const
{
	size_t ls = s.size();
	int w = 0;
	for (unsigned int i = 0; i < ls; ++i) {
		//FIXME: we need to detect surrogate pairs and act accordingly
		/**
		if isSurrogateBase(s[i]) {
			docstring c = s[i];
			if (smallcaps_shape_)
				w += metrics_.width(toqstr(c + s[i + 1]));
			else
				w += smallcaps_metrics_.width(toqstr(c + s[i + 1]));
			++i;
		}
		else
		*/
		w += width(s[i]);
	}

	return w;
}


int GuiFontMetrics::width(QString const & ucs2) const
{
	return width(qstring_to_ucs4(ucs2));
}


int GuiFontMetrics::signedWidth(docstring const & s) const
{
	if (s.empty())
		return 0;

	if (s[0] == '-')
		return -width(s.substr(1, s.size() - 1));
	else
		return width(s);
}


void GuiFontMetrics::rectText(docstring const & str,
	int & w, int & ascent, int & descent) const
{
	static int const d = Inset::TEXT_TO_INSET_OFFSET / 2;

	w = width(str) + Inset::TEXT_TO_INSET_OFFSET;
	ascent = metrics_.ascent() + d;
	descent = metrics_.descent() + d;
}



void GuiFontMetrics::buttonText(docstring const & str,
	int & w, int & ascent, int & descent) const
{
	rectText(str, w, ascent, descent);
	w += Inset::TEXT_TO_INSET_OFFSET;
}


Dimension const GuiFontMetrics::defaultDimension() const
{
	return Dimension(0, maxAscent(), maxDescent());
}


Dimension const GuiFontMetrics::dimension(char_type c) const
{
	return Dimension(width(c), ascent(c), descent(c));
}


GuiFontMetrics::AscendDescend const GuiFontMetrics::fillMetricsCache(
		char_type c) const
{
	QRect r;
	if (is_utf16(c))
		r = metrics_.boundingRect(ucs4_to_qchar(c));
	else
		r = metrics_.boundingRect(toqstr(docstring(1, c)));

	AscendDescend ad = { -r.top(), r.bottom() + 1};
	// We could as well compute the width but this is not really
	// needed for now as it is done directly in width() below.
	metrics_cache_.insert(c, ad);

	return ad;
}


int GuiFontMetrics::width(char_type c) const
{
	if (smallcaps_shape_)
		return smallcapsWidth(c);

	int value = width_cache_.value(c, outOfLimitMetric);
	if (value != outOfLimitMetric)
		return value;

	if (is_utf16(c))
		value = metrics_.width(ucs4_to_qchar(c));
	else
		value = metrics_.width(toqstr(docstring(1, c)));

	width_cache_.insert(c, value);

	return value;
}


int GuiFontMetrics::ascent(char_type c) const
{
	static AscendDescend const outOfLimitAD = 
		{outOfLimitMetric, outOfLimitMetric};
	AscendDescend value = metrics_cache_.value(c, outOfLimitAD);
	if (value.ascent != outOfLimitMetric)
		return value.ascent;

	value = fillMetricsCache(c);
	return value.ascent;
}


int GuiFontMetrics::descent(char_type c) const
{
	static AscendDescend const outOfLimitAD = 
		{outOfLimitMetric, outOfLimitMetric};
	AscendDescend value = metrics_cache_.value(c, outOfLimitAD);
	if (value.descent != outOfLimitMetric)
		return value.descent;

	value = fillMetricsCache(c);
	return value.descent;
}

} // namespace frontend
} // namespace lyx
