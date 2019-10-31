#ifndef TOWER_HH
#define TOWER_HH

#include "JetInfo.hh"

class Tower {
public:
  int et;
};

class SuperTower {
 public:
  int et;
  int peak_et;
  void init() {
    et = 0;
    peak_et = 0;
  }
  void set(SuperTower &supertower) {
    et = supertower.et;
    peak_et = supertower.peak_et;
  }
};
#endif
