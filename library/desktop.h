/*
 *    Copyright (C) by Milan Kazarka
 *
 *    You may distribute under the terms of the Artistic License,
 *    as specified in the Artistic.txt file
 *
 */

#ifndef _DESKTOP_H_
#define _DESKTOP_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "mikaGl/mikaGlWindow.h"

class milanDesktop {
  public:
    mikaGlWindow *glWindow;
  
    milanDesktop( );
    ~milanDesktop( );
};

#endif
