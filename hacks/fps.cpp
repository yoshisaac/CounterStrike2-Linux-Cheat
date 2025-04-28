#include "fps.hpp"

#include "draw.hpp"

#include <thread>
#include <string>

#include "../xutil.hpp"
#include "../gui/config.hpp"


#define FPSMETERSAMPLE 100
std::string fpsstring = "";
int fpsmeterc = 0;

std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();

void fps(XdbeBackBuffer back_buffer, Display* draw_display, Window window, GC gc) {
  if (!config.visuals.fps) return;
  
  
  fpsmeterc++;
  if(fpsmeterc == FPSMETERSAMPLE) {
    fpsmeterc = 0;
    t1 = t2;
    t2 = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
    fpsstring = /*to_string(duration) + " / " +*/ std::to_string(1000000*FPSMETERSAMPLE/duration);
  }

  XSetForeground(draw_display, gc, Draw::black);
  XFillRectangle(draw_display, back_buffer, gc, 0, 0, 70, 30);

  XSetForeground(draw_display, gc, Draw::white);
  XSetFont(draw_display, gc, Draw::font->fid);

  const char* text = ("FPS: " + fpsstring).c_str();
  XDrawString(draw_display, back_buffer, gc, 10, 20, text, strlen(text));


}
