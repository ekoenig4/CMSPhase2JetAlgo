#ifndef TOWER3X3_HH
#define TOWER3X3_HH

class Jet;

class Tower3x3 {
 public:
  int index;
  int iphi;
  int ieta;
  int ecal_et;
  int highest_ecal_et;

  int njets = 0;
  Jet* jetlist[5];
  
  Tower3x3(int index) {
    this->index = index;
    iphi = 0;
    ieta = 0;
    ecal_et = 0;
    highest_ecal_et = 0;
    njets = 0;
    for (int i = 0; i < 5; i++) jetlist[i] = 0;
  }
  void setSeed(int iphi,int ieta,int ecal_et) {
    this->iphi = iphi;
    this->ieta = ieta;
    this->ecal_et = ecal_et;
    highest_ecal_et = ecal_et;
    njets = 0;
    for (int i = 0; i < 5; i++) jetlist[i] = 0;
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
