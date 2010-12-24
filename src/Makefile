SRC_SOURCES := cylon.c

SRC_SOURCES := $(SRC_SOURCES:%=$(CURDIR)/%)
SRC_DEPFILES := $(SRC_SOURCES:.c=.d)
SRC_OBJFILES := $(SRC_SOURCES:.c=.o)

CYLON_LIBRARY := libcylon.so
CYLON_LIBRARY := $(CYLON_LIBRARY:%=$(CURDIR)/%)

.PHONY: src/compile src/clean src/dependencies

src/dependencies: $(SRC_DEPFILES)
src/compile: $(CYLON_LIBRARY)
src/clean:
	$(RM) $(SRC_DEPFILES) $(SRC_OBJFILES) $(CYLON_LIBRARY)

$(CYLON_LIBRARY): $(SRC_OBJFILES)
$(CYLON_LIBRARY): SO_OBJECTS := $(SRC_OBJFILES)

# vim:tw=80
