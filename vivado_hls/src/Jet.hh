#ifndef JET_HH
#define JET_HH

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <bitset>
#include "Tower3x3.hh"
#include <stdio.h>

class Jet {
 public:
  bool isSet;
  int ieta;
  int iphi;
  int ecal_et;
  int highest_ecal_et;
  
  Jet() {
    isSet = false;
    ieta = 0;
    iphi = 0;
    ecal_et = 0;
    highest_ecal_et = 0;
  }
  void setSeed(Tower3x3* tower3x3) {
    isSet = true;
    iphi = tower3x3->iphi;
    ieta = tower3x3->ieta;
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
    // tower3x3->addJet(this);
    return true;
  }
  void checkOverlap(Tower3x3* tower3x3) {
    // for (int ijet = 0; ijet < tower3x3->njets; ijet++) {
    //   Jet* overlap = tower3x3->jetlist[ijet];
    //   if ( !this->equal(overlap) ) {
    // 	// printf("---Overlap Jet Found: (%i,%i,%i)\n",overlap->seed.iphi,overlap->seed.ieta,overlap->ecal_et);
    // 	if ( this->ecal_et < overlap->ecal_et ) {
    // 	  // printf("-----Removing Tower: (%i,%i,%i)\n",tower3x3->iphi,tower3x3->ieta,tower3x3->ecal_et);
    // 	  this->ecal_et -= tower3x3->ecal_et;
    // 	  return;
    // 	} 
    //   }
    // }
  }
  bool equal(Jet* jet) {
    return this->iphi == jet->iphi && this->ieta == jet->ieta;
  }
};
#endif
