#include "../xutil.hpp"

void draw(pid_t game_pid, XdbeBackBuffer back_buffer, Display* draw_display, Window window);

namespace Draw {
  inline XFontStruct* shadowfont;
  inline XFontStruct* font;

  inline Xutil::Color black;
  inline Xutil::Color red;
  inline Xutil::Color green;
  inline Xutil::Color white;
  inline Xutil::Color cyan;
  inline Xutil::Color yellow;
  inline Xutil::Color orange;
}
