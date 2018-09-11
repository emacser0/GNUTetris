#include <thread>
#include <ncursesw/ncurses.h>
#include "object_manager.hpp"
namespace ts=tetris;
inline void
ncurses_init() {
  initscr();
  keypad(stdscr,TRUE);
  timeout(1);
  noecho();
}
int
main(int __attribute__((unused)) argc,
     char __attribute__((unused)) **argv) {
  int time=500;
  int freq=200;
  unsigned int ch;
  auto obj_mgr=ObjectManager<ts::x_size,ts::y_size>();

  auto grid=*(obj_mgr.grid);
  auto displayer=*(obj_mgr.displayer);
  auto drawer=*(obj_mgr.drawer);
  auto collide_checker=*(obj_mgr.collide_checker);
  auto block_manager=*(obj_mgr.block_manager);
  auto block_stacker=*(obj_mgr.block_stacker);

  auto move_d=*(obj_mgr.alloc_block_point(0,1));
  auto move_nd=*(obj_mgr.alloc_block_point(0,-1));
  auto move_right=*(obj_mgr.alloc_block_point(1,0));
  auto move_left=*(obj_mgr.alloc_block_point(-1,0));
  ncurses_init();
  drawer <<= '@';
  for(;time--;) {
    if((ch=getch())=='a') {
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
    block_stacker >>= drawer;
    clear();
    ((block_manager >>= drawer) >>= displayer) >>= *stdscr;
    refresh();
    !drawer;
    block_stacker >>= drawer;
    move_d >>= block_manager;
    if(block_manager >>= collide_checker) {
      (move_nd >>= block_manager) >>= block_stacker;
      !block_manager;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(freq));
  }
  endwin();
  !obj_mgr;
}
