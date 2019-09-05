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
  int ecal_et;
  int highest_ecal_et;
  
  Jet() {
    isSet = false;
    ieta = 0;
    iphi = 0;
    ecal_et = 0;
    highest_ecal_et = 0;
  }
  void setSeed(int iphi,int ieta,int ecal_et,int highest_ecal_et) {
    isSet = true;
    this->iphi = iphi;
    this->ieta = ieta;
    this->ecal_et = ecal_et;
    this->highest_ecal_et = highest_ecal_et;
  }
  bool addCluster(int dphi,int deta,int ecal_et,int highest_ecal_et) {
    if ( dphi > 0 || (dphi == 0 && deta > 0) )
      if ( highest_ecal_et > this->highest_ecal_et )
	return false;
    if ( dphi < 0 || (dphi == 0  && deta < 0) )
      if ( highest_ecal_et >= this->highest_ecal_et )
	return false;
    this->ecal_et += ecal_et;
    return true;
  }
  void checkOverlap(JetInfo jetlist[M_JET_OVR],int njets, int ecal_et) {
    for (int ijet = 0; ijet < M_JET_OVR; ijet++) {
      if (ijet >= njets) continue;
      JetInfo overlap = jetlist[ijet];
      if ( !this->equal(overlap) ) {
    	if (DEBUG_Ovl) printf("---Overlap Jet Found: (%i,%i,%i)\n",overlap.iphi,overlap.ieta,overlap.ecal_et);
    	if ( this->ecal_et < overlap.ecal_et ) {
    	  if (DEBUG_Ovl) printf("-----Removing Tower: (%i)\n",ecal_et);
    	  this->ecal_et -= ecal_et;
	  if (DEBUG_Ovl) printf("-------Result: (%i,%i,%i)\n",iphi,ieta,this->ecal_et);
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
