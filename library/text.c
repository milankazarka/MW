#include "text.h"
#include "config.h"
#include "microDebug.h"

milanText::milanText( unsigned char *str )
{
  #ifdef _DEBUG
    printf("milanText::milanText\n");
  #endif
  if (!str)
  {
  }
  else
  {
    myStr = (unsigned char*)malloc(strlen((char*)str)+1);
    strcpy((char*)myStr,(char*)str);
  }
}

milanText::~milanText( )
{
  if (myStr)
    free(myStr);
}
