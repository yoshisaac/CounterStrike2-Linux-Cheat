struct Esp {
  bool master = true;

  bool box = true;
  int box_color[3] = {255, 0, 50};
  
  bool health = true;

  bool name = true;

  bool skeleton = false;
  int skeleton_color[3] = {255, 255, 255};
  
  bool snap_lines = false;
};

struct Aim {
  bool master = false;
  char key = 'c';
  bool recoil = true;
};

struct Misc {
  bool spin;
};

struct Config {
  Esp esp;
  Aim aim;
  Misc misc;
};

inline Config config;
