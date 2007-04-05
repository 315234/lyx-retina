// -*- C++ -*-
/**
 * \file validators.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Angus Leeming
 * \author Richard Heck
 *
 * Full author contact details are available in file CREDITS.
 *
 * Validators are used to decide upon the legality of some input action.
 * For example, a "line edit" widget might be used to input a "glue length".
 * The correct syntax for such a length is "2em + 0.5em". The LengthValidator
 * below will report whether the input text conforms to this syntax.
 *
 * This information is used in LyX primarily to give the user some
 * feedback on the validity of the input data using the "checked_widget"
 * concept. For example, if the data is invalid then the label of
 * a "line edit" widget is changed in colour and the dialog's "Ok"
 * and "Apply" buttons are disabled. See checked_widgets.[Ch] for
 * further details.
 */

#ifndef VALIDATORS_H
#define VALIDATORS_H

#include "lyxlength.h"
#include "lyxgluelength.h"

#include <QValidator>

class QWidget;
class QLineEdit;


namespace lyx {

/** A class to ascertain whether the data passed to the @c validate()
 *  member function can be interpretted as a LyXGlueLength.
 */
class LengthValidator : public QValidator
{
	Q_OBJECT
public:
	/// Define a validator for widget @c parent.
	LengthValidator(QWidget * parent);

	/** @returns QValidator::Acceptable if @c data is a LyXGlueLength.
	 *  If not, returns QValidator::Intermediate.
	 */
	QValidator::State validate(QString & data, int &) const;

	/** @name Bottom
	 *  Set and retrieve the minimum allowed LyXLength value.
	 */
	//@{
	void setBottom(LyXLength const &);
	void setBottom(LyXGlueLength const &);
	LyXLength bottom() const { return b_; }
	//@}

private:
#if defined(Q_DISABLE_COPY)
	LengthValidator( const LengthValidator & );
	LengthValidator& operator=( const LengthValidator & );
#endif

	LyXLength b_;
	LyXGlueLength g_;
	bool no_bottom_;
	bool glue_length_;
};


/// @returns a new @c LengthValidator that does not accept negative lengths.
LengthValidator * unsignedLengthValidator(QLineEdit *);

//FIXME This should be generalized to take "text" as part of the
//constructor and so to set what text we check for, rather than
//hard-coding it as "auto". But see qt_helpers.h for reasons this
//is not so trivial and an idea about how to do it. (RGH)
/** A class to ascertain whether the data passed to the @c validate()
 *  member function can be interpretted as a LyXGlueLength or is "auto".
 */
class LengthAutoValidator : public LengthValidator
{
	Q_OBJECT
	public:
	/// Define a validator for widget @c parent.
		LengthAutoValidator(QWidget * parent);

	/** @returns QValidator::Acceptable if @c data is a LyXGlueLength
		* or is "auto". If not, returns QValidator::Intermediate.
	 */
		QValidator::State validate(QString & data, int &) const;
};

/// @returns a new @c LengthAutoValidator that does not accept negative lengths.
LengthAutoValidator * unsignedLengthAutoValidator(QLineEdit *);

//FIXME As above, this should really take a text argument.
/**
 * A class to determine whether the passed is a double
 * or is "auto".
 *
 */
class DoubleAutoValidator : public QDoubleValidator {
	Q_OBJECT
	public:
		DoubleAutoValidator(QWidget * parent);
		DoubleAutoValidator(double bottom, double top, int decimals, 
			QObject * parent);
		QValidator::State validate(QString & input, int & pos) const;
};

// Forward declarations
class LyXRC;

namespace frontend { class KernelDocType; } 


/** A class to ascertain whether the data passed to the @c validate()
 *  member function is a valid file path.
 *  The test is active only when the path is to be stored in a LaTeX
 *  file, LaTeX being quite picky about legal names.
 */
class PathValidator : public QValidator
{
	Q_OBJECT
public:
	/** Define a validator for widget @c parent.
	 *  If @c acceptable_if_empty is @c true then an empty path
	 *  is regarded as acceptable.
	 */
	PathValidator(bool acceptable_if_empty, QWidget * parent);

	/** @returns QValidator::Acceptable if @c data is a valid path.
	 *  If not, returns QValidator::Intermediate.
	 */
	QValidator::State validate(QString &, int &) const;

	/** Define what checks that @c validate() will perform.
	 *  @param doc_type checks are activated only for @c LATEX docs.
	 *  @param lyxrc contains a @c tex_allows_spaces member that
	 *  is used to define what is legal.
	 */
	void setChecker(frontend::KernelDocType const & doc_type,
			LyXRC const & lyxrc);

private:
#if defined(Q_DISABLE_COPY)
	PathValidator(const PathValidator &);
	PathValidator & operator=(const PathValidator &);
#endif

	bool acceptable_if_empty_;
	bool latex_doc_;
	bool tex_allows_spaces_;
};


/// @returns the PathValidator attached to the widget, or 0.
PathValidator * getPathValidator(QLineEdit *);

} // namespace lyx

# endif // NOT VALIDATORS_H
