#ifndef JET_HH
#define JET_HH

#include "Tower3x3.hh"
#include <stdio.h>

class Jet {
 public:
  int index;
  struct {
    int ieta;
    int iphi;
    int et;
  } seed;
  int ecal_et;
  int highest_ecal_et;
  
  Jet(int index) {
    this->index = index;
    seed.ieta = 0;
    seed.iphi = 0;
    seed.et = 0;
    ecal_et = 0;
    highest_ecal_et = 0;
  }
  void setSeed(Tower3x3* tower3x3) {
    seed.iphi = tower3x3->iphi;
    seed.ieta = tower3x3->ieta;
    seed.et = tower3x3->ecal_et;
    ecal_et = tower3x3->ecal_et;
    highest_ecal_et = tower3x3->highest_ecal_et;
  }
  bool addCluster(Tower3x3* tower3x3,int dphi,int deta) {
    if ( dphi > 0 || (dphi == 0 && deta > 0) )
      if ( tower3x3->highest_ecal_et > highest_ecal_et )
	return false;
    if ( dphi < 0 || (dphi == 0  && deta < 0) )
      if ( tower3x3->highest_ecal_et >= highest_ecal_et )
	return false;
    ecal_et += tower3x3->ecal_et;
    tower3x3->addJet(this);
    return true;
  }
  void checkOverlap(Tower3x3* tower3x3) {
    for (int ijet = 0; ijet < tower3x3->njets; ijet++) {
      auto& overlap = tower3x3->jetlist[ijet];
      if ( this != overlap ) {
	// printf("---Overlap Jet Found: (%i,%i,%i)\n",overlap->seed.iphi,overlap->seed.ieta,overlap->ecal_et);
	if ( this->ecal_et < overlap->ecal_et ) {
	  // printf("-----Removing Tower: (%i,%i,%i)\n",tower3x3->iphi,tower3x3->ieta,tower3x3->ecal_et);
	  this->ecal_et -= tower3x3->ecal_et;
	  return;
	} 
      }
    }
  }
};
#endif
