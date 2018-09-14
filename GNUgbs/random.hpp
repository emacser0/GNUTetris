#ifndef BASIC_SYSTEM_RANDOM
#define BASIC_SYSTEM_RANDOM
#include <random>
#include <chrono>
#include <algorithm>
#include <functional>
namespace basic_system::random {
  template <typename T,const int s,const int e>
    class mt_gen {
    public:
      mt_gen() {
        generator =
          std::bind(std::uniform_int_distribution<T>(s,e),
                    std::mt19937
                    (std::chrono::
                     high_resolution_clock::
                     now().time_since_epoch().
                     count()));
      };
      inline T& operator>>=(T &v) {
        v=generator();
        return v;
      }
      std::function<int ()> generator;
    protected:
    private:
    };
    template<int len>
    class random_sequence {
    public:
      random_sequence() {
        for(int i=0;i<len;i++) {
          _seq[i]=i;
        }
        generator_r=mt_gen<int,0,len-1>();
        shuffle_seq();
        index=0;
      }
      void
      shuffle_seq() {
        int t,i_rand;
        for(int i=0;i<len;i++) {
          generator_r >>= i_rand;
          t=_seq[i];
          _seq[i]=_seq[i_rand];
          _seq[i_rand]=t;
        }
      }
      void
      shuffle_seq_std() {
        std::random_shuffle(_seq.begin(),_seq.end());
      }
      inline int& operator>>=(int &v) {
        if(index==7) {
          shuffle_seq();
          index=0;
        }
        v = _seq[index++];
        return v;
      }
    protected:
      mt_gen<int,0,len-1> generator_r;
      std::array<int,len> _seq;
      int index;
    private:
    };
}
#endif
