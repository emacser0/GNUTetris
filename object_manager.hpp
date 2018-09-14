#include "GNUgbs/basic_system.hpp"
#include "tetris/tetris.hpp"
namespace bs = basic_system;
namespace ts = tetris;
template <const int xsize,const int ysize>
class ObjectManager {
public:
  ObjectManager() {
    main_grid=new bs::
      Grid2D<xsize,ysize>();

    hold_grid=new bs::
      Grid2D<4,4>();

    main_grid_displayer=new bs::
      GridDisplayer<xsize,ysize>(main_grid);

    hold_grid_displayer=new bs::
      GridDisplayer<4,4>(hold_grid);

    main_grid_drawer=new bs::
      GridDrawer<xsize,ysize>(main_grid);

    hold_grid_drawer=new bs::
      GridDrawer<4,4>(hold_grid);

    collide_checker=new ts::
      CollideChecker<xsize,ysize>(main_grid);

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
    delete main_grid;
    delete hold_grid;
    delete main_grid_displayer;
    delete hold_grid_displayer;
    delete main_grid_drawer;
    delete hold_grid_drawer;
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
  bs::Grid2D<xsize,ysize> *main_grid;
  bs::Grid2D<4,4> *hold_grid;
  bs::GridDisplayer<xsize,ysize> *main_grid_displayer;
  bs::GridDisplayer<4,4> *hold_grid_displayer;
  bs::GridDrawer<xsize,ysize> *main_grid_drawer;
  bs::GridDrawer<4,4> *hold_grid_drawer;
  ts::CollideChecker<xsize,ysize> *collide_checker;
  ts::BlockManager<xsize,ysize> *block_manager;
  ts::BlockStacker<xsize,ysize> *block_stacker;
  ts::ScoreChecker *score_checker;
  ts::BlockHolder *block_holder;
  std::vector<ts::BlockPoint<xsize,ysize>*> block_point_basket;
protected:
private:
};
