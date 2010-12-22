CySim
=====

This is a full simulator for the cylon eye.

Command Processor
-----------------

This is the program that actually processes the commands sent to the display. A socket is opened and it listens for commands. Whenever the display changes, it outputs the binary data to stdout.

GUI
---

This is a wrapper around the command processor that provides a nice interface. It requires the command processor to be built in addition to a config file. The config file specifies the physical positions of the boards as well as their addresses.


Building
--------

This isn't integrated in the CMake build system yet, so you'll have to do it manually.

###Command Processor

Run javac on each of the .java files except for TestCmdProc.java

###GUI

Run qmake (this will generate a Makefile), run make, move the .class files from the Command Processor into the same directory as the executable.
