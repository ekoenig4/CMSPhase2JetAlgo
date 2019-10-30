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
  int peak_et;
  
  inline bool equal(JetInfo jet) {
    return this->iphi == jet.iphi && this->ieta == jet.ieta;
  }
};
#endif
