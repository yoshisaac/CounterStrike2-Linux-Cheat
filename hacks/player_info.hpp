#include <unistd.h>
#include <cstdint>

void players(pid_t game_pid);

namespace PlayerInfo {
  inline uintptr_t entity_list;

  class Player {
  public:
    int index;
    int health;
    float location[3];
    
    Player() {
      index = -1;
      health = -1;
      location[0] = 0;
      location[1] = 0;
      location[2] = 0;
    }

    Player(int index, int health, float location[3]) {
      this->index = index;
      this->health = health;

      this->location[0] = location[0]; 
      this->location[1] = location[1];
      this->location[2] = location[2];
    }
  };

  inline Player l_players[64];

  static Player get_player(int index) {
    if (index > 64) return Player();
    return l_players[index];
  }

  inline int i_local_player = -1;
  inline uintptr_t ptr_local_player;
  
  static Player get_local_player(void) {
    return l_players[i_local_player];
  }
}
