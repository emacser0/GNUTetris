#include "basic_system/basic_system.hpp"
#include "tetris/tetris.hpp"
namespace bs = basic_system;
namespace ts = tetris;
template <const int xsize,const int ysize>
class ObjectManager {
public:
  ObjectManager() {
    grid=new bs::Grid2D<ts::x_size,ts::y_size>();
    displayer=new bs::Displayer<ts::x_size,ts::y_size>(grid);
    drawer=new bs::Drawer<ts::x_size,ts::y_size>(grid);
    collide_checker=new ts::
      CollideChecker<ts::x_size,ts::y_size>(grid);
    block_manager=new ts::
      BlockManager<ts::x_size,ts::y_size>();
  }
  ObjectManager& operator!() {
    delete grid;
    delete displayer;
    delete drawer;
    delete collide_checker;
    delete block_manager;
    return *this;
  }
  bs::Grid2D<xsize,ysize> *grid;
  bs::Displayer<xsize,ysize> *displayer;
  bs::Drawer<xsize,ysize> *drawer;
  ts::CollideChecker<xsize,ysize> *collide_checker;
  ts::BlockManager<xsize,ysize> *block_manager;
protected:
private:
};
