dnl check a particular libname
AC_DEFUN([QT4_TRY_LINK],
[
	SAVE_LIBS="$LIBS"
	LIBS="$LIBS $1"
	AC_TRY_LINK([
	#include <qglobal.h>
	#include <qstring.h>
		],
	[
	QString s("mangle_failure");
	#if (QT_VERSION < 400)
	break_me_(\\\);
	#endif
	],
	qt4_cv_libname=$1,
	)
	LIBS="$SAVE_LIBS"
])

dnl check we can do a compile
AC_DEFUN([QT4_CHECK_COMPILE],
[
	AC_MSG_CHECKING([for Qt 4 library name])

	AC_CACHE_VAL(qt4_cv_libname,
	[
		AC_LANG_CPLUSPLUS
		SAVE_CXXFLAGS=$CXXFLAGS
		CXXFLAGS="$CXXFLAGS $QT4_INCLUDES $QT4_LDFLAGS"
		for libname in -lQtCore -lQtCore4 '-framework QtCore'
		do
			QT4_TRY_LINK($libname)
			if test -n "$qt4_cv_libname"; then
				QT4_CORE_LIB="$qt4_cv_libname"
				break;
			fi
		done
		qt4_cv_libname=
		for libname in '-lQtCore -lQtGui' \
		               '-lQtCore4 -lQtGui4' \
		               '-framework QtCore -framework QtConcurrent -framework QtWidgets -framework QtGui'\
		               '-framework QtCore -framework QtGui'
		do
			QT4_TRY_LINK($libname)
			if test -n "$qt4_cv_libname"; then
				break;
			fi
		done
		CXXFLAGS=$SAVE_CXXFLAGS
	])

	if test -z "$qt4_cv_libname"; then
		AC_MSG_RESULT([failed])
		if test "$FATAL" = 1 ; then
			AC_MSG_ERROR([Cannot compile a simple Qt 4 executable. Check you have the right \$QT4DIR !])
		fi
	else
		AC_MSG_RESULT([$qt4_cv_libname])
	fi
])

dnl get Qt version we're using
AC_DEFUN([QT4_GET_VERSION],
[
	AC_CACHE_CHECK([Qt 4 version],lyx_cv_qt4version,
	[
		AC_LANG_CPLUSPLUS
		SAVE_CPPFLAGS=$CPPFLAGS
		CPPFLAGS="$CPPFLAGS $QT4_INCLUDES"

		cat > conftest.$ac_ext <<EOF
#line __oline__ "configure"
#include "confdefs.h"
#include <qglobal.h>
"%%%"QT_VERSION_STR"%%%"
EOF
		lyx_cv_qt4version=`(eval "$ac_cpp conftest.$ac_ext") 2>&5 | \
			grep '^"%%%"'  2>/dev/null | \
			sed -e 's/"%%%"//g' -e 's/"//g'`
		rm -f conftest.$ac_ext
		CPPFLAGS=$SAVE_CPPFLAGS
	])

	QT4_VERSION=$lyx_cv_qt4version
	AC_SUBST(QT4_VERSION)
])

dnl start here
AC_DEFUN([QT4_DO_IT_ALL],
[
	dnl this variable is precious
	AC_ARG_VAR(QT4DIR, [the place where the Qt 4 files are, e.g. /usr/lib/qt4])

	dnl Please leave this alone. I use this file in
	dnl oprofile.
	FATAL=0

	AC_ARG_WITH(qt4-dir, [AC_HELP_STRING([--with-qt4-dir], [where the root of Qt 4 is installed])],
		[ qt4_cv_dir=`eval echo "$withval"/` ])

	AC_ARG_WITH(qt4-includes, [AC_HELP_STRING([--with-qt4-includes], [where the Qt 4 includes are])],
		[ qt4_cv_includes=`eval echo "$withval"` ])

	AC_ARG_WITH(qt4-libraries, [AC_HELP_STRING([--with-qt4-libraries], [where the Qt 4 library is installed])],
		[  qt4_cv_libraries=`eval echo "$withval"` ])

	dnl pay attention to $QT4DIR unless overridden
	if test -z "$qt4_cv_dir"; then
		qt4_cv_dir=$QT4DIR
	fi

	dnl derive inc/lib if needed
	if test -n "$qt4_cv_dir"; then
		if test -z "$qt4_cv_includes"; then
			qt4_cv_includes=$qt4_cv_dir/include
		fi
		if test -z "$qt4_cv_libraries"; then
			qt4_cv_libraries=$qt4_cv_dir/lib
		fi
	fi

	dnl compute the binary dir too
	if test -n "$qt4_cv_dir"; then
		qt4_cv_bin=$qt4_cv_dir/bin
	fi

	dnl Preprocessor flags
	QT4_CPPFLAGS="-DQT_NO_STL -DQT_NO_KEYWORDS"
	case ${host} in
	*mingw*) QT4_CPPFLAGS="-DQT_DLL $QT4_CPPFLAGS";;
	esac
	AC_SUBST(QT4_CPPFLAGS)

	dnl Check if it possible to do a pkg-config
	PKG_PROG_PKG_CONFIG
	if test -n "$PKG_CONFIG" ; then
		QT4_DO_PKG_CONFIG
	fi
	if test "$pkg_failed" != "no" ; then
		QT4_DO_MANUAL_CONFIG
	fi

	dnl Check qt version
	AS_VERSION_COMPARE($QT4_VERSION, $1, 
	[LYX_ERROR([LyX requires version $1 of Qt. Only version $QT4_VERSION has been found.])
	])

	AC_PATH_PROGS(MOC4, [moc-qt4 moc],[],$qt4_cv_bin:$PATH)
	AC_PATH_PROGS(UIC4, [uic-qt4 uic],[],$qt4_cv_bin:$PATH)
	AC_PATH_PROGS(RCC4, [rcc-qt4 rcc],[],$qt4_cv_bin:$PATH)
	AC_CHECK_PROG(bc_found,[bc],[yes],[no],$PATH)
])

