#ifndef MATH_HPP
#define MATH_HPP

#include <cmath>

#include "client.hpp"
#include "engine.hpp"

#include "memory.hpp"
#include "vector.hpp"

//180/pi
#define radpi 57.295779513082f

//pi/180
#define pideg 0.017453293f

#define isNaN(n) n != n

inline static float distance_3d(Vector3 location_one, Vector3 location_two) {
  return sqrt(((location_one.y - location_two.y)*(location_one.y - location_two.y)) +
	      ((location_one.x - location_two.x)*(location_one.x - location_two.x)) +
	      ((location_one.z - location_two.z)*(location_one.z - location_two.z)));
}
 
inline static float vmatrix[4][4];
inline static bool world_to_screen(pid_t gamePid, const Vector3 vIn, float vOut[2], bool ignore_oof=true) {
  Memory::read(gamePid, Client::view_matrix, &vmatrix, sizeof(vmatrix));

  float w = vmatrix[3][0] * vIn.x + vmatrix[3][1] * vIn.y + vmatrix[3][2] * vIn.z + vmatrix[3][3];

  if (w < 0.1f) return false;

  float vOutTmp[2];
  
  vOutTmp[0] = vmatrix[0][0] * vIn.x + vmatrix[0][1] * vIn.y + vmatrix[0][2] * vIn.z + vmatrix[0][3];
  vOutTmp[1] = vmatrix[1][0] * vIn.x + vmatrix[1][1] * vIn.y + vmatrix[1][2] * vIn.z + vmatrix[1][3];
  float invw = 1.0f / w;

  vOutTmp[0] *= invw;
  vOutTmp[1] *= invw;

  float x = Engine::window_width / 2.0f;
  float y = Engine::window_height / 2.0f;

  x += 0.5f * vOutTmp[0] * Engine::window_width + 0.5f;
  y -= 0.5f * vOutTmp[1] * Engine::window_height + 0.5f;
  
  vOut[0] = x * 100.f / 100.f;
  vOut[1] = y * 100.f / 100.f;

  if (ignore_oof == true) {
    if (vOut[0] > Engine::window_width*2 || vOut[1] > Engine::window_height*2 || vOut[0] < -Engine::window_width || vOut[1] < -Engine::window_height) return false;
  }
  
  return true;
}

#endif
