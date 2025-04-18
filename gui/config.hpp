struct Esp {
  bool master = true;

  bool ignore_team = true;
  
  bool spotted = false;

  bool box = true;
  int box_color[3] = {255, 0, 50};
  
  bool health_bar = true;
  bool health_text = false;
  
  bool name = true;

  bool skeleton = false;
  int skeleton_color[3] = {255, 255, 255};

  bool head_dot = false;
  int head_dot_color[3] = {255, 255, 255};
  
  bool snap_lines = false;
  int snap_lines_color[3] = {0, 255, 50};
};

struct Visuals {
  bool crosshair = false;
  int crosshair_color[3] = {0, 255, 50};

  bool visualize_recoil = true;
  bool sniper_only = true;
};

struct Aim {
  bool master = false;

  bool ignore_team = true;
  bool spotted = false;

  float fov = 90;
  bool show_fov = false;

  char key = 'c';

  bool recoil = true;

  bool auto_shoot = false;
};

struct Config {
  Esp esp;
  Visuals visuals;
  Aim aim;
};

inline Config config;
