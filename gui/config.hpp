struct Esp {
  bool master = true;
  bool box = true;
  bool health = true;
  bool skeleton = false;
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
