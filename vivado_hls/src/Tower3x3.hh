#ifndef TOWER3X3_HH
#define TOWER3X3_HH

#include "JetInfo.hh"

class Tower3x3 {
 public:
  int iphi;
  int ieta;
  int ecal_et;
  int highest_ecal_et;

  int njets;
  JetInfo jetlist[M_JET_OVR];

  Tower3x3() {
    setSeed(0,0,0);
  }
  void setSeed(int iphi,int ieta,int ecal_et) {
    this->iphi = iphi;
    this->ieta = ieta;
    this->ecal_et = ecal_et;
    highest_ecal_et = ecal_et;
    njets = 0;
  }
  void addTower(int ecal_et) {
    if ( ecal_et > highest_ecal_et ) highest_ecal_et = ecal_et;
    this->ecal_et += ecal_et;
  }
  void addJet(JetInfo & jet) {
    jetlist[njets] = jet;
    njets++;
  }
};
#endif
