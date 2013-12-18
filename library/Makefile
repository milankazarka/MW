CC = gcc
CCO = g++

CFLAGS = -c -DUNIX -DGUI -DUSEX -DJPEG \
	-g -O2 \
	-DUSESSL -I/ssl/include/openssl -Lssl/lib \
	-Ifreetype/include \
	-I/usr/include \
	-I/usr/include/libpng12 \
	-I/opt/mika/include -L/opt/mika/lib \
	-ImikaGl/libCairo/src
	
LDFLAGS = -pthread \
	-lpthread -lX11 -lXext \
	-lXrender -DGUI -DUSEX -DUNIX -DJPEG -lXpm \
	-DUSESSL -ltcl8.5 -ltk8.5 \
	-lz -lm -lcrypto -lcurl -lnxml

desktop :
	$(CCO) $(LDFLAGS) \
	desktop.o widget.o canvas.o object.o window.o \
	image.o text.o microDebug.o label.o main.o \
	mikaGl/mikaGl.a -o desktop

microDebug.o : microDebug.o
	$(CCO) $(CFLAGS) -c microDebug.c

desktop.o : desktop.c
	$(CCO) $(CFLAGS) -c desktop.c

canvas.o : canvas.c
	$(CCO) $(CFLAGS) -c canvas.c

widget.o : widget.c
	$(CCO) $(CFLAGS) -c widget.c

text.o : text.c
	$(CCO) $(CFLAGS) -c text.c

image.o : image.c
	$(CCO) $(CFLAGS) -c image.c

object.o : object.c
	$(CCO) $(CFLAGS) -c object.c

window.o : window.c
	$(CCO) $(CFLAGS) -c window.c

bridge.o : bridge.c
	$(CCO) $(CFLAGS) -c bridge.c
	
label.o : label.c
	$(CCO) $(CFLAGS) -c label.c

main.o : main.c
	$(CCO) $(CFLAGS) -c main.c

	