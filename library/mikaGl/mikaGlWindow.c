#include "mikaGlWindow.h"
#include "mikaGlConfig.h"
#include "mikaGlImage.h"

struct lowWindow *lowWindowInit( int w, int h )
{
  #ifdef _GLDEBUG
    printf("lowWindowInit\n");
  #endif
  struct lowWindow *window = NULL;
  window = (struct lowWindow*)malloc(sizeof(struct lowWindow));
  
  XInitThreads();
  window->dpy = XOpenDisplay(NULL);
  window->screen = DefaultScreen(window->dpy);
  Window root;
  unsigned long valuemask;
  XWindowAttributes attr1;
  XSetWindowAttributes attr;
  attr.event_mask = ExposureMask | ButtonPressMask | ButtonReleaseMask |
      ButtonMotionMask | EnterWindowMask | LeaveWindowMask;
  root = RootWindow(window->dpy,window->screen);
  XGetWindowAttributes(window->dpy,root,&attr1);
  valuemask = CWEventMask;
  
  #ifdef _OVERRIDE_REDIRECT
    valuemask += CWOverrideRedirect;
    attr.override_redirect = True;
  #endif
  
  window->id = XCreateWindow(
      window->dpy,
      root,
      0,0,
      w, h,
      1, DefaultDepth(window->dpy,window->screen),
      CopyFromParent,
      DefaultVisual(window->dpy,window->screen),
      valuemask, &attr
    );

  window->surface = (unsigned char*)malloc(w*h*4);

  int count = 0;
  XPixmapFormatValues *formats = NULL;
  XPixmapFormatValues *format = NULL;
  formats = XListPixmapFormats(window->dpy,&count);
  
  for(format = formats; count > 0; count--, format++)
  {
    if (format->depth == DefaultDepth(window->dpy,window->screen))
    {
      window->ximage = XCreateImage(
        window->dpy,
        DefaultVisual(window->dpy,window->screen),
        DefaultDepth(window->dpy,window->screen),
        ZPixmap,
        0,
        (char*)window->surface,
        w,h,
        format->scanline_pad,
        0
      );
    }
  }
    
  XSetWindowBackground(window->dpy,window->id,
      BlackPixel(window->dpy,window->screen));
  XSelectInput(window->dpy,window->id,
      ExposureMask|ButtonPressMask|ButtonReleaseMask|ButtonMotionMask);
  XMapWindow(window->dpy,window->id);
  
  return window;
}

mikaGlWindow::mikaGlWindow( int w, int h )
{
  #ifdef _GLDEBUG
    printf("mikaGlWindow::mikaGlWindow\n");
  #endif
  mw = w;
  mh = h;
  surface = new mikaGlSurface(mw,mh);
  window = lowWindowInit(mw,mh);
  gl = new mikaGl((void*)surface->pixels,mw,mh);
  delegate = NULL;
  #ifdef _GLTEST
    gl->setColor(67,67,67,190);
    gl->drawRoundRect(0,0,100,100,2);
    gl->drawRoundRect(80,80,180,180,2);
    gl->drawText(100,100,(unsigned char*)"hello world");
    mikaGlImage *testImage = NULL;
    try 
    {
      testImage = new mikaGlImage((unsigned char*)"image.jpg");
      testImage->resize(30,30);
    }
    catch (char const *e)
    {
      printf("mikaGlWindow::mikaGlWindow warning, couldn't load image e(%s)\n",e);
    }
    if (testImage)
    {
      gl->drawImage(0,0,testImage);
    }
  #endif
}

int mikaGlWindow::eventExpose( )
{
  static XImage *image = NULL;
  static cairo_t 		*cairo = NULL;
  static cairo_surface_t 	*csurface = NULL;
  static cairo_surface_t	*source = NULL;
  
  #ifdef _GLDEBUG
    printf("mikaGlWindow::eventExpose\n");
  #endif
  
  XGrabServer(window->dpy);
  /**
  csurface = cairo_xlib_surface_create(
                              window->dpy,
                              window->id,
                              DefaultVisual(window->dpy,window->screen),
                              mw,
                              mh
                              );
  */
  
  csurface = cairo_image_surface_create_for_data(
                              window->surface,
                              CAIRO_FORMAT_ARGB32,
                              mw,
                              mh,
                              4*mw
      );
  
  source = cairo_image_surface_create_for_data(
                              surface->pixels,
                              CAIRO_FORMAT_ARGB32,
                              mw,
                              mh,
                              4*mw
      );
  printf("\tcheck 01\n");
  
  cairo = cairo_create(csurface);
  cairo_set_source_surface(cairo, source, 0, 0);
  cairo_paint(cairo);
  
  cairo_destroy(cairo);
  cairo_surface_destroy(csurface);
  cairo_surface_destroy(source);
  
  printf("\tcheck 02\n");
  
  XPutImage(
    window->dpy,
    window->id,
    XDefaultGC(
      window->dpy,
      window->screen
      ),
    window->ximage,
    0,0,0,0,
    mw,mh
  );
  
  XUngrabServer(window->dpy);
  XFlush(window->dpy);
  XSync(window->dpy,0);
  
  return 0;
}

