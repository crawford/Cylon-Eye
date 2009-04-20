#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.63)

m4_define(cylon_version_major, 0)
m4_define(cylon_version_minor, 0)
m4_define(cylon_version_micro, 1)

AC_INIT([cylon], cylon_version_major.cylon_version_minor.cylon_version_micro ,[russ@csh.rit.edu])

CYLON_VERSION_MAJOR=cylon_version_major()
CYLON_VERSION_MINOR=cylon_version_minor()
CYLON_VERSION_MICRO=cylon_version_micro()
CYLON_VERSION=${CYLON_VERSION_MAJOR}.${CYLON_VERSION_MINOR}.${CYLON_VERSION_MICRO}
AC_SUBST(CYLON_VERSION_MAJOR)
AC_SUBST(CYLON_VERSION_MINOR)
AC_SUBST(CYLON_VERSION_MICRO)
AC_SUBST(CYLON_VERSION)

AC_CONFIG_SRCDIR([configure.ac])
AC_CONFIG_AUX_DIR([config])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE(AC_PACKAGE_NAME, AC_PACKAGE_VERSION)

AC_CANONICAL_HOST

dnl -----------------------------------
# Static/Shared binaries 

AC_ARG_ENABLE(static, [AC_HELP_STRING([--enable-static], [build static executable])])
if test "x$enable_static" = "xyes"; then
	AC_ENABLE_STATIC
	AC_DEFINE(ENABLE_STATIC,1,[Define if you want statically linked executables])
else
	AC_DISABLE_STATIC
fi
AM_CONDITIONAL(ENABLE_STATIC, [test "x$enable_static" = "xyes"])

AC_ARG_ENABLE(shared, [AC_HELP_STRING([--enable-shared], [build shared executable])])
if test "x$enable_shared" != "xno"; then
	AC_ENABLE_SHARED
	AC_DEFINE(ENABLE_SHARED,1,[Define if you want shared lib executables])
else
	AC_DISABLE_SHARED
fi
AM_CONDITIONAL(ENABLE_SHARED, [test "x$enable_shared" = "xyes"])

CFLAGS+=" -Wall "
AM_PATH_PYTHON

AC_C_CONST

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_LIBTOOL
AC_PROG_LN_S

dnl -----------------------------------
dnl Enable swig

AC_ARG_ENABLE(swig, [AC_HELP_STRING([--enable-swig=yes], [swig-generated language bindings])], [], [enable_swig=yes])

if test "x$enable_swig" != "xyes"; then
	use_swig="No (disabled)"
else
	AC_PROG_SWIG
	use_swig="Yes"
fi
AM_CONDITIONAL(WITH_SWIG, [test "x$SWIG" != "x"])

dnl -----------------------------------
dnl INCLUDES and LIBS for C#

dnl AC_ARG_ENABLE(sharp, [AC_HELP_STRING([--enable-sharp=yes], [C# language bindings])], [], [enable_sharp=yes])

#if test "x$enable_sharp" != "xyes"; then
#  use_sharp="No (disabled)"
#else
#  if test "x$use_swig" != "xYes"; then
#    use_sharp="No (swig not available)"
#  else
#    if test `$SWIG -help 2>&1 | $GREP -c '\-csharp *- Generate'` = 0; then
#      use_sharp="No (swig does not support -csharp option)"
#    else
#      AC_CHECK_PROG(MCS,mcs,mcs)
#      if test "x$MCS" = "x"; then
#        use_sharp="No (mcs not available)"
#      else
#        use_sharp="Yes"
#        SHARP_INCLUDES=
#        SHARP_LIBS=
dnl        AC_SUBST(SHARP_INCLUDES)
dnl        AC_SUBST(SHARP_LIBS)
#      fi
#    fi
#  fi
#fi
dnl AM_CONDITIONAL(WITH_SHARP, [test "x$use_sharp" = "xYes"])

dnl -----------------------------------
dnl INCLUDES and LIBS for PYTHON

AC_ARG_ENABLE(python,
  [AC_HELP_STRING([--enable-python=yes], [python language bindings])],
  [], [enable_python=yes])

if test "x$enable_python" != "xyes"; then
	use_python="No (disabled)"
else
	if test "x$use_swig" != "xYes"; then
		use_python="No (swig not available)"
	else
		if test `$SWIG -help 2>&1 | $GREP -c '\-python *- Generate'` = 0; then
			use_python="No (swig does not support -python option)"
		else
			use_python="Yes"
			SWIG_PYTHON
		fi
	fi
fi
AM_CONDITIONAL(WITH_PYTHON, [test "x$use_python" = "xYes"])

dnl -----------------------------------
dnl INCLUDES and LIBS for JAVA

AC_ARG_ENABLE(java,
  [AC_HELP_STRING([--enable-java=yes], [java language bindings])],
  [], [enable_java=yes])

if test "x$enable_java" != "xyes"; then
  use_java="No (disabled)"
else
  if test "x$use_swig" != "xYes"; then
    use_java="No (swig not available)"
  else
    if test `$SWIG -help 2>&1 | $GREP -c '\-java *- Generate'` = 0; then
      use_java="No (swig does not support -java option)"
    else
      AC_CHECK_PROG(JAVA,java,java)
      if test "x$JAVA" = "x"; then
        use_java="No (java not available)"
      else
	case "${host_os}" in
	  *darwin* )
            JAVA_INCLUDES="-I/System/Library/Frameworks/JavaVM.framework/Headers"
	    JAVA_LIBS=
	    ;;
	  *)
            JAVA_INCLUDES=
	    JAVA_LIBS=
	    ;;
        esac
	save_CPPFLAGS=$CPPFLAGS
        CPPFLAGS="$CPPFLAGS $JAVA_INCLUDES"
        AC_CHECK_HEADER(jni.h,,[
            use_java="No (missing header)"
  	    JAVA=
        ])
        CPPFLAGS="$save_CPPFLAGS"
        if test "x$JAVA" != "x"; then
          use_java="Yes"
	  AC_SUBST(JAVA_INCLUDES)
          AC_SUBST(JAVA_LIBS)
        fi
      fi
    fi
  fi
fi
AM_CONDITIONAL(WITH_JAVA, [test "x$use_java" = "xYes"])

# Checks for header files.
AC_CHECK_HEADERS([stdint.h stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_CHECK_FUNCS([strerror asprintf])

AC_CONFIG_FILES([Makefile \
		src/Makefile \
		swig/Makefile \
		cylon.pc])
AC_OUTPUT

echo ""
echo "----------------------------------------------------------------"
echo ""
echo "cylon-$VERSION will be compiled with the following:"
echo ""
echo "language extensions:"
echo "  cyl_sharp:      $use_sharp"
echo "  cyl_guile:      $use_guile"
echo "  cyl_io:         $use_io"
echo "  cyl_java:       $use_java"
echo "  cyl_lua:        $use_lua"
echo "  cyl_ocaml:      $use_ocaml"
echo "  cyl_perl:       $use_perl"
echo "  cyl_php:        $use_php"
echo "  cyl_python:     $use_python"
echo "  cyl_R:          $use_r"
echo "  cyl_ruby:       $use_ruby"
echo "  cyl_tcl:        $use_cyl_tcl"
echo ""
