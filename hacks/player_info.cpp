#include "player_info.hpp"

#include <unistd.h>
#include <cstdint>

#include "../memory.hpp"

void players(pid_t game_pid) {
  for (unsigned long i = 1; i <= 64; ++i) {
    uintptr_t entry;
    Memory::read(game_pid, PlayerInfo::entity_list + 0x10, &entry, sizeof(uintptr_t));
    if (!entry) {
      PlayerInfo::l_players[i-1] = PlayerInfo::Player();
      continue;
    }
    
    uintptr_t controller;
    Memory::read(game_pid, entry + 120 * (i & 0x1FF), &controller, sizeof(uintptr_t));
    if (!controller) {
      PlayerInfo::l_players[i-1] = PlayerInfo::Player();
      continue;
    }

    uintptr_t pawn;
    Memory::read(game_pid, controller + 0x7B4, &pawn, sizeof(uintptr_t));
    if (!pawn) {
      PlayerInfo::l_players[i-1] = PlayerInfo::Player();
      continue;
    }

    //idk anymore
    uintptr_t __pawn2__;
    Memory::read(game_pid, (PlayerInfo::entity_list + 0x10) + 8 * ((pawn & 0x7FFF) >> 9), &__pawn2__, sizeof(uintptr_t));
    if (!__pawn2__) {
      PlayerInfo::l_players[i-1] = PlayerInfo::Player();
      continue;
    }

    uintptr_t player;
    Memory::read(game_pid, __pawn2__ + 120 * (pawn & 0x1FF), &player, sizeof(uintptr_t));
    if (!player) {
      PlayerInfo::l_players[i-1] = PlayerInfo::Player();
      continue;
    }

    uintptr_t game_scene;
    Memory::read(game_pid, player + 0x4A0, &game_scene, sizeof(uintptr_t));
    if (!game_scene) {
      PlayerInfo::l_players[i-1] = PlayerInfo::Player();
      continue;
    }
    
    uintptr_t local_player;
    Memory::read(game_pid, PlayerInfo::ptr_local_player, &local_player, sizeof(uintptr_t));

    if (player == local_player) { PlayerInfo::i_local_player = i-1; }

    uintptr_t bone_matrix_ptr;
    float bone_matrix[80][3];
    Memory::read(game_pid, game_scene + 0x170 + 0x80, &bone_matrix_ptr, sizeof(uintptr_t));
    for (int i = 0; bone_matrix_ptr != NULL && i < 80; ++i) {
      Memory::read(game_pid, bone_matrix_ptr + i * 32, &bone_matrix[i][0], sizeof(float[3]));
    }

    uintptr_t aim_punch_length;
    Memory::read(game_pid, player + 0x1550, &aim_punch_length, sizeof(uintptr_t));

    uintptr_t aim_punch_data_address;
    Memory::read(game_pid, player + 0x1550 + 0x8, &aim_punch_data_address, sizeof(uintptr_t));

    float aim_punch[2];
    Memory::read(game_pid, aim_punch_data_address + (aim_punch_length -1) * 12, &aim_punch, sizeof(float[2]));
    aim_punch[0] *= 2;
    aim_punch[1] *= 2;
    
    if (aim_punch_length < 1) {
      aim_punch[0] = 0;
      aim_punch[1] = 0;
    }
    
    unsigned char team;
    Memory::read(game_pid, player + 0x55B, &team, sizeof(unsigned char));

    int health = -1;
    Memory::read(game_pid, player + 0x4BC, &health, sizeof(int));

    unsigned char flags2;
    Memory::read(game_pid, player + 0x564, &flags2, sizeof(unsigned char));
    bool crouched = flags2 & (1<<1);

    float height;
    Memory::read(game_pid, player + 0xC40, &height, sizeof(float));
    
    float location[3];
    Memory::read(game_pid, player + 0xD40, &location, sizeof(float[3]));

    unsigned long spotted_mask;
    Memory::read(game_pid, player + 0x328C, &spotted_mask, sizeof(unsigned long));
    bool spotted = spotted_mask & (1 << PlayerInfo::i_local_player);
    
    PlayerInfo::l_players[i-1] = PlayerInfo::Player(i-1, health, team,
						    crouched, spotted,
						    height,
						    aim_punch, location, bone_matrix,
						    PlayerInfo::l_players[i-1].fov_distance);
  }
}