int mikaGlWindow::eventButtonDown( int x, int y, int id )
{
  #ifdef _GLDEBUG
    printf("mikaGlWindow::eventButtonDown\n");
  #endif
  struct eventButton ev;
  if (delegate)
  {
    if (delegate->onButtonDown)
    {
      ev.x = x;
      ev.y = y;
      ev.id = id;
      delegate->onButtonDown((void*)this,&ev);
    }
  }  
  return 0;
}

int mikaGlWindow::eventButtonRelease( int x, int y, int id )
{
  #ifdef _GLDEBUG
    printf("mikaGlWindow::eventButtonRelease\n");
  #endif
  struct eventButton ev;
  if (delegate)
  {
    if (delegate->onButtonRelease)
    {
      ev.x = x;
      ev.y = y;
      ev.id = id;
      delegate->onButtonRelease((void*)this,&ev);
    }
  }
  return 0;
}

int mikaGlWindow::eventButtonMotion( int x, int y, int id )
{
  struct eventButton ev;
  if (delegate)
  {
    if (delegate->onButtonMotion)
    {
      ev.x = x;
      ev.y = y;
      ev.id = id;
      delegate->onButtonMotion((void*)this,&ev);
    }
  }
  return 0;
}

void mikaGlWindow::onEventButtonDown( int x, int y, int id )
{
  #ifdef _GLDEBUG
    printf("mikaGlWindow::onEventButtonDown\n");
  #endif
}

void mikaGlWindow::onEventButtonRelease( int x, int y, int id )
{
  #ifdef _GLDEBUG
    printf("mikaGlWindow::onEventButtonRelease\n");
  #endif
}

void mikaGlWindow::onEventButtonMotion( int x, int y, int id )
{
  #ifdef _GLDEBUG
    printf("mikaGlWindow::onEventButtonMotion\n");
  #endif
}

int mikaGlWindow::eventLoopThreaded( )
{
  XEvent ev;
  while(1)
  {
    XNextEvent(window->dpy,&ev);
  
    switch(ev.type)
    {
      case ButtonPress:
        #ifdef _GLDEBUG
          printf("mikaGlWindow::eventLoopThreaded ButtonPress\n");
        #endif
        eventButtonDown(ev.xbutton.x,ev.xbutton.y,0);
        break;
      case ButtonRelease:
        #ifdef _GLDEBUG
          printf("mikaGlWindow::eventLoopThreaded ButtonRelease\n");
        #endif
        eventButtonRelease(ev.xbutton.x,ev.xbutton.y,0);
        break;
      case MotionNotify:
        #ifdef _GLDEBUG
          printf("mikaGlWindow::eventLoopThread MotionNotify\n");
        #endif
        eventButtonMotion(ev.xmotion.x,ev.xmotion.y,0);
        break;
      case Expose:
        #ifdef _GLDEBUG
          printf("mikaGlWindow::eventLoopThreaded Expose\n");
        #endif
        eventExpose();
        break;
      default:
        break;
    }
  }
  return 0;
}

void *eventLoopThread(void*data)
{
  if (!data)
  {
    #ifdef _GLDEBUG
      printf("eventLoopThread error, invalid arguments\n");
    #endif
    return NULL;
  }
  #ifdef _GLDEBUG
    printf("eventLoopThread\n");
  #endif
  mikaGlWindow *glw = (mikaGlWindow*)data;
  glw->eventLoopThreaded();
  return NULL;
}

int mikaGlWindow::eventLoop( )
{
  pthread_t t;
  pthread_create(&t,NULL,eventLoopThread,this);
  return 0;
}

int mikaGlWindow::wait( )
{
  while(1)
  {
    sleep(1);
  }
  return 0;
}

mikaGlWindow::~mikaGlWindow( )
{
}

int mikaGlWindow::triggerExpose( )
{
  #ifdef _DEBUG
    printf("mikaGlWindow::triggerExpose\n");
  #endif
  
  return 0;
}
