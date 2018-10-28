#include <thread>
#include "GNUgbs/ncursesw.hpp"
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
  int freq=500;
  unsigned int ch;
  auto obj_mgr=ObjectManager<ts::x_size,ts::y_size>();

  auto &main_grid=*(obj_mgr.main_grid);
  auto &hold_grid=*(obj_mgr.hold_grid);
  auto &main_grid_displayer=*(obj_mgr.main_grid_displayer);
  auto &hold_grid_displayer=*(obj_mgr.hold_grid_displayer);
  auto &main_grid_drawer=*(obj_mgr.main_grid_drawer);
  auto &hold_grid_drawer=*(obj_mgr.hold_grid_drawer);
  auto &collide_checker=*(obj_mgr.collide_checker);
  auto &block_manager=*(obj_mgr.block_manager);
  auto &block_stacker=*(obj_mgr.block_stacker);
  auto &score_checker=*(obj_mgr.score_checker);
  auto &block_holder=*(obj_mgr.block_holder);

  auto &move_down=*(obj_mgr.alloc_block_point(0,1));
  auto &move_up=*(obj_mgr.alloc_block_point(0,-1));
  auto &move_right=*(obj_mgr.alloc_block_point(1,0));
  auto &move_left=*(obj_mgr.alloc_block_point(-1,0));

  auto *hold_win = nc::newwin(10, 10, 10, 20);
  auto refresh_grid=[&]() {
    nc::wclear(nc::stdscr);
    nc::wclear(hold_win);
    ((block_manager >>= main_grid_drawer) >>= main_grid_displayer) >>= nc::stdscr;
    ((block_holder >>= hold_grid_drawer) >>= hold_grid_displayer) >>= hold_win;
    nc::wrefresh(nc::stdscr);
    nc::wrefresh(hold_win);
    !main_grid_drawer;
    !hold_grid_drawer;
    block_stacker >>= main_grid_drawer;
  };
  ncurses_init();
  main_grid_drawer <<= '@';
  while(true) {
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
      break;
    case 0x20:
      while(!(block_manager >>= collide_checker)) {
        move_down >>= block_manager;
     }
      while(block_manager >>= collide_checker) {
        move_up >>= block_manager;
      }
      break;
    case 's':
      block_manager >>= block_holder;
    }
    nc::flushinp();
    block_stacker >>= main_grid_drawer;
    refresh_grid();
    move_down >>= block_manager;
    if(block_manager >>= collide_checker) {
      ((move_up >>= block_manager) >>=  block_stacker);
      block_stacker >>= score_checker;
      !block_manager;
      !block_holder;
      if(block_manager >>= collide_checker) {
        break;
      }
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(freq));
  }
  nc::delwin(hold_win);
  nc::endwin();
  !obj_mgr;
}
