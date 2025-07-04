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

#include <climits>
#include <stdio.h>
#include <string>

namespace Xutil {

  typedef unsigned long Color;
  
  inline Color xcolor_from_rgba(short red, short green, short blue, short alpha, Display* d) {
    XColor color;
    int screen = DefaultScreen(d);
    
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
  
  inline Color xcolor_from_rgb(short red, short green, short blue, Display* d) {
    return xcolor_from_rgba(red, green, blue, 255, d);
  }

  inline Window focused_window(Display* d) {
    Window focusedWindow;
    int revert;
    XGetInputFocus(d, &focusedWindow, &revert);
    return focusedWindow;
  }

  inline pid_t window_to_pid(Display* d, Window window) {
    Atom prop = XInternAtom(d, "_NET_WM_PID", False);

    Atom actualType;
    int format;
    unsigned long nItems, bytesAfter;
    unsigned char *propData = NULL;

    if (XGetWindowProperty(d, window, prop, 0, LONG_MAX / 4, False, AnyPropertyType,
			   &actualType, &format, &nItems, &bytesAfter, &propData) == Success) {
      if (propData != NULL) {
	pid_t *pidArray = reinterpret_cast<pid_t *>(propData);
	pid_t pid = pidArray[0];
	XFree(propData);
	return pid;
      }
    }

    return -1; // Unable to retrieve PID
  }

  inline pid_t focused_window_to_pid(Display* d) {
    return Xutil::window_to_pid(d, Xutil::focused_window(d));
  }
  
  inline bool key_down(Display* d, int key) {
    char keys[32];
    XQueryKeymap(d, keys);
    
    KeyCode keyCode = XKeysymToKeycode(d, key);
    return (keys[keyCode / 8] & (1 << (keyCode % 8))) != 0;
  }

  inline void display_resolution(int resolution[2]) {
    FILE* fp = popen("xrandr | grep primary -A 1 | grep \'\\*\' | awk {\'print $1\'}", "r");
    char buffer[1024];
    int x, y;

    if (fp == NULL) {
      resolution = 0;
      return;
    }

    if (fgets(buffer, sizeof(buffer), fp) != NULL) {
      std::string num_buffer = "";
      for (unsigned int i = 0; i < strlen(buffer); ++i) {
	if (buffer[i] == 'x') { x = atoi(num_buffer.c_str()); num_buffer = ""; continue; }
	num_buffer += buffer[i];
      }
      y = atoi(num_buffer.c_str());
    }
    
    pclose(fp);

    resolution[0] = x;
    resolution[1] = y;
  }
}

#endif
