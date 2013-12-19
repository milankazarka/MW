/*
 *    Copyright (C) by Milan Kazarka
 *
 *    You may distribute under the terms of the Artistic License,
 *    as specified in the Artistic.txt file
 *
 */

#include "mikaGlImage.h"
#include "mikaGlConfig.h"
#include "mikaGl.h"

void __jpeg_cb_error_exit(j_common_ptr cinfo)
{
  printf("jpeg_error\n");
}

#define _TEST_SCALING_NO
/** generate a mikaGlImage from a jpeg file
 */
mikaGlImage::mikaGlImage( unsigned char *filename )
{
    #ifdef _GLDEBUG
      printf("mikaGlImage::mikaGlImage\n");
    #endif
    
    csurface = NULL;
  
    struct jpeg_decompress_struct cinfo;
    struct jpeg_decompress_struct *ciptr;
    struct jpeg_error_mgr *emgr = NULL;
    FILE *fd = NULL;
    JSAMPLE *lb;
    ciptr = &cinfo;
      
    if ((fd=fopen((char*)filename,"rb"))==NULL)
    {
      printf("failed to open jpeg (%s)\n",(char*)filename);
      throw "failed to open jpeg";
    }
    else
    {
      #ifdef _GLDEBUG
        printf("opened '%s'\n",(char*)filename);
      #endif 
    }
    
    if (fd)
    {  
      emgr = (struct jpeg_error_mgr*)malloc(sizeof(struct jpeg_error_mgr));
      ciptr->err = jpeg_std_error(emgr);
      emgr->error_exit = __jpeg_cb_error_exit;
      
      jpeg_create_decompress(ciptr);
      jpeg_stdio_src(ciptr,fd);
      jpeg_read_header(ciptr,1);
      ciptr->out_color_space = JCS_RGB;
      jpeg_start_decompress(ciptr);

      static int h, w, cc, xs, yn, xn;
      static unsigned char pixel[4];
      pixel[0] = 0x10;
      pixel[1] = 0x10;
      pixel[2] = 0x10;
      pixel[3] = 0xff;
      
      w = ciptr->output_width;
      h = ciptr->output_height;
      cc = ciptr->output_components;
      mw = w;
      mh = h;

      pixels = (unsigned char*)malloc(mw*4*mh);
      for(yn = 0; yn < mh; yn++)
      {
        for(xn = 0; xn < mw; xn++)
        {
          memcpy(
            pixels+(yn*(mw*4))+(xn*4),
            (void*)&pixel,4
            );
        }
      }
      
      #ifdef _GLDEBUG
        printf("jpeg_image_attr h(%d) w(%d)\n",w,h);
      #endif
      
      lb = (JSAMPLE *)(*ciptr->mem->alloc_small)(
        (j_common_ptr)ciptr,
        JPOOL_PERMANENT,
        cc*w);
            
      yn = 0;
      while(ciptr->output_scanline < ciptr->output_height)
      {
        jpeg_read_scanlines(ciptr,&lb,1);
        for (xs = 0; xs < w; xs++)
        {
          pixel[2] = *(lb+3*xs);
          pixel[1] = *(lb+3*xs+1);
          pixel[0] = *(lb+3*xs+2);
            
          memcpy(
            pixels+(yn*(mw*4))+(xs*4),
            (void*)&pixel,4
            );
        }
        yn++;
      }
      
      jpeg_finish_decompress(ciptr);
      jpeg_destroy_decompress(ciptr);
      
      fclose(fd);
    
      csurface = cairo_image_surface_create_for_data(
        pixels,
        CAIRO_FORMAT_ARGB32,
        mw,mh,
        4*mw
        );
      if (!csurface)
      {
        #ifdef _GLDEBUG
          printf("couldn't create cairo surface from jpeg image\n");
        #endif
      }
      
      // testing scaling
      #ifdef _TEST_SCALING
        unsigned char *newPixels = (unsigned char*)malloc(mw*mh*4);
        memset(newPixels,0x00,mw*mh*4);
        cairo_surface_t *newSurface = cairo_image_surface_create_for_data(
          newPixels,
          CAIRO_FORMAT_ARGB32,
          mw,mh,
          4*mw
          );
        cairo_t *cnew = cairo_create(newSurface);
      
        cairo_scale(cnew,(double)0.3,(double)0.3);
        cairo_rotate(cnew,12.0*3.14/180);
      
        cairo_set_source_surface(cnew,csurface,0,0); // the source is the original image
        cairo_paint(cnew);
        cairo_surface_flush(csurface);
        cairo_surface_flush(newSurface);
        cairo_surface_finish(csurface);
        cairo_surface_finish(newSurface);
        memcpy(pixels,newPixels,mw*mh*4);
      
        free(newPixels);
        cairo_destroy(cnew);
        cairo_surface_destroy(newSurface);
      #endif
    }
}

mikaGlImage::~mikaGlImage( )
{
  free(pixels);
  if (csurface)
    free(csurface);
}

/** resizing. This retains the size of the original pixel surface.
 *  In a further resizing method scale the size of the surface too.
 *
 *  Here you can only make the surface smaller.
 */
int mikaGlImage::resize( int neww, int newh )
{
  if (neww>mw || newh>mh)
  {
    #ifdef _GLDEBUG
      printf("mikaGlImage::resize error, can't scale upwards in this method\n");
    #endif
    return 1;
  }

  unsigned char *newPixels = (unsigned char*)malloc(mw*mh*4);
  memset(newPixels,0x00,mw*mh*4);
  cairo_surface_t *newSurface = cairo_image_surface_create_for_data(
          newPixels,
          CAIRO_FORMAT_ARGB32,
          mw,mh,
          4*mw
          );
  cairo_t *cnew = cairo_create(newSurface);
      
  cairo_scale(cnew,(double)((double)neww/(double)mw),(double)((double)newh/(double)mh));
      
  cairo_set_source_surface(cnew,csurface,0,0); // the source is the original image
  cairo_paint(cnew);
  cairo_surface_flush(csurface);
  cairo_surface_flush(newSurface);
  cairo_surface_finish(csurface);
  cairo_surface_finish(newSurface);
  memcpy(pixels,newPixels,mw*mh*4);
      
  free(newPixels);
  cairo_destroy(cnew);
  cairo_surface_destroy(newSurface);
  
  return 0;
}
