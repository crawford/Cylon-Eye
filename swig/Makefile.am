AM_CPPFLAGS = \
	-I$(top_srcdir)/src

LIBS = @LIBS@ -lc

BASESOURCES = cylon.i
BASELIBS = $(top_builddir)/src/libcylon.la

pkgpythondir = $(pkglibdir)/python
PYTHON_data=cylon.py
nodist_libcylon_python_la_SOURCES = cylon_python.c $(PYTHON_data)
libcylon_python_la_SOURCES = $(BASESOURCES)
libcylon_python_la_LDFLAGS = -avoid-version -module
libcylon_python_la_LIBADD = $(BASELIBS) $(PYTHON_LIBS)
libcylon_python_la_CPPFLAGS = $(AM_CPPFLAGS) $(SWIG_PYTHON_CPPFLAGS)
$(PYTHON_data): cylon_python.c
cylon_python.c: cylon.i
	$(SWIG) $(SWIG_PYTHON_OPT) -I$(top_srcdir)/src -o $@ $<

pkgjavadir = $(pkglibdir)/java
JAVA_data=SWIGTYPE_p_int_fast64_t.java SWIGTYPE_p_int_fast16_t.java SWIGTYPE_p_int_fast8_t.java cylon.java cylonConstants.java cylonJNI.java frame_t.java panel_t.java
nodist_libcylon_java_la_SOURCES = cylon_java.c
libcylon_java_la_SOURCES = $(BASESOURCES)
libcylon_java_la_LDFLAGS = -avoid-version -module
libcylon_java_la_LIBADD = $(BASELIBS) $(JAVA_LIBS)
libcylon_java_la_CPPFLAGS = $(AM_CPPFLAGS) $(JAVA_INCLUDES)
$(JAVA_data): cylon_java.c
cylon_java.c: cylon.i
	$(SWIG) -java -I$(top_srcdir)/src -o $@ $<

if WITH_PYTHON
pkgpython_DATA = $(PYTHON_data)
pkgpython_LTLIBRARIES = libcylon_python.la
endif
if WITH_JAVA
pkgjava_DATA = $(JAVA_data)
pkgjava_LTLIBRARIES = libcylon_java.la
endif

install-data-hook:
if WITH_PYTHON
	(cd $(DESTDIR)$(pkgpythondir); $(RM) -f _cylon.so; $(LN_S) libcylon_python.so _cylon.so;)
endif

uninstall-hook:
	$(RM) -rf $(DESTDIR)$(pkgpythondir);

CLEANFILES = \
	cylon_python.c $(PYTHON_data) \
	cylon_java.c $(JAVA_data)
