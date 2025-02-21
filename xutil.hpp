#ifndef XUTIL_HPP
#define XUTIL_HPP

#include <X11/Xos.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/extensions/shape.h>
#include <X11/extensions/Xfixes.h>
#include <X11/extensions/Xcomposite.h>
#include <X11/extensions/Xdbe.h>

#include <stdio.h>

namespace Xutil {

  typedef unsigned long Color;
  
  inline Color xcolor_from_rgba(short red, short green, short blue, short alpha, Display* d, int screen) {
    XColor color;

    // m_color.red = red * 65535 / 255;
    color.red = (red * 0xFFFF) / 0xFF;
    color.green = (green * 0xFFFF) / 0xFF;
    color.blue = (blue * 0xFFFF) / 0xFF;
    color.flags = DoRed | DoGreen | DoBlue;

    
    if (!XAllocColor(d, DefaultColormap(d, screen), &color)) {
      printf("createXColorFromRGB: Cannot create color\n");
    }


    *(&color.pixel) = ((*(&color.pixel)) & 0x00ffffff) | (alpha << 24);
    return color.pixel;
  }
  
  inline Color xcolor_from_rgb(short red, short green, short blue, Display* d, int screen) {
    return xcolor_from_rgba(red, green, blue, 255, d, screen);
  }

}

#endif
