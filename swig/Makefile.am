AM_CPPFLAGS = \
	-I$(top_srcdir)/src

SWIG_SOURCES = cylon.i

PYTHON_data=cylon.py
pkgpythondir = $(pkglibdir)/python
libcylon_python_la_SOURCES = cylon_python.c $(SWIG_SOURCES)
libcylon_python_la_LDFLAGS = -avoid-version -module
libcylon_python_la_LIBADD = $(top_builddir)/src/libcylon.la
libcylon_python_la_CPPFLAGS = $(AM_CPPFLAGS) $(SWIG_PYTHON_CPPFLAGS)
cylon_python.c: $(SWIG_SOURCES)
	$(SWIG) $(SWIG_PYTHON_OPT) -I$(top_srcdir)/src -o $@ $<

if WITH_PYTHON
pkgpython_DATA = $(PYTHON_data)
pkgpython_LTLIBRARIES = libcylon_python.la
endif

install-data-hook:
if WITH_PYTHON
	(cd $(DESTDIR)$(pkgpythondir); $(RM) -f _cylon.so; $(LN_S) libcylon_python.so _cylon.so;)
endif

uninstall-hook:
	$(RM) -rf $(DESTDIR)$(pkgpythondir);

CLEANFILES = cylon_python.c $(PYTHON_data)