AC_DEFUN([QT4_DO_PKG_CONFIG],
[
	dnl tell pkg-config to look also in $qt4_cv_dir/lib.
	save_PKG_CONFIG_PATH=$PKG_CONFIG_PATH
	if test -n "$qt4_cv_dir" ; then
	  PKG_CONFIG_PATH=$qt4_cv_dir/lib:$qt4_cv_dir/lib/pkgconfig:$PKG_CONFIG_PATH
	  export PKG_CONFIG_PATH
	fi
	PKG_CHECK_MODULES(QT4_CORE, QtCore,,[:])
	if test "$pkg_failed" = "no" ; then
		QT4_CORE_INCLUDES=$QT4_CORE_CFLAGS
		AC_SUBST(QT4_CORE_INCLUDES)
		QT4_CORE_LDFLAGS=`$PKG_CONFIG --libs-only-L QtCore`
		AC_SUBST(QT4_CORE_LDFLAGS)
		QT4_CORE_LIB=`$PKG_CONFIG --libs-only-l QtCore`
		AC_SUBST(QT4_CORE_LIB)
	fi
	PKG_CHECK_MODULES(QT4_FRONTEND, QtCore QtGui,,[:])
	if test "$pkg_failed" = "no" ; then
		QT4_INCLUDES=$QT4_FRONTEND_CFLAGS
		dnl QT4_LDFLAGS=$QT4_FRONTEND_LIBS
		QT4_LDFLAGS=`$PKG_CONFIG --libs-only-L QtCore QtGui`
		AC_SUBST(QT4_INCLUDES)
		AC_SUBST(QT4_LDFLAGS)
		QT4_VERSION=`$PKG_CONFIG --modversion QtCore`
		AC_SUBST(QT4_VERSION)
		QT4_LIB=`$PKG_CONFIG --libs-only-l QtCore QtGui`
		AC_SUBST(QT4_LIB)
		LIBS="$LIBS `$PKG_CONFIG --libs-only-other QtCore QtGui`"
	fi
	PKG_CONFIG_PATH=$save_PKG_CONFIG_PATH
])

AC_DEFUN([QT4_DO_MANUAL_CONFIG],
[
	dnl Check for X libraries
	AC_PATH_X
	AC_PATH_XTRA
	case $have_x in
	    yes) LIBS="$X_PRE_LIBS $LIBS $X_LIBS -lX11 $X_EXTRA_LIBS"
	         CPPFLAGS="$CPPFLAGS $X_CFLAGS";;
	     no) LYX_ERROR([Cannot find X window libraries and/or headers.]);;
	disable) ;;
	esac

	dnl flags for compilation
	QT4_INCLUDES=
	QT4_LDFLAGS=
	QT4_CORE_INCLUDES=
	QT4_CORE_LDFLAGS=
	if test -n "$qt4_cv_includes"; then
		QT4_INCLUDES="-I$qt4_cv_includes"
		for i in Qt QtCore QtGui QtWidgets QtConcurrent; do
			QT4_INCLUDES="$QT4_INCLUDES -I$qt4_cv_includes/$i"
		done
		QT4_CORE_INCLUDES="-I$qt4_cv_includes -I$qt4_cv_includes/QtCore"
	fi
	case "$qt4_cv_libraries" in
	*framework*)
		QT4_LDFLAGS="-F$qt4_cv_libraries"
		QT4_CORE_LDFLAGS="-F$qt4_cv_libraries"
		;;
	"")
		;;
	*)
		QT4_LDFLAGS="-L$qt4_cv_libraries"
		QT4_CORE_LDFLAGS="-L$qt4_cv_libraries"
		;;
	esac
	AC_SUBST(QT4_INCLUDES)
	AC_SUBST(QT4_CORE_INCLUDES)
	AC_SUBST(QT4_LDFLAGS)
	AC_SUBST(QT4_CORE_LDFLAGS)

	QT4_CHECK_COMPILE

	QT4_LIB=$qt4_cv_libname;
	AC_SUBST(QT4_LIB)
	AC_SUBST(QT4_CORE_LIB)

	if test -n "$qt4_cv_libname"; then
		QT4_GET_VERSION
	fi
])
