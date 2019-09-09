#ifndef JET_HH
#define JET_HH

#include "Config.hh"
#include "JetInfo.hh"

const int M_JET_OVR = 4; // Maximum Number of Jets for Overlap

class Jet {
public:
  bool isSet;
  int ieta;
  int iphi;
  int et;
  int highest_et;
  
  Jet() {
    isSet = false;
    ieta = 0;
    iphi = 0;
    et = 0;
    highest_et = 0;
  }
  void setSeed(int iphi,int ieta,int et,int highest_et) {
    isSet = true;
    this->iphi = iphi;
    this->ieta = ieta;
    this->et = et;
    this->highest_et = highest_et;
  }
  bool addCluster(int dphi,int deta,int et,int highest_et) {
    if ( dphi > 0 || (dphi == 0 && deta > 0) )
      if ( highest_et > this->highest_et )
	return false;
    if ( dphi < 0 || (dphi == 0  && deta < 0) )
      if ( highest_et >= this->highest_et )
	return false;
    this->et += et;
    return true;
  }
  void checkOverlap(JetInfo jetlist[M_JET_OVR],int njets, int et) {
    for (int ijet = 0; ijet < M_JET_OVR; ijet++) {
      if (ijet >= njets) continue;
      JetInfo overlap = jetlist[ijet];
      if ( !this->equal(overlap) ) {
    	if (DEBUG_Ovl) printf("---Overlap Jet Found: (%i,%i,%i)\n",overlap.iphi,overlap.ieta,overlap.et);
    	if ( this->et < overlap.et ) {
    	  if (DEBUG_Ovl) printf("-----Removing Tower: (%i)\n",et);
    	  this->et -= et;
	  if (DEBUG_Ovl) printf("-------Result: (%i,%i,%i)\n",iphi,ieta,this->et);
    	  return;
    	} 
      }
    }
  }
  bool equal(JetInfo jet) {
    return this->iphi == jet.iphi && this->ieta == jet.ieta;
  }
};
#endif
