#!/bin/bash
rm desktop
rm *.o
make microDebug.o
make canvas.o
make text.o
make image.o
make widget.o
make window.o
make object.o
make bridge.o
make desktop.o
make label.o
make main.o
make desktop
rm library.a
ar rvs library.a \
	mikaGl/mikaGl.a \
	microDebug.o \
	canvas.o \
	text.o \
	image.o \
	widget.o \
	window.o \
	object.o \
	bridge.o \
	desktop.o \
	label.o
