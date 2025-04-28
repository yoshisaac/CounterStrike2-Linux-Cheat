#include "draw.hpp"

#include "esp.hpp"
#include "crosshair.hpp"
#include "fps.hpp"

#include "../engine.hpp"

#include "../xutil.hpp"

void draw(pid_t game_pid, XdbeBackBuffer back_buffer, Display* draw_display, Window window) {
  GC gc;
  for (;;) {
    XGCValues values;
    gc = XCreateGC(draw_display, window, GCLineWidth, &values); //Allocate memory for the graphics context
    //clear the buffer
    XSetForeground(draw_display, gc, 0);
    XFillRectangle(draw_display, back_buffer, gc, 0, 0, Engine::window_width, Engine::window_height);
    
    //write to buffer
    esp(game_pid, back_buffer, draw_display, window, gc);
    crosshair(game_pid, back_buffer, draw_display, window, gc);
    fps(back_buffer, draw_display, window, gc);
    
    //swap buffers
    XdbeSwapInfo swap_info;
    swap_info.swap_window = window;
    swap_info.swap_action = 0;
    XdbeSwapBuffers(draw_display, &swap_info, 1);
    
    XFreeGC(draw_display, gc); //free the memory we allocated for the graphics context
    usleep(1000*1000/300); //target frame rate (300fps)
  }
}
