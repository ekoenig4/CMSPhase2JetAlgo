#ifndef TOWER3X3_HH
#define TOWER3X3_HH

#include "JetInfo.hh"

class Tower3x3 {
 public:
  int iphi;
  int ieta;
  int et;
  int highest_et;

  int njets;
  JetInfo jetlist[M_JET_OVR];

  Tower3x3() {
    iphi = 0;
    ieta = 0;
    et = 0;
    highest_et = 0;
    njets = 0;
  }
  // void setSeed(int iphi,int ieta,int et) {
  // }
  void addTower(int et) {
    if ( et > highest_et ) highest_et = et;
    this->et += et;
  }
  // void addJet(JetInfo & jet) {
  //   jetlist[njets] = jet;
  //   njets++;
  // }
};
#endif
