#ifndef VECTOR_HPP
#define VECTOR_HPP

struct Vector3 {
  float x, y, z;
  
  void operator=(const Vector3 vec) {
    x = vec.x;
    y = vec.y;
    z = vec.z;
  }

  void operator=(const float vec[3]) {
    x = vec[0];
    y = vec[1];
    z = vec[2];
  }

  float dot(const Vector3 vec) {
    return x * vec.x + y * vec.y + z * vec.z;
  }
};

struct Euler {
  float pitch, yaw, roll = 0;
};

#endif
