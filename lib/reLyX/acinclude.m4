dnl Usage: RELYX_WARNING(message)  Displays the warning "message" and sets
dnl the flag lyx_warning to yes.
AC_DEFUN([RELYX_WARNING],[
relyx_warning_txt="$relyx_warning_txt
== $1
"
relyx_warning=yes])


dnl Usage: RELYX_ERROR(message)  Displays the error "message" and sets
dnl the flag lyx_error to yes.
AC_DEFUN([RELYX_ERROR],[
relyx_error_txt="$relyx_error_txt
== $1
"
relyx_error=yes])


dnl RELYX_SEARCH_PROG(VARIABLE-NAME,PROGRAMS-LIST,ACTION-IF-FOUND)
dnl
define([RELYX_SEARCH_PROG],[dnl
case "`uname -s 2> /dev/null`" in
OS/2)
  PATH=`echo -E "$PATH" | sed 's+\\\\+/+g'`
  PATH_IFS=';'
  ;;
*)
  PATH_IFS=':'
  ;;
esac
for ac_prog in $2 ; do
# Extract the first word of "$ac_prog", so it can be a program name with
# args.
  set dummy $ac_prog ; ac_word=$[2]
  if test ! -n "[$]$1"; then
    IFS="${IFS=         }"; ac_save_ifs="$IFS"; IFS="${IFS}$PATH_IFS"
    for ac_dir in $PATH; do
      test -z "$ac_dir" && ac_dir=.
      if test -f [$ac_dir/$ac_word] -o -f [$ac_dir/$ac_word$ac_exeext]; then
        $1="$ac_prog"
        break
      fi
    done
    IFS="$ac_save_ifs"
  fi

  if test -n "[$]$1"; then
    ac_result=yes
  else
    ac_result=no
  fi
  ifelse($3,,,[$3])
  test -n "[$]$1" && break
done
])


dnl Usage RELYX_PROG_PERL_OK
AC_DEFUN([RELYX_PROG_PERL_OK],[
if echo 'require 5.002;exit' | $ac_dir/$ac_prog 2>&5
then
  PERL=$ac_dir/$ac_prog
else
  PERL=
fi])

dnl Usage RELYX_CHECK_PERL
AC_DEFUN([RELYX_CHECK_PERL],[
AC_MSG_CHECKING([for perl >= 5.002])
RELYX_SEARCH_PROG(PERL, perl perl5 perl5.6.1  perl5.6.0 perl5.005 perl5.004 perl5.003 perl5.002,
RELYX_PROG_PERL_OK
)
if test -n "$PERL" ; then
  AC_MSG_RESULT($PERL)
else
  PERL="/bin/echo reLyX has been disabled because perl version 5.002 has
not \
been found.\nTo reenable it, edit "
  AC_MSG_RESULT(no)
  RELYX_WARNING(dnl
Configure has not been able to find a version 5.002 or better of Perl.
   reLyX has been setup to only display a warning message.
   Set variable PERL to some value to install a working reLyX.)
fi
AC_SUBST(PERL)])

dnl Usage: RELYX_CHECK_ERRORS  Displays a warning message if a RELYX_ERROR
dnl   has occured previously.
AC_DEFUN([RELYX_CHECK_ERRORS],[
if test x$relyx_error = xyes; then
cat <<EOF
**** The following problems have been detected by configure.
**** Please check the messages below before running 'make'.
**** (see the section 'Problems' in the INSTALL file)
$relyx_error_txt
$relyx_warning_txt
deleting cache $cache_file
EOF
  rm -f $cache_file
else

if test x$relyx_warning = xyes; then
cat <<EOF
=== The following minor problems have been detected by configure.
=== Please check the messages below before running 'make'.
=== (see the section 'Problems' in the INSTALL file)
$relyx_warning_txt
EOF
fi
fi])


dnl Check what kind of packaging should be used at install time. 
dnl The default is autodetected. 
AC_DEFUN([RELYX_USE_PACKAGING],
[AC_MSG_CHECKING([what packaging should be used])
AC_ARG_WITH(packaging,
  [  --with-packaging=THIS   Use THIS packaging for installation:
			    Possible values: posix, windows, macosx],
  [lyx_use_packaging="$withval"], [
  case $host in
    *-apple-darwin*) lyx_use_packaging=macosx ;;
     *-pc-mingw32*) lyx_use_packaging=windows;;
                  *) lyx_use_packaging=posix;;
  esac])
AC_MSG_RESULT($lyx_use_packaging)
case $lyx_use_packaging in 
   macosx) default_prefix="/Applications/LyX.app"
	   bindir='${prefix}/Contents/MacOS'
	   libdir='${prefix}/Contents/Resources'
	   datadir='${prefix}/Contents/Resources'
	   mandir='${prefix}/Contents/Resources/man' ;;
  windows) default_prefix="C:/Program Files/LyX"
	   bindir='${prefix}/bin'
	   libdir='${prefix}/Resources'
	   datadir='${prefix}/Resources'
	   mandir='${prefix}/Resources/man' ;;
    posix) default_prefix=$ac_default_prefix ;;
    *) RELYX_ERROR([Unknown packaging type $lyx_use_packaging]) ;;
esac
])
