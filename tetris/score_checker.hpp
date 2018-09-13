#ifndef SCORE_CHECKER
#define SCORE_CHECKER
#include "../basic_system/basic_system.hpp"
#include "env.hpp"
namespace tetris {
  class ScoreChecker {
  public:
    ScoreChecker() {
      _score=0;
    }
    ~ScoreChecker() {

    }
    void
    refresh_score(int score) {
      _score+=score;
    }
  protected:
    int _score;
  private:
  };
}
#endif
