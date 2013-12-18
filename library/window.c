#include "window.h"
#include "config.h"
#include "canvas.h"
#include "image.h"
#include "widget.h"
#include "text.h"
#include "microDebug.h"

milanWindow *windows = NULL;

milanWindow::milanWindow( int w, int h )
{
  mw = w;
  mh = h;
  glWindow = new mikaGlWindow(mw,mh);
  glWindowDelegate.onButtonDown = onButtonDownShared;
  glWindowDelegate.onButtonRelease = onButtonUpShared;
  glWindowDelegate.onButtonMotion = onButtonMotionShared;
  glWindow->delegate = &glWindowDelegate;
  next = windows;
  windows = this;
  currentCanvas = NULL;
}

milanWindow::~milanWindow( )
{
}

int milanWindow::eventLoop( )
{
  glWindow->eventLoop();
  glWindow->wait();
  return 0;
}

int milanWindow::triggerExpose( )
{
  #ifdef _DEBUG
    printf("milanWindow::triggerExpose\n");
  #endif
  if (currentCanvas)
  {
    milanCanvas *canvas = (milanCanvas*)currentCanvas;
    canvas->triggerExpose();
  }
  glWindow->triggerExpose();
  return 0;
}

/** in case we're drawing externally, just blit
 *
 */
int milanWindow::triggerExposeSimple( )
{
  glWindow->triggerExpose();
  return 0;
}

/** todo - refactoring. Instead set the parent of the delegate to this object.
 *  Will give us more flexibility.
 */
void *onButtonDownShared( void *windowData, struct eventButton *event )
{
  if (!windowData || !event)
    return NULL;
  mikaGlWindow *glWindow = (mikaGlWindow*)windowData;
  milanWindow *current = windows;
  while(current)
  {
    if (current->glWindow==glWindow)
    {
      current->onButtonDown(event);
      break;
    }
    current = current->next;
  }
  return NULL;
}

void *onButtonUpShared( void *windowData, struct eventButton *event )
{
  if (!windowData || !event)
    return NULL;
  mikaGlWindow *glWindow = (mikaGlWindow*)windowData;
  milanWindow *current = windows;
  while(current)
  {
    if (current->glWindow==glWindow)
    {
      current->onButtonUp(event);
      break;
    }
    current = current->next;
  }
  return NULL;
}

void *onButtonMotionShared( void *windowData, struct eventButton *event )
{
  if (!windowData || !event)
    return NULL;
  mikaGlWindow *glWindow = (mikaGlWindow*)windowData;
  milanWindow *current = windows;
  while(current)
  {
    if (current->glWindow==glWindow)
    {
      current->onButtonMotion(event);
      break;
    }
    current = current->next;
  }
  return NULL;
}

int milanWindow::onButtonDown( struct eventButton *event )
{
  if (!event)
  {
    #ifdef _DEBUG
      printf("milanWindow::onButtonDown error, invalid argument\n");
    #endif
    return 1;
  }
  #ifdef _DEBUG
    printf("milanWindow::onButtonDown\n");
  #endif
  
  onButtonDownEvent(event);
  
  if (currentCanvas)
  {
    milanCanvas *c = (milanCanvas*)currentCanvas;
      c->onButtonDown(event);
  }
  return 0;
}

int milanWindow::onButtonUp( struct eventButton *event )
{
  if (!event)
  {
    #ifdef _DEBUG
      printf("milanWindow::onButtonUp error, invalid argument\n");
    #endif
    return 1;
  }
  #ifdef _DEBUG
    printf("milanWindow::onButtonUp\n");
  #endif
  
  onButtonUpEvent(event);
  
  if (currentCanvas)
  {
    milanCanvas *c = (milanCanvas*)currentCanvas;
      c->onButtonUp(event);
  }
  return 0;
}

int milanWindow::onButtonMotion( struct eventButton *event )
{
  if (!event)
    return 1;
  
  onButtonMotionEvent(event);
  
  return 0;
}

void milanWindow::onButtonDownEvent( struct eventButton *event )
{
  mprintf("milanWindow::onButtonDownEvent\n");
}

void milanWindow::onButtonUpEvent( struct eventButton *event )
{
  mprintf("milanWindow::onButtonUpEvent\n");
}

void milanWindow::onButtonMotionEvent( struct eventButton *event )
{
  mprintf("milanWindow::onButtonMotionEvent\n");
}

int milanWindow::addCanvas( object *nc )
{
  if (!nc)
  {
    return 1;
  }
  milanCanvas *c = (milanCanvas*)nc;
  c->next = (milanCanvas*)canvasses;
  canvasses = nc;
  return 0;
}

int milanWindow::pushCanvas( object *c )
{
  if (!c)
  {
    #ifdef _DEBUG
      printf("milanWindow::pushCanvas error, invalid argument\n");
    #endif
    return 1;
  }
  #ifdef _DEBUG
    printf("milanWindow::pushCanvas\n");
  #endif
  currentCanvas = c;
  milanCanvas *canvas = (milanCanvas*)c;
  
  return 0;
}

/** direct drawing
 */

int milanWindow::ddrawImage( object *image, int x, int y )
{
  if (!image)
    return 1;
  
  milanImage *mimage = (milanImage*)image;
  mikaGl *gl = glWindow->gl;
  gl->drawImage(x,y,mimage->glImage);
  
  return 0;
}

int milanWindow::ddrawWidget( object *widget, int x, int y )
{
  if (!widget)
    return 1;
  milanWidget *mwidget = (milanWidget*)widget;
  return 0;
}

int milanWindow::ddrawRoundRect( int x, int y, int w, int h, unsigned char *color )
{
  if (!color)
    return 1;
  
  mikaGl *gl = glWindow->gl;  
  gl->setColor(color[0],color[1],color[2],color[3]);
  
  return 0;
}

int milanWindow::ddrawText( object *text, int x, int y )
{
  if (!text)
    return 1;
  
  milanText *mtext = (milanText*)text;
  mikaGl *gl = glWindow->gl;
  gl->drawText(x,y,(unsigned char*)mtext->myStr);
  
  return 0;
}
