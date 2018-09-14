#ifndef BLOCK_STACKER
#define BLOCK_STACKER
#include <vector>
#include <chrono>
#include <thread>
#include <fstream>
#include "../GNUgbs/basic_system.hpp"
#include "env.hpp"
#include "score_checker.hpp"
namespace tetris {
  template <const int xsize,const int ysize>
  class BlockStacker {
  public:
    void add_block(unsigned int block, int x, int y) {
      int calculated=std::pow(16,3);
      int table[4]={0x8,0x4,0x2,0x1};
      for(int i=0;i<4;i++,calculated/=16) {
        for(int j=0;j<4;j++) {
          if((block&(table[j]*calculated))) {
            _blocks.push_back({x+j,y+i});
          }
        }
      }
    }
    void
    check_lines() {
      _scorecache=0;
      _linecache={};
      _lineclearcache={};
      for(auto i : _blocks) {
        _linecache.at(i[1])++;
      }
      for(int i=1;i<ysize-1;i++) {
        if(_linecache.at(i)==xsize-2) {
          _scorecache++;
          _lineclearcache.push_back(i);
        }
      }
      clear_lines();
      pull_down_blocks();

    }
    basic_system::GridDrawer<xsize,ysize>&
    operator>>=(basic_system::GridDrawer<xsize,ysize>& drawer) {
      for(auto i : _blocks) {
        drawer.draw_block(0x8000, i[0],i[1]);
      }
      return drawer;
    }
    ScoreChecker&
    operator >>=(ScoreChecker &checker) {
      checker.refresh_score(_scorecache);
      _scorecache=0;
      return checker;
    }
  protected:
    std::vector<std::array<int,2>> _blocks;
  private:
    int _scorecache;
    std::array<int,ysize> _linecache;
    std::vector<int> _lineclearcache;
    void
    pull_down_blocks() {
      for(auto &i : _blocks) {
        for(auto j : _lineclearcache) {
          if(i.at(1)<j) {
            i.at(1)+=_lineclearcache.size();
          }
          break;
        }
      }
    }
    void
    clear_lines() {
      for(auto i=_blocks.rbegin();i!=_blocks.rend();i++) {
        for(auto j : _lineclearcache) {
          if((*i).at(1)==j) {
            _blocks.erase(--(i.base()));
          }
        }
      }
    }
  };
}
#endif
