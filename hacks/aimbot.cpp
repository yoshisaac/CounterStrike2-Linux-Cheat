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
    if (player.team == plocal.team) continue;
    if (player.health <= 0) continue;
      
    if (PlayerInfo::get_player(Aimbot::index).health <= 0) {
      Aimbot::index = -1;
    }
      
    float plocal_angles[3];
    Memory::read(game_pid, Client::view_angles, &plocal_angles, sizeof(float[3]));
    float plocal_angles_final[3] = {*plocal_angles};

    float delta_location[3] = { float(plocal.location[0] - player.bone_matrix[6][0]),
				float(plocal.location[1] - player.bone_matrix[6][1]),
				float(plocal.location[2] + plocal.height - player.bone_matrix[6][2])};

    float hyp = sqrt(delta_location[0] * delta_location[0] + delta_location[1] * delta_location[1]);

    plocal_angles_final[0] = atan(delta_location[2] / hyp) * radpi;
    plocal_angles_final[1] = atan(delta_location[1] / delta_location[0]) * radpi;

    float distance = distance_3d(plocal.location, player.bone_matrix[6]);

    float fov = sqrt(powf(sin((plocal_angles[0] - plocal_angles_final[0] + plocal.aim_punch[0]) * pideg) * distance, 2.0) + powf(sin((plocal_angles[1] - plocal_angles_final[1] + plocal.aim_punch[1]) * pideg) * distance, 2.0));

    if (Aimbot::index == i) {
	
      PlayerInfo::l_players[Aimbot::index].fov_distance = fov;
	
      if (delta_location[0] >= 0.0f) {
	plocal_angles_final[1] += 180.0f;
      }

      if (fov > 90 && !Xutil::key_down(aim_display, config.aim.key)) {
	Aimbot::index = -1;
	continue;
      }
	
      while (plocal_angles_final[0] > 89)
	plocal_angles_final[0] -= 180;

      while (plocal_angles_final[0] < -89)
	plocal_angles_final[0] += 180;

      if (isNaN(plocal_angles_final[0]))
	plocal_angles_final[0] = 0;

      if (isNaN(plocal_angles_final[1]))
	plocal_angles_final[1] = 0;
	
      if (config.aim.recoil) {
	plocal_angles_final[0] -= (plocal.aim_punch[0]);
	plocal_angles_final[1] -= (plocal.aim_punch[1]);
      } 

    }

    if (Aimbot::index == i && Xutil::key_down(aim_display, config.aim.key)) {
      Memory::write(game_pid, Client::view_angles, &*plocal_angles_final, sizeof(float[2]));
    } else {
      if (fov <= 90 && fov < PlayerInfo::get_player(Aimbot::index).fov_distance) {
	Aimbot::index = i;
	PlayerInfo::l_players[Aimbot::index].fov_distance = fov;
      }
    }
  }
}
