AUTOMAKE_OPTIONS = foreign
ACLOCAL_AMFLAGS = -I m4

SUBDIRS = src swig

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = cylon.pc

install-data-hook:
if WITH_PYTHON
	-mkdir -p $(DESTDIR)@PYTHON_SITE_PKG@;
	if test -w $(DESTDIR)@PYTHON_SITE_PKG@; then \
		(cd $(DESTDIR)@PYTHON_SITE_PKG@; \
			$(LN_S) -f $(pkgpythondir)/libcylon_python.so _cylon.so; \
			$(LN_S) -f $(pkgpythondir)/cylon.py cylon.py;) \
	else \
		echo "Warning: @PYTHON_SITE_PKG@ is not writable."; \
		echo "Skipping system installation of python binding."; \
	fi
endif
