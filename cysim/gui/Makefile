QMAKE ?= qmake

QMAKEFLAGS += -Wall

QMAKE_MAKEFILE := $(CURDIR)/CylonEyeSim.qmk

QMAKE_MAKEFILE_EXES := $(CURDIR)/CylonEyeSim

.PHONY: cysim/gui/compile cysim/gui/clean cysim/gui/dependencies $(QMAKE_MAKEFILE)

cysim/gui/compile: $(QMAKE_MAKEFILE)
	$(MAKE) -C $(CURDIR) -f $(QMAKE_MAKEFILE)

cysim/gui/clean:
	if [[ -e $(QMAKE_MAKEFILE) ]]; then \
		$(MAKE) -C $(CURDIR) -f $(QMAKE_MAKEFILE) distclean; \
	fi

cysim/gui/dependencies:

$(QMAKE_MAKEFILE): %.qmk: %.pro
	$(QMAKE) $(QMAKEFLAGS) -o $@ QMAKE_CXX="$(CXX)" QMAKE_CXXFLAGS="$(CXXFLAGS)" $<
