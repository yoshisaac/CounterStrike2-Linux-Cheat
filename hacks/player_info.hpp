#include <unistd.h>
#include <cstdint>
#include <string>

#include "../vector.hpp"

void players(pid_t game_pid);

namespace PlayerInfo {
  inline uintptr_t entity_list;

  class Player {
  public:
    // player structure
    int index;
    int health;
    int team;
    int fov;
    bool crouched;
    bool on_ground;
    bool spotted;
    float fov_multiplier;
    int fov_desired;
    float height;
    Euler aim_punch;
    Vector3 location;
    Vector3 bone_matrix[80];
    std::string name;
    std::string weapon_name;

    // aimbot related
    float fov_distance;
    
    Player() {
      index = -1;
      health = -1;
      team = -1;
      crouched = false;
      spotted = false;
      fov_distance = 999999;
      location = {0, 0, 0};
      name = "< invalid >";
      weapon_name = "< invalid >";
    }

    Player(int index, int health, int team, int fov,
	   bool crouched, bool on_ground, bool spotted,
	   float fov_multiplier, int fov_desired, float height,
	   float aim_punch[2], float location[3], float bone_matrix[80][3],
	   float fov_distance, std::string name, std::string weapon_name) {
      this->index = index;
      this->health = health;
      this->team = team;
      this->fov = fov;
      this->crouched = crouched;
      this->on_ground = on_ground;
      this->spotted = spotted;
      this->fov_multiplier = fov_multiplier;
      this->fov_desired = fov_desired;
      this->height = height;
      this->aim_punch.pitch = aim_punch[0];
      this->aim_punch.yaw = aim_punch[1];
      
      this->location = location;

      this->name = name;

      this->weapon_name = weapon_name;
      
      for (int i = 0; i < 80; ++i) {
	this->bone_matrix[i].x = bone_matrix[i][0];
	this->bone_matrix[i].y = bone_matrix[i][1];
	this->bone_matrix[i].z = bone_matrix[i][2];	
      }

      this->fov_distance = fov_distance;
    }
  };

  inline Player l_players[64];

  static Player get_player(unsigned int index) {
    if (index > 64) return Player();
    return l_players[index];
  }

  inline int i_local_player;
  inline uintptr_t ptr_local_player;
  
  static Player get_local_player(void) {
    return l_players[i_local_player];
  }
}
