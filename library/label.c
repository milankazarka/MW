#include "label.h"
#include "microDebug.h"

milanLabel::milanLabel( int x, int y, int w, int h )
 :  milanWidget(x,y,w,h)
{
  
}

milanLabel::~milanLabel( )
{
}

int milanLabel::setText( milanText *text )
{
 if (!text)
 {
  mprintf("milanLabel::setText error, invalid text\n");
  return 1;
 }
 
 myText = text;
 
 return 0;
}

