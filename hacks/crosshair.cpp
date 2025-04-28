#include "crosshair.hpp"

#include "draw.hpp"

#include "../xutil.hpp"
#include "../math.hpp"

#include "../gui/config.hpp"

#include "aimbot.hpp"
#include "player_info.hpp"

void crosshair(pid_t game_pid, XdbeBackBuffer back_buffer, Display* draw_display, Window window, GC gc) {
  if (Xutil::focused_window_to_pid(draw_display) != game_pid) return;

  int screen_center[2] = {Engine::window_width/2, Engine::window_height/2};
  
  PlayerInfo::Player plocal = PlayerInfo::get_local_player();
  
  if (plocal.health <= 0) return; //this is generally also true when outside of a match
  
  if (config.visuals.crosshair) {
    if (!(config.visuals.sniper_only && (plocal.weapon_name != "weapon_awp" && plocal.weapon_name != "weapon_g3sg1" &&
					plocal.weapon_name != "weapon_scar20" && plocal.weapon_name != "weapon_ssg08"))) {
    
      XSetLineAttributes(draw_display, gc, 3, LineSolid, CapButt, JoinBevel);
  
  
      XSetForeground(draw_display, gc, Xutil::xcolor_from_rgb(config.visuals.crosshair_color[0],
							      config.visuals.crosshair_color[1],
							      config.visuals.crosshair_color[2],
							      draw_display));

      int punch_x = 0;
      int punch_y = 0;

      if (config.visuals.visualize_recoil) {
	punch_x = (int(screen_center[0] / 90)) * plocal.aim_punch.yaw;
	punch_y = (int(screen_center[1] / 45)) * plocal.aim_punch.pitch;
      }
      
      XDrawLine(draw_display, back_buffer, gc,
		screen_center[0] - punch_x - 13,
		screen_center[1] + punch_y,
		screen_center[0] - punch_x + 14,
		screen_center[1] + punch_y);

      XDrawLine(draw_display, back_buffer, gc,
		screen_center[0] - punch_x,
		screen_center[1] + punch_y - 13,
		screen_center[0] - punch_x,
		screen_center[1] + punch_y + 14);
    }
  }

  
  if (config.aim.show_fov) {
    XSetLineAttributes(draw_display, gc, 0, LineSolid, CapButt, JoinBevel);
    
    XSetForeground(draw_display, gc, Draw::white);
  
    float radius = tanf(config.aim.fov / 180.0f * M_PI / 2.0f) /
      tanf((90*plocal.fov_multiplier) / 180.0f * M_PI / 2.0f) *
      Engine::window_width / 2.0f;

    XDrawArc(draw_display, back_buffer, gc, screen_center[0]-(radius/2), screen_center[1]-(radius/2), radius, radius, 0, 360*65);
  }


  if (config.aim.snap_lines) {
    XSetLineAttributes(draw_display, gc, 0, LineSolid, CapButt, JoinBevel);
  
    XSetForeground(draw_display, gc, Xutil::xcolor_from_rgb(config.aim.snap_lines_color[0],
							    config.aim.snap_lines_color[1],
							    config.aim.snap_lines_color[2],
							    draw_display));

    if (PlayerInfo::get_player(Aimbot::index).index != -1) {
      float out[2];
      world_to_screen(game_pid, PlayerInfo::get_player(Aimbot::index).bone_matrix[6], out);

      XDrawLine(draw_display, back_buffer, gc, screen_center[0], screen_center[1], out[0], out[1]);
    }
  }

}
