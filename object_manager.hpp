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

    grid_displayer=new bs::
      GridDisplayer<xsize,ysize>(grid);

    grid_drawer=new bs::
      GridDrawer<xsize,ysize>(grid);

    collide_checker=new ts::
      CollideChecker<xsize,ysize>(grid);

    block_manager=new ts::
      BlockManager<xsize,ysize>();

    block_stacker=new ts::
      BlockStacker<xsize,ysize>();

    score_checker=new ts::
      ScoreChecker();

    block_holder=new ts::
      BlockHolder();
  }
  ~ObjectManager() {
    !(*this);
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
    delete grid_displayer;
    delete grid_drawer;
    delete collide_checker;
    delete block_manager;
    delete block_stacker;
    delete score_checker;
    delete block_holder;
    for(auto i : block_point_basket) {
      delete i;
    }
    return *this;
  }
  bs::Grid2D<xsize,ysize> *grid;
  bs::GridDisplayer<xsize,ysize> *grid_displayer;
  bs::GridDrawer<xsize,ysize> *grid_drawer;
  ts::CollideChecker<xsize,ysize> *collide_checker;
  ts::BlockManager<xsize,ysize> *block_manager;
  ts::BlockStacker<xsize,ysize> *block_stacker;
  ts::ScoreChecker *score_checker;
  ts::BlockHolder *block_holder;
  std::vector<ts::BlockPoint<xsize,ysize>*> block_point_basket;
protected:
private:
};
