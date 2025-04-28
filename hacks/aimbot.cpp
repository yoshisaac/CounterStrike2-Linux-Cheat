#include "aimbot.hpp"

#include "player_info.hpp"

#include "../gui/config.hpp"

#include "../memory.hpp"
#include "../client.hpp"
#include "../math.hpp"

void aimbot(pid_t game_pid, Display* aim_display) {
  for (unsigned long i = 0; i < 64; ++i) {
    if (!config.aim.master) break;
      
    PlayerInfo::Player player = PlayerInfo::get_player(i);
    PlayerInfo::Player plocal = PlayerInfo::get_local_player();

      
    if (player.index == plocal.index) continue;
    if (config.esp.ignore_team == true && player.team == plocal.team) continue;
    if (player.health <= 0) continue;
    if (config.aim.spotted == true && player.spotted == false) continue;
    
    float _out[2];
    if (!world_to_screen(game_pid, player.bone_matrix[6], _out)) continue;
    
    if (PlayerInfo::get_player(Aimbot::index).health <= 0 ||
	(config.aim.spotted == true && PlayerInfo::get_player(Aimbot::index).spotted == false)) {
      Aimbot::index = -1;
      PlayerInfo::l_players[Aimbot::index].fov_distance = 999999;
    }
      
    Euler plocal_angles;
    Memory::read(game_pid, Client::view_angles, &plocal_angles, sizeof(float[3]));
    Euler plocal_angles_final = plocal_angles;

    Vector3 delta_location = { float(plocal.location.x - player.bone_matrix[6].x),
			       float(plocal.location.y - player.bone_matrix[6].y),
			       float(plocal.location.z + plocal.height - player.bone_matrix[6].z)};

    float hyp = sqrt(delta_location.x * delta_location.x + delta_location.y * delta_location.y);

    plocal_angles_final.pitch = atan(delta_location.z / hyp) * radpi;
    plocal_angles_final.yaw = atan(delta_location.y / delta_location.x) * radpi;

    float distance = distance_3d(plocal.location, player.bone_matrix[6]);

    float fov = sqrt(powf(sin((plocal_angles.pitch - plocal_angles_final.pitch + plocal.aim_punch.pitch * 2) * pideg) * 180, 2.0) +
		     powf(sin((plocal_angles.yaw   - plocal_angles_final.yaw   + plocal.aim_punch.yaw   * 2) * pideg) * 180, 2.0));
    
    if (Aimbot::index == i) {
	
      PlayerInfo::l_players[Aimbot::index].fov_distance = fov;
	
      if (delta_location.x >= 0.0f) {
	plocal_angles_final.yaw += 180.0f;
      }

      if (fov > config.aim.fov && !Xutil::key_down(aim_display, config.aim.key)) {
	PlayerInfo::l_players[Aimbot::index].fov_distance = 999999;
	Aimbot::index = -1;
	continue;
      }
	
      while (plocal_angles_final.pitch > 89)
	plocal_angles_final.pitch -= 180;

      while (plocal_angles_final.pitch < -89)
	plocal_angles_final.pitch += 180;

      if (isNaN(plocal_angles_final.pitch))
	plocal_angles_final.pitch = 0;

      if (isNaN(plocal_angles_final.yaw))
	plocal_angles_final.yaw = 0;
	
      if (config.aim.recoil) {
	plocal_angles_final.pitch -= (plocal.aim_punch.pitch * 2);
	plocal_angles_final.yaw -= (plocal.aim_punch.yaw * 2);
      } 

    }

    if (Aimbot::index == i && Xutil::key_down(aim_display, config.aim.key)) {
      Memory::write(game_pid, Client::view_angles, &plocal_angles_final, sizeof(float[2]));

      char a = 0b0;	
      Memory::read(game_pid, Client::force_attack, &a, sizeof(char));

      if (config.aim.auto_shoot) {
	a = a | (1<<0);
      }

      Memory::write(game_pid, Client::force_attack, &a, sizeof(char));

    } else {
      if (fov <= config.aim.fov && fov < PlayerInfo::get_player(Aimbot::index).fov_distance) {
	Aimbot::index = i;
	PlayerInfo::l_players[Aimbot::index].fov_distance = fov;
      }
    }
  }
}
