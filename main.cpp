#include <thread>
#include "ncursesw.hpp"
#include "object_manager.hpp"

namespace ts=tetris;
namespace nc=ncursesw;
inline void
ncurses_init() {
  nc::initscr();
  nc::keypad(nc::stdscr,TRUE);
  nc::nodelay(nc::stdscr,TRUE);
  nc::noecho();
}
int
main(int __attribute__((unused)) argc,
     char __attribute__((unused)) **argv) {
  int time=500;
  int freq=300;
  unsigned int ch;
  auto obj_mgr=ObjectManager<ts::x_size,ts::y_size>();

  auto &grid=*(obj_mgr.grid);
  auto &grid_displayer=*(obj_mgr.grid_displayer);
  auto &grid_drawer=*(obj_mgr.grid_drawer);
  auto &collide_checker=*(obj_mgr.collide_checker);
  auto &block_manager=*(obj_mgr.block_manager);
  auto &block_stacker=*(obj_mgr.block_stacker);
  auto &score_checker=*(obj_mgr.score_checker);
  auto &block_holder=*(obj_mgr.block_holder);

  auto &move_down=*(obj_mgr.alloc_block_point(0,1));
  auto &move_up=*(obj_mgr.alloc_block_point(0,-1));
  auto &move_right=*(obj_mgr.alloc_block_point(1,0));
  auto &move_left=*(obj_mgr.alloc_block_point(-1,0));
  auto refresh_grid=[&]() {
    nc::clear();
    ((block_manager >>= grid_drawer) >>= grid_displayer) >>= *nc::stdscr;
    nc::refresh();
    !grid_drawer;
    block_stacker >>= grid_drawer;
  };
  ncurses_init();
  grid_drawer <<= '@';
  for(;time--;) {
    ch=nc::wgetch(nc::stdscr);
    switch(ch) {
    case 'a':
      block_manager.rotate_l();
      if(block_manager >>= collide_checker) {
        block_manager.rotate_r();
      }
      break;
    case 'd':
      block_manager.rotate_r();
      if(block_manager >>= collide_checker) {
        block_manager.rotate_l();
      }
      break;
    case KEY_RIGHT:
      move_right >>= block_manager;
      if(block_manager >>= collide_checker) {
        move_left >>= block_manager;
      }
      break;
    case KEY_LEFT:
      move_left >>= block_manager;
      if(block_manager >>= collide_checker) {
        move_right >>= block_manager;
      }
      break;
    case KEY_DOWN:
      move_down >>= block_manager;
      if(block_manager >>= collide_checker) {
        move_up >>= block_manager;
      }
      refresh_grid();
      break;
    case 0x20:
      while(!(block_manager >>= collide_checker)) {
        move_down >>= block_manager;
      }
      while(block_manager >>= collide_checker) {
        move_up >>= block_manager;
      }
      break;
    }
    nc::flushinp();
    block_stacker >>= grid_drawer;
    refresh_grid();
    move_down >>= block_manager;
    if(block_manager >>= collide_checker) {
      ((move_up >>= block_manager) >>=  block_stacker);
      block_stacker >>= score_checker;
      !block_manager;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(freq));
  }
  nc::endwin();
  !obj_mgr;
}
