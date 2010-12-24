CYSIM_CMDPROC_SOURCES := CmdProc.java Frame.java Panel.java Zigbee.java

CYSIM_CMDPROC_SOURCES := $(CYSIM_CMDPROC_SOURCES:%=$(CURDIR)/%)
CYSIM_CMDPROC_OBJFILES := $(CYSIM_CMDPROC_SOURCES:.java=.o)

CYSIM_CMDPROC_EXES := $(CURDIR)/CmdProc

.PHONY: cysim/cmdproc/compile cysim/cmdproc/clean cysim/cmdproc/dependencies

cysim/cmdproc/dependencies:
cysim/cmdproc/compile: $(CYSIM_CMDPROC_EXES)
cysim/cmdproc/clean:
	$(RM) $(CYSIM_CMDPROC_OBJFILES) $(CYSIM_CMDPROC_EXES)

$(CYSIM_CMDPROC_EXES): $(CYSIM_CMDPROC_OBJFILES)
	$(GCJ) $(JAVAFLAGS) --main=CmdProc -o $@ $^

$(CYSIM_CMDPROC_OBJFILES): %.o: %.java
	$(GCJ) -c $(JAVAFLAGS) -o $@ $<

# vim:tw=80
