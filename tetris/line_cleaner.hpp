#ifndef LINE_CLEANER
#define LINE_CLEANER
#include "../basic_system/basic_system.hpp"
#include <set>
#include "env.hpp"
#include "score_checker.hpp"
namespace tetris {
  template <const int xsize,const int ysize>
  class LineCleaner {
  public:
    LineCleaner(basic_system::Grid2D<xsize,ysize> &grid) {
      _grid=grid;
      _scorecache=0;
    }
    void
    clear_lines(std::set<std::array<int,2>> &bstack) {
      //_scorecache++;
    }
    ScoreChecker&
    operator >>=(ScoreChecker &checker) {
      checker.refresh_score(_scorecache);
      _scorecache=0;
      return checker;
    }
  protected:
    basic_system::Grid2D<xsize,ysize> *_grid;
    int _scorecache;
  private:
  };
}
#endif
