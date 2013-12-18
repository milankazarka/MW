#ifndef _WINDOW_H_
#define _WINDOW_H_

#include "window.h"
#include "object.h"
#include "mikaGl/mikaGlWindow.h"

class milanWindow : public object {
  public:
    int 		mw;
    int 		mh;
    mikaGlWindow 	*glWindow;
    struct mikaGlWindowDelegate glWindowDelegate;
    milanWindow 	*next;
    object 		*canvasses;
    object 		*currentCanvas;
  
    milanWindow( int w, int h );
    ~milanWindow( );
    
    int triggerExpose( );
    int triggerExposeSimple( );		// in case we're drawing externally, only blit
    int onButtonDown( struct eventButton *event );
    int onButtonUp( struct eventButton *event );
    int onButtonMotion( struct eventButton *event );
    int eventLoop( );
    
    int addCanvas( object *nc );
    int pushCanvas( object *c );
    
    /** direct drawing of object to the Window. This is useful when we are
     *  using an external widget set and only use this framework for drawing.
     */

     int ddrawImage( object *image, int x, int y );
     int ddrawRoundRect( int x, int y, int w, int h, unsigned char *color );
     int ddrawWidget( object *widget, int x, int y );
     int ddrawText( object *text, int x, int y );
     
     virtual void onButtonDownEvent( struct eventButton *event );
     virtual void onButtonUpEvent( struct eventButton *event );
     virtual void onButtonMotionEvent( struct eventButton *event );
};

void *onButtonDownShared( void *windowData, struct eventButton *event );
void *onButtonUpShared( void *windowData, struct eventButton *event );
void *onButtonMotionShared( void *windowData, struct eventButton *event );

extern milanWindow *windows;

#endif
