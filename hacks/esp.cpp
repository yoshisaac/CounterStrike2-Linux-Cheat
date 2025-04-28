#include "esp.hpp"
#include "draw.hpp"

#include "../gui/config.hpp"

#include "player_info.hpp"

#include "../math.hpp"

void esp(pid_t game_pid, XdbeBackBuffer back_buffer, Display* draw_display, Window window, GC gc) {
  if (Xutil::focused_window_to_pid(draw_display) != game_pid) return;
  if (!config.esp.master) return;
  
  for (unsigned long i = 0; i < 64; ++i) {
    PlayerInfo::Player player = PlayerInfo::get_player(i);
    PlayerInfo::Player plocal = PlayerInfo::get_local_player();

    if (player.index == plocal.index) continue;
    if (config.esp.ignore_team == true && player.team == plocal.team) continue;
    if (config.esp.spotted == true && player.spotted == false) continue;
    if (player.index == -1) continue;
    if (player.health <= 0) continue;

    float y_offset[2];
    Vector3 location_z_offset = {player.bone_matrix[29].x, player.bone_matrix[29].y, player.bone_matrix[29].z + player.height + 9};
    world_to_screen(game_pid, location_z_offset, y_offset);
    
    float screen[2];
    if (!world_to_screen(game_pid, player.bone_matrix[29], screen, true)) continue;
      
    float distance = distance_3d(plocal.location, player.location);

    float y_offset_text[2];
    location_z_offset.z = location_z_offset.z + 5;
    world_to_screen(game_pid, location_z_offset, y_offset_text);

    float y_offset_down[2];
    location_z_offset.z = location_z_offset.z - 6;
    world_to_screen(game_pid, location_z_offset, y_offset_down);
    
    float health_offset = (13000/distance) * (1.0/plocal.fov_multiplier);
    float box_offset = (12000/distance) * (1.0/plocal.fov_multiplier);
    
    if (config.esp.health_bar) {
      //health bar shadow
      XSetForeground(draw_display, gc, Draw::black);
      XSetLineAttributes(draw_display, gc, 4, LineSolid, CapButt, JoinMiter);

      XDrawLine(draw_display, back_buffer, gc, screen[0] - health_offset - 5, y_offset[1]-2, screen[0] - health_offset - 5, screen[1]+2);

    
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



      XSetLineAttributes(draw_display, gc, 2, LineSolid, CapButt, JoinMiter);
      XDrawLine(draw_display, back_buffer, gc, screen[0] - health_offset - 5, screen[1], screen[0] - health_offset - 5, y_offset[1] - ydelta);          
    }      

    if (config.esp.health_text) {
      XSetForeground(draw_display, gc, Draw::black);
      XSetFont(draw_display, gc, Draw::shadowfont->fid);
      XDrawString(draw_display, back_buffer, gc, screen[0] - health_offset - 29 + 1, y_offset_down[1] + 1,
		  std::to_string(player.health).c_str(), strlen(std::to_string(player.health).c_str()));
      
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
      XDrawString(draw_display, back_buffer, gc, screen[0] - health_offset - 29, y_offset_down[1],
		  std::to_string(player.health).c_str(), strlen(std::to_string(player.health).c_str()));

    }
    
    if (config.esp.name) {
      XSetForeground(draw_display, gc, Draw::black);
      XSetFont(draw_display, gc, Draw::shadowfont->fid);
      XDrawString(draw_display, back_buffer, gc, screen[0] - int(strlen(player.name.c_str())*6/2) + 1, y_offset[1] - 5 + 1,
		  player.name.c_str(), strlen(player.name.c_str()));

      XSetForeground(draw_display, gc, Draw::white);
      XSetFont(draw_display, gc, Draw::font->fid);
      XDrawString(draw_display, back_buffer, gc, screen[0] - int(strlen(player.name.c_str())*6/2), y_offset[1] - 5,
		  player.name.c_str(), strlen(player.name.c_str()));
    }    
    
    //boxes
    if (config.esp.box) {
      XSetForeground(draw_display, gc, Draw::black);
      
      XSetLineAttributes(draw_display, gc, 4, LineSolid, CapButt, JoinMiter);

      XDrawLine(draw_display, back_buffer, gc, (screen[0] - box_offset), y_offset[1] - 1, (screen[0] - box_offset), screen[1] + 1); //left
      XDrawLine(draw_display, back_buffer, gc, (screen[0] + box_offset), y_offset[1] - 1, (screen[0] + box_offset), screen[1] + 1); //right
      XDrawLine(draw_display, back_buffer, gc, screen[0] + box_offset + 1, y_offset[1], screen[0] - box_offset - 1, y_offset[1]); //top
      XDrawLine(draw_display, back_buffer, gc, screen[0] - box_offset - 1, screen[1], screen[0] + box_offset + 1, screen[1]); //bottom
      
      XSetForeground(draw_display, gc, Xutil::xcolor_from_rgb(config.esp.box_color[0],
							      config.esp.box_color[1],
							      config.esp.box_color[2],
							      draw_display));

      XSetLineAttributes(draw_display, gc, 2, LineSolid, CapRound, JoinRound);

      XDrawLine(draw_display, back_buffer, gc, (screen[0] - box_offset), y_offset[1], (screen[0] - box_offset), screen[1]);
      XDrawLine(draw_display, back_buffer, gc, (screen[0] + box_offset), y_offset[1], (screen[0] + box_offset), screen[1]);
      XDrawLine(draw_display, back_buffer, gc, screen[0] + box_offset, y_offset[1], screen[0] - box_offset - 1, y_offset[1]);
      XDrawLine(draw_display, back_buffer, gc, screen[0] - box_offset, screen[1], screen[0] + box_offset, screen[1]);
    }
    
    //snap lines
    if (config.esp.snap_lines) {
      XSetForeground(draw_display, gc, Xutil::xcolor_from_rgb(config.esp.snap_lines_color[0],
							      config.esp.snap_lines_color[1],
							      config.esp.snap_lines_color[2],
							      draw_display));
      XSetLineAttributes(draw_display, gc, 0, LineSolid, CapButt, JoinMiter);
      
      XDrawLine(draw_display, back_buffer, gc, Engine::window_width/2, Engine::window_height, screen[0], screen[1]);
    }

    if (config.esp.weapon_name) {
      const char* weapon_name = player.weapon_name.c_str()+7;
   
      XSetForeground(draw_display, gc, Draw::black);
      XSetFont(draw_display, gc, Draw::shadowfont->fid);
      XDrawString(draw_display, back_buffer, gc, screen[0] - int(strlen(weapon_name)*6/2) + 1, screen[1] + 11 + 1,
		  weapon_name, strlen(weapon_name));

      XSetForeground(draw_display, gc, Draw::white);
      XSetFont(draw_display, gc, Draw::font->fid);
      XDrawString(draw_display, back_buffer, gc, screen[0] - int(strlen(weapon_name)*6/2), screen[1] + 11,
		  weapon_name, strlen(weapon_name));
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
      XSetForeground(draw_display, gc, Xutil::xcolor_from_rgb(config.esp.skeleton_color[0],
							      config.esp.skeleton_color[1],
							      config.esp.skeleton_color[2],
							      draw_display));
      float bone[2];
      float bone2[2];

      //back + head
      if (world_to_screen(game_pid, player.bone_matrix[1], bone) && world_to_screen(game_pid, player.bone_matrix[4], bone2))
	  XDrawLine(draw_display, back_buffer, gc, bone[0], bone[1], bone2[0], bone2[1]);
    
      if (world_to_screen(game_pid, player.bone_matrix[5], bone) && world_to_screen(game_pid, player.bone_matrix[4], bone2))
	XDrawLine(draw_display, back_buffer, gc, bone[0], bone[1], bone2[0], bone2[1]);
    
      if (world_to_screen(game_pid, player.bone_matrix[5], bone) && world_to_screen(game_pid, player.bone_matrix[6], bone2))
	XDrawLine(draw_display, back_buffer, gc, bone[0], bone[1], bone2[0], bone2[1]);
      
      //left leg + foot
      if (world_to_screen(game_pid, player.bone_matrix[1], bone) && world_to_screen(game_pid, player.bone_matrix[22], bone2))
	XDrawLine(draw_display, back_buffer, gc, bone[0], bone[1], bone2[0], bone2[1]);

      if (world_to_screen(game_pid, player.bone_matrix[23], bone) && world_to_screen(game_pid, player.bone_matrix[22], bone2))
	XDrawLine(draw_display, back_buffer, gc, bone[0], bone[1], bone2[0], bone2[1]);

      if (world_to_screen(game_pid, player.bone_matrix[23], bone) && world_to_screen(game_pid, player.bone_matrix[24], bone2))
	XDrawLine(draw_display, back_buffer, gc, bone[0], bone[1], bone2[0], bone2[1]);

      //right leg + foot
      if (world_to_screen(game_pid, player.bone_matrix[1], bone) && world_to_screen(game_pid, player.bone_matrix[25], bone2))
	XDrawLine(draw_display, back_buffer, gc, bone[0], bone[1], bone2[0], bone2[1]);

      if (world_to_screen(game_pid, player.bone_matrix[26], bone) && world_to_screen(game_pid, player.bone_matrix[25], bone2))
	XDrawLine(draw_display, back_buffer, gc, bone[0], bone[1], bone2[0], bone2[1]);

      if (world_to_screen(game_pid, player.bone_matrix[26], bone) && world_to_screen(game_pid, player.bone_matrix[27], bone2))
	XDrawLine(draw_display, back_buffer, gc, bone[0], bone[1], bone2[0], bone2[1]);

      //left arm
      if (world_to_screen(game_pid, player.bone_matrix[5], bone) && world_to_screen(game_pid, player.bone_matrix[8], bone2))
	XDrawLine(draw_display, back_buffer, gc, bone[0], bone[1], bone2[0], bone2[1]);

      if (world_to_screen(game_pid, player.bone_matrix[9], bone) && world_to_screen(game_pid, player.bone_matrix[8], bone2))
	XDrawLine(draw_display, back_buffer, gc, bone[0], bone[1], bone2[0], bone2[1]);

      if (world_to_screen(game_pid, player.bone_matrix[9], bone) && world_to_screen(game_pid, player.bone_matrix[10], bone2))
	XDrawLine(draw_display, back_buffer, gc, bone[0], bone[1], bone2[0], bone2[1]);

      //right arm
      if (world_to_screen(game_pid, player.bone_matrix[5], bone) && world_to_screen(game_pid, player.bone_matrix[13], bone2))
	XDrawLine(draw_display, back_buffer, gc, bone[0], bone[1], bone2[0], bone2[1]);

      if (world_to_screen(game_pid, player.bone_matrix[14], bone) && world_to_screen(game_pid, player.bone_matrix[13], bone2))
	XDrawLine(draw_display, back_buffer, gc, bone[0], bone[1], bone2[0], bone2[1]);

      if (world_to_screen(game_pid, player.bone_matrix[14], bone) && world_to_screen(game_pid, player.bone_matrix[15], bone2))
	XDrawLine(draw_display, back_buffer, gc, bone[0], bone[1], bone2[0], bone2[1]);
    }

    if (config.esp.head_dot) {
      float circle_size = (7500/distance) * (1.0/plocal.fov_multiplier);
      float ear_length = (6500/distance) * (1.0/plocal.fov_multiplier);
      float head[2];

      XSetLineAttributes(draw_display, gc, 0, LineSolid, CapButt, JoinMiter);
      XSetForeground(draw_display, gc, Xutil::xcolor_from_rgb(config.esp.head_dot_color[0],
							      config.esp.head_dot_color[1],
							      config.esp.head_dot_color[2],
							      draw_display));
      
      if (world_to_screen(game_pid, player.bone_matrix[6], head)){
	if (config.esp.head_dot_fill)
	  XFillArc(draw_display, back_buffer, gc, head[0]-(circle_size/2), head[1]-(circle_size/2), circle_size, circle_size, 0, 360*65);
	else
	  XDrawArc(draw_display, back_buffer, gc, head[0]-(circle_size/2), head[1]-(circle_size/2), circle_size, circle_size, 0, 360*65);

	if (config.esp.cat_ears) {
	//right side
	XDrawLine(draw_display, back_buffer, gc, head[0] + (circle_size/2) - 1, head[1], head[0] + (circle_size/2) - 1, head[1] - ear_length);
	XDrawLine(draw_display, back_buffer, gc, head[0], head[1] - (circle_size/2), head[0] + (circle_size/2) - 1, head[1] - ear_length);

	//left side
	XDrawLine(draw_display, back_buffer, gc, head[0] - (circle_size/2), head[1], head[0] - (circle_size/2), head[1] - ear_length);
	XDrawLine(draw_display, back_buffer, gc, head[0], head[1] - (circle_size/2), head[0] - (circle_size/2), head[1] - ear_length);
	
	}

      }
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
 
