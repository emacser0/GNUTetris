#include "basic_system/basic_system.hpp"
#include "tetris/tetris.hpp"
namespace bs = basic_system;
namespace ts = tetris;
template <const int xsize,const int ysize>
class ObjectManager {
public:
  ObjectManager() {
    grid=new bs::
      Grid2D<xsize,ysize>();

    displayer=new bs::
      Displayer<xsize,ysize>(grid);

    drawer=new bs::
      Drawer<xsize,ysize>(grid);

    collide_checker=new ts::
      CollideChecker<xsize,ysize>(grid);

    block_manager=new ts::
      BlockManager<xsize,ysize>();

    block_stacker=new ts::
      BlockStacker<xsize,ysize>();
  }
  template <typename ...ARGS>
  ts::BlockPoint<xsize,ysize>*
  alloc_block_point(ARGS ...args) {
    auto block_point=new ts::
      BlockPoint<xsize,ysize>(args...);
    block_point_basket.push_back(block_point);
    return block_point;
  }

  ObjectManager& operator!() {
    delete grid;
    delete displayer;
    delete drawer;
    delete collide_checker;
    delete block_manager;
    delete block_stacker;
    for(auto i : block_point_basket) {
      delete i;
    }
    return *this;
  }
  bs::Grid2D<xsize,ysize> *grid;
  bs::Displayer<xsize,ysize> *displayer;
  bs::Drawer<xsize,ysize> *drawer;
  ts::CollideChecker<xsize,ysize> *collide_checker;
  ts::BlockManager<xsize,ysize> *block_manager;
  ts::BlockStacker<xsize,ysize> *block_stacker;
  std::vector<ts::BlockPoint<xsize,ysize>*> block_point_basket;
protected:
private:
};
