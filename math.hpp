#ifndef MATH_HPP
#define MATH_HPP

#include <cmath>

#include "client.hpp"
#include "memory.hpp"

inline static float vmatrix[4][4];
inline static bool world_to_screen(pid_t gamePid, const float vIn[3], float vOut[2]) {
  Memory::read(gamePid, Client::view_matrix, &vmatrix, sizeof(vmatrix));

  float w = vmatrix[3][0] * vIn[0] + vmatrix[3][1] * vIn[1] + vmatrix[3][2] * vIn[2] + vmatrix[3][3];

  if (w < 0.01f)
    return false;

  float vOutTmp[2];
  
  vOutTmp[0] = vmatrix[0][0] * vIn[0] + vmatrix[0][1] * vIn[1] + vmatrix[0][2] * vIn[2] + vmatrix[0][3];
  vOutTmp[1] = vmatrix[1][0] * vIn[0] + vmatrix[1][1] * vIn[1] + vmatrix[1][2] * vIn[2] + vmatrix[1][3];
  float invw = 1.0f / w;

  vOutTmp[0] *= invw;
  vOutTmp[1] *= invw;

  int width = 1920;
  int height = 1080;

  float x = width / 2.0f;
  float y = height / 2.0f;

  x += 0.5f * vOutTmp[0] * width + 0.5f;
  y -= 0.5f * vOutTmp[1] * height + 0.5f;
  
  vOut[0] = x * 100.f / 100.f;
  vOut[1] = y * 100.f / 100.f;

  return true;
}

inline static float distanceFormula3D(float locationOne[3], float locationTwo[3]) {
  return sqrt(((locationOne[1] - locationTwo[1])*(locationOne[1] - locationTwo[1])) + ((locationOne[0] - locationTwo[0])*(locationOne[0] - locationTwo[0])) + ((locationOne[2] - locationTwo[2])*(locationOne[2] - locationTwo[2])));
}

#endif
