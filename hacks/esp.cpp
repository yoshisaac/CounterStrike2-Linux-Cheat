#include "esp.hpp"

#include "player_info.hpp"

#include "../math.hpp"

void esp(pid_t game_pid, XdbeBackBuffer back_buffer, Display* draw_display, Window window, GC gc) {
    for (unsigned long i = 0; i < 64; ++i) {
      PlayerInfo::Player player = PlayerInfo::get_player(i);
      PlayerInfo::Player plocal = PlayerInfo::get_local_player();

      if (player.index == plocal.index) continue;
      if (player.index == -1) continue;
      if (player.health <= 0) continue;
      
      float screen[2];
      if (!world_to_screen(game_pid, player.location, screen)) continue;
	    

     
      float distance = distanceFormula3D(plocal.location, player.location);

      float y_offset[2];
      float location_z_offset[3] = {player.location[0], player.location[1], player.location[2] + 70};
      world_to_screen(game_pid, location_z_offset, y_offset);

      
      XSetForeground(draw_display, gc, Xutil::xcolor_from_rgb(0,
							      0,
							      0,
							      draw_display,
							      DefaultScreen(draw_display)
							      )
		     );

      XSetLineAttributes(draw_display, gc, 4, LineSolid, CapButt, JoinMiter);

      XDrawLine(draw_display, back_buffer, gc, (screen[0] - (10000/distance)), y_offset[1] - 1, (screen[0] - (10000/distance)), screen[1] + 1); //left
      XDrawLine(draw_display, back_buffer, gc, (screen[0] + (10000/distance)), y_offset[1] - 1, (screen[0] + (10000/distance)), screen[1] + 1); //right
      XDrawLine(draw_display, back_buffer, gc, screen[0] + (10000/distance) + 1, y_offset[1], screen[0] - (10000/distance) - 1, y_offset[1]); //top
      XDrawLine(draw_display, back_buffer, gc, screen[0] - (10000/distance) - 1, screen[1], screen[0] + (10000/distance) + 1, screen[1]); //bottom
      
      XSetForeground(draw_display, gc, Xutil::xcolor_from_rgb(255,
							      0,
							      50,
							      draw_display,
							      DefaultScreen(draw_display)
							      )
		     );

      XSetLineAttributes(draw_display, gc, 2, LineSolid, CapButt, JoinRound);

      XDrawLine(draw_display, back_buffer, gc, (screen[0] - (10000/distance)), y_offset[1], (screen[0] - (10000/distance)), screen[1]);
      XDrawLine(draw_display, back_buffer, gc, (screen[0] + (10000/distance)), y_offset[1], (screen[0] + (10000/distance)), screen[1]);
      XDrawLine(draw_display, back_buffer, gc, screen[0] + (10000/distance), y_offset[1], screen[0] - (10000/distance) - 1, y_offset[1]);
      XDrawLine(draw_display, back_buffer, gc, screen[0] - (10000/distance), screen[1], screen[0] + (10000/distance), screen[1]);

	
    }
}
 
