#include <unistd.h>
#include <cstdint>
#include <stdio.h>

void players(pid_t game_pid);

namespace PlayerInfo {
  inline uintptr_t entity_list;

  class Player {
  public:
    int index;
    int health;
    int team;
    bool crouched;
    bool spotted;
    float fov_multiplier;
    float height;
    float aim_punch[2];
    float location[3];
    float bone_matrix[80][3];

    float fov_distance;
    
    Player() {
      index = -1;
      health = -1;
      team = -1;
      crouched = false;
      spotted = false;
      fov_distance = 999999;
      location[0] = 0;
      location[1] = 0;
      location[2] = 0;
    }

    Player(int index, int health, int team,
	   bool crouched, bool spotted, float fov_multiplier, float height,
	   float aim_punch[2], float location[3], float bone_matrix[80][3],
	   float fov_distance) {
      this->index = index;
      this->health = health;
      this->team = team;
      this->crouched = crouched;
      this->spotted = spotted;
      this->fov_multiplier = fov_multiplier;
      this->height = height;
      this->aim_punch[0] = aim_punch[0];
      this->aim_punch[1] = aim_punch[1];
      
      this->location[0] = location[0]; 
      this->location[1] = location[1];
      this->location[2] = location[2];

      for (int i = 0; i < 80; ++i) {
	this->bone_matrix[i][0] = bone_matrix[i][0];
	this->bone_matrix[i][1] = bone_matrix[i][1];
	this->bone_matrix[i][2] = bone_matrix[i][2];	
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
