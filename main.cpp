#include <unistd.h>
#include <stdio.h>
#include <thread>

#include "hacks/player_info.hpp"
#include "hacks/create_move.hpp"
#include "hacks/draw.hpp"

#include "gui/gui.hpp"

#include "process.hpp"
#include "memory.hpp"
#include "client.hpp"
#include "engine.hpp"
#include "xutil.hpp"

int main(int argc, char *argv[]) {
  if (getuid()) { //check if root
    printf("Please run as root\n");
    return 1;
  }
  pthread_setname_np(pthread_self(), "main_thread");
  
  pid_t game_pid = Process::name_to_pid("cs2");
  if (!game_pid) {
    printf("Game not open\n");
    return 1;
  }
  
  printf("Game pid: %d\n", game_pid);  
  
  /* X11 initiation */
  Display* display = XOpenDisplay(NULL);
  if (!display) return 1;

  Display* draw_display = XOpenDisplay(NULL);

  Display* move_display = XOpenDisplay(NULL);
  
  int screen = DefaultScreen(display);

  int shape_event_base;
  int shape_error_base;

  if (!XShapeQueryExtension(display, &shape_event_base, &shape_error_base)) {
    printf("cs-source-hack: NO shape extension in your system !\n");
    return 1;
  }

  Window root = DefaultRootWindow(display);

  Visual* visual = DefaultVisual(display, screen);

  XVisualInfo vinfo;
  XMatchVisualInfo(display, DefaultScreen(display), 32, TrueColor, &vinfo);
  Colormap colormap = XCreateColormap(display, DefaultRootWindow(display), vinfo.visual, AllocNone);

  Xutil::Color bgcolor = Xutil::xcolor_from_rgba(0, 0, 0, 0, display);
  
#define BASIC_EVENT_MASK (StructureNotifyMask|ExposureMask|PropertyChangeMask|EnterWindowMask|LeaveWindowMask|KeyPressMask|KeyReleaseMask|KeymapStateMask)
#define NOT_PROPAGATE_MASK (KeyPressMask|KeyReleaseMask|ButtonPressMask|ButtonReleaseMask|PointerMotionMask|ButtonMotionMask)

  XSetWindowAttributes attr;
  attr.background_pixmap = None;
  attr.background_pixel = bgcolor;
  attr.border_pixel=0;
  attr.win_gravity=NorthWestGravity;
  attr.bit_gravity=ForgetGravity;
  attr.save_under=1;
  attr.event_mask=BASIC_EVENT_MASK;
  attr.do_not_propagate_mask=NOT_PROPAGATE_MASK;
  attr.override_redirect=1; // OpenGL > 0
  attr.colormap = colormap;

  unsigned long mask = CWColormap | CWBorderPixel | CWBackPixel | CWEventMask | CWWinGravity|CWBitGravity | CWSaveUnder | CWDontPropagate | CWOverrideRedirect;

  //this assumes the game is full srceen of some sort. Why wouldn't it be? Play the damn game correctly!
  int tmp_window_resolution[2];
  Xutil::display_resolution(tmp_window_resolution);
  Engine::window_width = tmp_window_resolution[0];
  Engine::window_height = tmp_window_resolution[1];
  
  Window window = XCreateWindow(display, root, 0, 0, Engine::window_width, Engine::window_height, 0, vinfo.depth, InputOutput, vinfo.visual, mask, &attr);
  
  XShapeCombineMask(display, window, ShapeInput, 0, 0, None, ShapeSet);
    
  XShapeSelectInput(display, window, ShapeNotifyMask);
  
  XserverRegion region = XFixesCreateRegion(display, NULL, 0);
  XFixesSetWindowShapeRegion(display, window, ShapeInput, 0, 0, region);
  XFixesDestroyRegion(display, region);

  XdbeBackBuffer back_buffer = XdbeAllocateBackBufferName(display, window, 0);
  
  XMapWindow(display, window);

  Draw::black = Xutil::xcolor_from_rgb(0, 0, 0, display);
  Draw::green = Xutil::xcolor_from_rgb(0, 255, 50, draw_display);
  Draw::red = Xutil::xcolor_from_rgb(255, 0, 50, draw_display);
  Draw::white = Xutil::xcolor_from_rgb(255, 255, 255, draw_display);
  Draw::cyan = Xutil::xcolor_from_rgb(0, 255, 240, draw_display);
  Draw::yellow = Xutil::xcolor_from_rgb(255, 255, 0, draw_display);
  Draw::orange = Xutil::xcolor_from_rgb(255, 170, 0, draw_display); 
  
  Draw::shadowfont = XLoadQueryFont(display, "fixed");
  if (!Draw::shadowfont) {
    printf("Could not Query Font \"fixed\"\n");
    return 1;
  }
  
  Draw::font = XLoadQueryFont(display, "6x13");
  if (!Draw::font) {
    printf("Could not Query Font \"6x13\"\n");
    return 1;
  }
  /* X11 initiation end */

  const uintptr_t client_address = Memory::module_base_address(game_pid, "libclient.so");
  printf("client: %p\n", client_address);
  
  const uintptr_t engine_address = Memory::module_base_address(game_pid, "libengine2.so");
  printf("engine: %p\n", engine_address);
  
  const uintptr_t resource_offset = Memory::interface_offset(game_pid, engine_address, "GameResourceServiceClientV0");
  printf("resource: %p\n", resource_offset);

  Memory::read(game_pid, resource_offset + 0x50, &PlayerInfo::entity_list, sizeof(uintptr_t));

  PlayerInfo::ptr_local_player = Memory::scan_pattern(game_pid,
						      {0x48, 0x83, 0x3D, 0x00,  0x00,  0x00,  0x00,  0x00, 0x0F, 0x95, 0xC0, 0xC3},
						      {true, true, true, false, false, false, false, true, true, true, true, true}, 12,
						      client_address);
  if (!PlayerInfo::ptr_local_player) { printf("Couldn't find local player\n"); return 1; }
  PlayerInfo::ptr_local_player = Memory::relative_address(game_pid, PlayerInfo::ptr_local_player, 0x3, 0x8);
  
  Client::view_matrix = Memory::scan_pattern(game_pid,
					     {0x48, 0x8D, 0x05, 0x00, 0x00, 0x00, 0x00, 0x4C, 0x8D, 0x05, 0x00, 0x00, 0x00, 0x00, 0x48, 0x8D, 0x0D},
					     {
					       true,
					       true,
					       true,
					       false,
					       false,
					       false,
					       false,
					       true,
					       true,
					       true,
					       false,
					       false,
					       false,
					       false, 
					       true,
					       true,
					       true,
					     },
					     17, client_address);
  if (!Client::view_matrix) { printf("Couldn't find view matrix\n"); return 1; }
  Client::view_matrix = Memory::relative_address(game_pid, Client::view_matrix + 0x7, 0x3, 0x7);
  
  Client::view_angles = client_address + 0x39905E0; //this would be better as a pattern
  printf("view_angles: %p\n", Client::view_angles);

  Client::force_attack = client_address + 0x39904A0; //this too!
  printf("force_attack: %p\n", Client::force_attack);

  //start all other thread loops
  std::thread gui_thread(gui, argc, argv);
  pthread_setname_np(gui_thread.native_handle(), "gui_thread");
  
  std::thread draw_thread(draw, game_pid, back_buffer, draw_display, window);
  pthread_setname_np(draw_thread.native_handle(), "draw_thread");

  std::thread move_thread(create_move, game_pid, move_display);
  pthread_setname_np(move_thread.native_handle(), "move_thread");

  //main_thead's loop
  for (;;) {
    players(game_pid);
    usleep(1000*1000/250);
  }
}
