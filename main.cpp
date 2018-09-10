#include "basic_system/basic_system.hpp"
#include "tetris.hpp"
namespace bs = basic_system;
namespace ts = tetris;
int
main(int __attribute__((unused)) argc,
     char __attribute__((unused)) **argv) {
  auto grid=bs::Grid2D<ts::x_size,ts::y_size>();
  auto displayer = bs::Displayer<ts::x_size,ts::y_size>(&grid);
  auto drawer = bs::Drawer<ts::x_size,ts::y_size>(&grid);
  auto collide_checker=ts::
    CollideChecker<ts::x_size,ts::y_size>(&grid);
  auto block_manager=ts::
    BlockManager<ts::x_size,ts::y_size>();
  int time=100;
  std::array<int,2> move_d={0,1};
  int freq=100;
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
}
