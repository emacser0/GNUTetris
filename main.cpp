#include <thread>
#include "object_manager.hpp"

int
main(int __attribute__((unused)) argc,
     char __attribute__((unused)) **argv) {
  int time=100;
  std::array<int,2> move_d={0,1};
  int freq=100;

  auto obj_mgr=ObjectManager<ts::x_size,ts::y_size>();

  auto grid=*(obj_mgr.grid);
  auto displayer=*(obj_mgr.displayer);
  auto drawer=*(obj_mgr.drawer);
  auto collide_checker=*(obj_mgr.collide_checker);
  auto block_manager=*(obj_mgr.block_manager);

  !block_manager;
  drawer <<= '@';
  while(time--) {
    ((block_manager >>= drawer) >>= displayer) >>= std::cout;
    !drawer;
    block_manager <<= move_d;
    if(block_manager >>= collide_checker) {
      !block_manager;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(freq));
  }
  !obj_mgr;
}
