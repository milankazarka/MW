/*
 *    Copyright (C) by Milan Kazarka
 *
 *    You may distribute under the terms of the Artistic License,
 *    as specified in the Artistic.txt file
 *
 */

#ifndef _LABEL_H_
#define _LABEL_H_

#include "widget.h"
#include "text.h"

class milanLabel : public milanWidget {
  public:
    milanText *myText;
  
    milanLabel( int x, int y, int w, int h );
    ~milanLabel( );
    
    int setText( milanText *text );
};

#endif
