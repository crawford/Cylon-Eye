CySim
=====

This is a full simulator for the cylon eye.

Command Processor
-----------------

This is the program that actually processes the commands sent to the display. A socket is opened and it listens for commands. Whenever the display changes, it outputs the binary data to stdout.

GUI
---

This is a wrapper around the command processor that provides a nice interface. It requires the command processor to be built in addition to a config file. The config file specifies the physical positions of the boards as well as their addresses.
