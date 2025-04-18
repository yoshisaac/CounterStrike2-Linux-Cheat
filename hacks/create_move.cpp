#include "create_move.hpp"

#include "../xutil.hpp"

#include "aimbot.hpp"

void create_move(pid_t game_pid, Display* move_display) {
  for (;;) {
    if (Xutil::focused_window_to_pid(move_display) != game_pid) { usleep(1000*1000/250); continue; }
    
    aimbot(game_pid, move_display);

    //bhop or something could go here
    
    usleep(1000*1000/250);
  }
}
