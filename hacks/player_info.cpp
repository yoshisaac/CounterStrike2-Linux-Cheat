#include "player_info.hpp"

#include <unistd.h>
#include <cstdint>

#include "../memory.hpp"

void players(pid_t game_pid) {
  for (unsigned long i = 1; i < 64; ++i) {
    uintptr_t entry;
    //this shifting the index right nine bits is bullshit
    Memory::read(game_pid, PlayerInfo::entity_list + 0x08 * (i >> 9) + 0x10, &entry, sizeof(uintptr_t));
    if (!entry) continue;

    uintptr_t controller;
    //why the bitmask?
    Memory::read(game_pid, entry + 120 * (i & 0x1FF), &controller, sizeof(uintptr_t));
    if (!controller) continue;

    uintptr_t pawn;
    Memory::read(game_pid, controller + 0x7B4, &pawn, sizeof(uintptr_t));
    if (!pawn) continue;

    //idk anymore
    uintptr_t __pawn2__;
    Memory::read(game_pid, (PlayerInfo::entity_list + 0x10) + 8 * ((pawn & 0x7FFF) >> 9), &__pawn2__, sizeof(uintptr_t));

    uintptr_t player;
    Memory::read(game_pid, __pawn2__ + 120 * (pawn & 0x1FF), &player, sizeof(uintptr_t));
    if (!player) continue;

    uintptr_t local_player;
    Memory::read(game_pid, PlayerInfo::ptr_local_player, &local_player, sizeof(uintptr_t));

    if (player == local_player) { PlayerInfo::i_local_player = i-1; }
      
    int health = -1;
    Memory::read(game_pid, player + 0x4BC, &health, sizeof(int));

    float location[3];
    Memory::read(game_pid, player + 0xD40, &location, sizeof(float[3]));

    PlayerInfo::l_players[i-1] = PlayerInfo::Player(i-1, health, location);
  }
}
