#include <thread>
#include "object_manager.hpp"
namespace ts=tetris;
int
main(int __attribute__((unused)) argc,
     char __attribute__((unused)) **argv) {
  int time=100;
  int freq=100;

  auto obj_mgr=ObjectManager<ts::x_size,ts::y_size>();

  auto grid=*(obj_mgr.grid);
  auto displayer=*(obj_mgr.displayer);
  auto drawer=*(obj_mgr.drawer);
  auto collide_checker=*(obj_mgr.collide_checker);
  auto block_manager=*(obj_mgr.block_manager);
  auto block_stacker=*(obj_mgr.block_stacker);

  auto move_d=*(obj_mgr.alloc_block_point(0,1));
  auto move_nd=*(obj_mgr.alloc_block_point(0,-1));
  !block_manager;
  drawer <<= '@';
  for(;time--;) {
    block_stacker >>= drawer;
    ((block_manager >>= drawer) >>= displayer) >>= std::cout;
    !drawer;
    block_stacker >>= drawer;
    move_d >>= block_manager;
    if(block_manager >>= collide_checker) {
      (move_nd >>= block_manager) >>= block_stacker;
      !block_manager;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(freq));
  }
  !obj_mgr;
}
