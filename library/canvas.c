/*
 *    Copyright (C) by Milan Kazarka
 *
 *    You may distribute under the terms of the Artistic License,
 *    as specified in the Artistic.txt file
 *
 */

#include "canvas.h"
#include "config.h"
#include "widget.h"
#include "window.h"
#include "microDebug.h"

milanCanvas::milanCanvas( object *parent )
{
  printf("milanCanvas::milanCanvas\n");
  myParent = parent;
  milanWindow *window = (milanWindow*)parent;
  milanWidget *widget = new milanWidget(0,0,window->mw,window->mh);
  canvasWidget = (object*)widget;
  objects = NULL;
}

milanCanvas::~milanCanvas( )
{
  #ifdef _DEBUG
    printf("milanCanvas::~milanCanvas\n");
  #endif
  milanCanvasObject *current = objects;
  milanCanvasObject *next = NULL;
  while(current)
  {
    next = current->next;
    delete current;
    current = next;
  }
}

int milanCanvas::raise( )
{
  return 0;
}

int milanCanvas::isEventWidget( struct milanCanvasObject *co, struct eventButton *button )
{
  if (!co || !button)
    return 0;
  
  milanWidget *widget = (milanWidget*)co->myWidget;
  if (
    widget->mx < button->x &&
    widget->my < button->y &&
    widget->mx+widget->mw > button->x &&
    widget->my+widget->mh > button->y
    )
  {
    return 1;
  }
  
  return 0;
}

int milanCanvas::onButtonDown( struct eventButton *button )
{
  #ifdef _DEBUG
    printf("milanCanvas::onButtonDown (%dx%d)\n",
      button->x,button->y);
  #endif
  
  onButtonDownEvent(button);
  
  milanWidget *widget = NULL;
  milanCanvasObject *current = objects;
  while(current)
  {
    if (isEventWidget(current,button))
    {
      widget = (milanWidget*)current->myWidget;
        widget->onButtonDown(button);
    }
    
    current = current->next;
  }
  return 0;
}

int milanCanvas::onButtonUp( struct eventButton *button )
{
  #ifdef _DEBUG
    printf("milanCanvas::onButtonUp (%dx%d)\n",
      button->x,button->y);
  #endif
  
  onButtonUpEvent(button);
  
  milanWidget *widget = NULL;
  milanCanvasObject *current = objects;
  while(current)
  {
    if (isEventWidget(current,button))
    {
      widget = (milanWidget*)current->myWidget;
        widget->onButtonUp(button);
    }
      
    current = current->next;
  }
  return 0;
}

void milanCanvas::onButtonDownEvent( struct eventButton *button )
{
  mprintf("milanCanvas::onButtonDownEvent\n");
}

void milanCanvas::onButtonUpEvent( struct eventButton *button )
{
  mprintf("milanCanvas::onButtonUpEvent\n");
}

int milanCanvas::triggerExpose( )
{
  #ifdef _DEBUG
    printf("milanCanvas::triggerExpose\n");
  #endif
  
  if (canvasWidget)
  {
    milanWidget *widget = (milanWidget*)canvasWidget;
    widget->drawOnWindow( (milanWindow*)myParent );
  
    milanCanvasObject *current = objects;
    while(current)
    {
      widget = (milanWidget*)current->myWidget;
      widget->drawOnWindow( (milanWindow*)myParent );
      current = current->next;
    }
  }
  #ifdef _DEBUG
    printf("\tend\n");
  #endif
  
  return 0;
}

int milanCanvas::addWidget( object *widget )
{
  milanCanvasObject *co = new milanCanvasObject(widget);
  co->next = objects;
  objects = co;
  return 0;
}

// canvas object

milanCanvasObject::milanCanvasObject( object *widget )
{
  milanWidget *mw = (milanWidget*)widget;
  #ifdef _DEBUG
    printf("milanCanvasObject::milanCanvasObject widget(%p) (%dx%d+%d+%d)\n",
      widget,mw->mw,mw->mh,mw->mx,mw->my);
  #endif
  myWidget = widget;
}

milanCanvasObject::~milanCanvasObject( )
{
}
