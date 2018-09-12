#include <thread>
#include "ncursesw.hpp"
#include "object_manager.hpp"

namespace ts=tetris;
namespace nc=ncursesw;
inline void
ncurses_init() {
  nc::initscr();
  nc::keypad(nc::stdscr,TRUE);
  nc::wtimeout(nc::stdscr,1);
  nc::noecho();
}
int
main(int __attribute__((unused)) argc,
     char __attribute__((unused)) **argv) {
  int time=500;
  int freq=200;
  unsigned int ch;
  auto obj_mgr=ObjectManager<ts::x_size,ts::y_size>();

  auto &grid=*(obj_mgr.grid);
  auto &grid_displayer=*(obj_mgr.displayer);
  auto &grid_drawer=*(obj_mgr.drawer);
  auto &collide_checker=*(obj_mgr.collide_checker);
  auto &block_manager=*(obj_mgr.block_manager);
  auto &block_stacker=*(obj_mgr.block_stacker);

  auto &move_down=*(obj_mgr.alloc_block_point(0,1));
  auto &move_up=*(obj_mgr.alloc_block_point(0,-1));
  auto &move_right=*(obj_mgr.alloc_block_point(1,0));
  auto &move_left=*(obj_mgr.alloc_block_point(-1,0));
  ncurses_init();
  grid_drawer <<= '@';
  for(;time--;) {
    if((ch=nc::wgetch(nc::stdscr))=='a') {
      block_manager.rotate_l();
      if(block_manager >>= collide_checker) {
        block_manager.rotate_r();
      }
    }
    else if(ch=='d') {
      block_manager.rotate_r();
      if(block_manager >>= collide_checker) {
        block_manager.rotate_l();
      }
    }
    else if(ch==KEY_RIGHT) {
      move_right >>= block_manager;
      if(block_manager >>= collide_checker) {
        move_left >>= block_manager;
      }
    }
    else if(ch==KEY_LEFT) {
      move_left >>= block_manager;
      if(block_manager >>= collide_checker) {
        move_right >>= block_manager;
      }
    }
    else if(ch==KEY_DOWN) {
      move_down >>= block_manager;
      if(block_manager >>= collide_checker) {
        move_up >>= block_manager;
      }
    }
    block_stacker >>= grid_drawer;
    nc::clear();
    ((block_manager >>= grid_drawer) >>= grid_displayer) >>= *nc::stdscr;
    nc::refresh();
    !grid_drawer;
    block_stacker >>= grid_drawer;
    move_down >>= block_manager;
    if(block_manager >>= collide_checker) {
      (move_up >>= block_manager) >>= block_stacker;
      !block_manager;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(freq));
  }
  nc::endwin();
  !obj_mgr;
}
