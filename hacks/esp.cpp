#include "esp.hpp"
#include "draw.hpp"

#include "../gui/config.hpp"

#include "player_info.hpp"

#include "../math.hpp"

void esp(pid_t game_pid, XdbeBackBuffer back_buffer, Display* draw_display, Window window, GC gc) {
  if (Xutil::window_to_pid(draw_display, Xutil::focused_window(draw_display)) != game_pid) return;
  if (!config.esp.master) return;
  
  for (unsigned long i = 0; i < 64; ++i) {
    PlayerInfo::Player player = PlayerInfo::get_player(i);
    PlayerInfo::Player plocal = PlayerInfo::get_local_player();

    if (player.index == plocal.index) continue;
    if (player.team == plocal.team) continue;
    if (player.index == -1) continue;
    if (player.health <= 0) continue;
      
    float y_offset[2];
    float location_z_offset[3] = {player.bone_matrix[29][0], player.bone_matrix[29][1], player.bone_matrix[29][2] + player.height + 9};
    world_to_screen(game_pid, location_z_offset, y_offset);
    
    float screen[2];
    if (!world_to_screen(game_pid, player.bone_matrix[29], screen)) continue;

      
    float distance = distance_3d(plocal.location, player.location);

    float y_offset_text[2];
    location_z_offset[2] = location_z_offset[2] + 5;
    world_to_screen(game_pid, location_z_offset, y_offset_text);

    if (config.esp.health) {
      //health bar shadow
      XSetForeground(draw_display, gc, Draw::black);
      XSetFont(draw_display, gc, Draw::shadowfont->fid);
      XSetLineAttributes(draw_display, gc, 4, LineSolid, CapButt, JoinMiter);

      XDrawLine(draw_display, back_buffer, gc, screen[0] - (13000/distance) - 5, y_offset[1]-2, screen[0] - (13000/distance) - 5, screen[1]+2);
      XDrawString(draw_display, back_buffer, gc, screen[0] - (13000/distance) + 1, y_offset_text[1] + 1, std::to_string(player.health).c_str(), strlen(std::to_string(player.health).c_str()));
    
      int ydelta = (y_offset[1] - screen[1]) * (1.f - (player.health / 100.f));
	
      //health bar color
      if (player.health > 100) { //show that they have more health than what is conventional
	XSetForeground(draw_display, gc, Draw::cyan);
	ydelta = 0;
      }
      else if (player.health <= 100 && player.health >= 90)
	XSetForeground(draw_display, gc, Draw::green);
      else if (player.health < 90 && player.health > 60)
	XSetForeground(draw_display, gc, Draw::yellow);
      else if (player.health <= 60 && player.health > 35)
	XSetForeground(draw_display, gc, Draw::orange);
      else if (player.health <= 35)
	XSetForeground(draw_display, gc, Draw::red);

      XSetFont(draw_display, gc, Draw::font->fid);


      XSetLineAttributes(draw_display, gc, 2, LineSolid, CapButt, JoinMiter);
      XDrawLine(draw_display, back_buffer, gc, screen[0] - (13000/distance) - 5, screen[1], screen[0] - (13000/distance) - 5, y_offset[1] - ydelta);          
      XDrawString(draw_display, back_buffer, gc, screen[0] - (13000/distance), y_offset_text[1], std::to_string(player.health).c_str(), strlen(std::to_string(player.health).c_str()));
    }      

    //boxes
    if (config.esp.box) {
      XSetForeground(draw_display, gc, Draw::black);
      
      XSetLineAttributes(draw_display, gc, 4, LineSolid, CapButt, JoinMiter);

      XDrawLine(draw_display, back_buffer, gc, (screen[0] - (12000/distance)), y_offset[1] - 1, (screen[0] - (12000/distance)), screen[1] + 1); //left
      XDrawLine(draw_display, back_buffer, gc, (screen[0] + (12000/distance)), y_offset[1] - 1, (screen[0] + (12000/distance)), screen[1] + 1); //right
      XDrawLine(draw_display, back_buffer, gc, screen[0] + (12000/distance) + 1, y_offset[1], screen[0] - (12000/distance) - 1, y_offset[1]); //top
      XDrawLine(draw_display, back_buffer, gc, screen[0] - (12000/distance) - 1, screen[1], screen[0] + (12000/distance) + 1, screen[1]); //bottom
      
      XSetForeground(draw_display, gc, Draw::red);

      XSetLineAttributes(draw_display, gc, 2, LineSolid, CapRound, JoinRound);

      XDrawLine(draw_display, back_buffer, gc, (screen[0] - (12000/distance)), y_offset[1], (screen[0] - (12000/distance)), screen[1]);
      XDrawLine(draw_display, back_buffer, gc, (screen[0] + (12000/distance)), y_offset[1], (screen[0] + (12000/distance)), screen[1]);
      XDrawLine(draw_display, back_buffer, gc, screen[0] + (12000/distance), y_offset[1], screen[0] - (12000/distance) - 1, y_offset[1]);
      XDrawLine(draw_display, back_buffer, gc, screen[0] - (12000/distance), screen[1], screen[0] + (12000/distance), screen[1]);
    }

    //snap lines
    if (config.esp.snap_lines) {
      XSetForeground(draw_display, gc, Draw::green);
      XSetLineAttributes(draw_display, gc, 0, LineSolid, CapButt, JoinMiter);
      
      XDrawLine(draw_display, back_buffer, gc, 1920/2, 1080, screen[0], screen[1]);
    }

    /*
    XSetForeground(draw_display, gc, Draw::white);
    for (int i = 0; i < 50; ++i) {
      float bone[2];
      if (i >= 30 && i < 50) continue;
      world_to_screen(game_pid, player.bone_matrix[i], bone);
      XDrawString(draw_display, back_buffer, gc, bone[0], bone[1], std::to_string(i).c_str(), strlen(std::to_string(i).c_str()));
    }
    */

    if (config.esp.skeleton) {
      XSetLineAttributes(draw_display, gc, 0, LineSolid, CapButt, JoinMiter);
      XSetForeground(draw_display, gc, Draw::white);
      float bone[2];
      float bone2[2];

      //back + head
      if (!world_to_screen(game_pid, player.bone_matrix[1], bone)) continue;
      if (!world_to_screen(game_pid, player.bone_matrix[4], bone2)) continue;
      XDrawLine(draw_display, back_buffer, gc, bone[0], bone[1], bone2[0], bone2[1]);
    
      if (!world_to_screen(game_pid, player.bone_matrix[5], bone)) continue;
      XDrawLine(draw_display, back_buffer, gc, bone[0], bone[1], bone2[0], bone2[1]);
    
      if (!world_to_screen(game_pid, player.bone_matrix[6], bone2)) continue;
      XDrawLine(draw_display, back_buffer, gc, bone[0], bone[1], bone2[0], bone2[1]);

      //left leg + foot
      if (!world_to_screen(game_pid, player.bone_matrix[1], bone)) continue;
      if (!world_to_screen(game_pid, player.bone_matrix[22], bone2)) continue;
      XDrawLine(draw_display, back_buffer, gc, bone[0], bone[1], bone2[0], bone2[1]);

      if (!world_to_screen(game_pid, player.bone_matrix[23], bone)) continue;
      XDrawLine(draw_display, back_buffer, gc, bone[0], bone[1], bone2[0], bone2[1]);

      if (!world_to_screen(game_pid, player.bone_matrix[24], bone2)) continue;
      XDrawLine(draw_display, back_buffer, gc, bone[0], bone[1], bone2[0], bone2[1]);

      //right leg + foot
      if (!world_to_screen(game_pid, player.bone_matrix[1], bone)) continue;
      if (!world_to_screen(game_pid, player.bone_matrix[25], bone2)) continue;
      XDrawLine(draw_display, back_buffer, gc, bone[0], bone[1], bone2[0], bone2[1]);

      if (!world_to_screen(game_pid, player.bone_matrix[26], bone)) continue;
      XDrawLine(draw_display, back_buffer, gc, bone[0], bone[1], bone2[0], bone2[1]);

      if (!world_to_screen(game_pid, player.bone_matrix[27], bone2)) continue;
      XDrawLine(draw_display, back_buffer, gc, bone[0], bone[1], bone2[0], bone2[1]);

      //left arm
      if (!world_to_screen(game_pid, player.bone_matrix[5], bone)) continue;
      if (!world_to_screen(game_pid, player.bone_matrix[8], bone2)) continue;
      XDrawLine(draw_display, back_buffer, gc, bone[0], bone[1], bone2[0], bone2[1]);

      if (!world_to_screen(game_pid, player.bone_matrix[9], bone)) continue;
      XDrawLine(draw_display, back_buffer, gc, bone[0], bone[1], bone2[0], bone2[1]);

      if (!world_to_screen(game_pid, player.bone_matrix[10], bone2)) continue;
      XDrawLine(draw_display, back_buffer, gc, bone[0], bone[1], bone2[0], bone2[1]);

      //right arm
      if (!world_to_screen(game_pid, player.bone_matrix[5], bone)) continue;
      if (!world_to_screen(game_pid, player.bone_matrix[13], bone2)) continue;
      XDrawLine(draw_display, back_buffer, gc, bone[0], bone[1], bone2[0], bone2[1]);

      if (!world_to_screen(game_pid, player.bone_matrix[14], bone)) continue;
      XDrawLine(draw_display, back_buffer, gc, bone[0], bone[1], bone2[0], bone2[1]);

      if (!world_to_screen(game_pid, player.bone_matrix[15], bone2)) continue;
      XDrawLine(draw_display, back_buffer, gc, bone[0], bone[1], bone2[0], bone2[1]);
    }

    //index
    // XSetFont(draw_display, gc, Draw::shadowfont->fid);
    // XSetForeground(draw_display, gc, Draw::black);

      
    // XDrawString(draw_display, back_buffer, gc, screen[0] + 1, screen[1] - 10 + 1, std::to_string(player.index).c_str(), strlen(std::to_string(player.index).c_str()));

    // XSetFont(draw_display, gc, Draw::font->fid);
    // XSetForeground(draw_display, gc, Draw::white);

      
    // XDrawString(draw_display, back_buffer, gc, screen[0], screen[1] - 10, std::to_string(player.index).c_str(), strlen(std::to_string(player.index).c_str()));

  }
}
 
