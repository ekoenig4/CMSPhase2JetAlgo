#ifndef TOWER3X3_HH
#define TOWER3X3_HH

class Jet;

class Tower3x3 {
 public:
  int iphi;
  int ieta;
  int ecal_et;
  int highest_ecal_et;

  int njets;
  Jet* jetlist[4];

  Tower3x3() {
    this->setSeed(0,0,0);
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
  void addJet(Jet* jet) {
    jetlist[njets] = jet;
    njets++;
  }
};
#endif
