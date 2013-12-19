/*
 *    Copyright (C) by Milan Kazarka
 *
 *    You may distribute under the terms of the Artistic License,
 *    as specified in the Artistic.txt file
 *
 */

#ifndef _MIKA_GL_IMAGE_H_
#define _MIKA_GL_IMAGE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libJpeg/jpeglib.h"
#include "libJpeg/jpegint.h"
#include "libJpeg/jerror.h"
#include "libJpeg/jconfig.h"
#include "cairo.h"
#include "cairo-xlib.h"

class mikaGlImage {
  public:
    int mw;
    int mh;
    unsigned char 	*pixels;
    cairo_surface_t 	*csurface; // cairo surface
  
    mikaGlImage( unsigned char *filename );
    ~mikaGlImage( );
    
    int resize( int neww, int newh );
};

#endif
