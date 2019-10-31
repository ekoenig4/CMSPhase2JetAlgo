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
  void init() {
    isSet = false;
    ieta = 0;
    iphi = 0;
    et = 0;
    peak_et = 0;
  }
  void set(Jet &jet) {
    isSet = true;
    ieta = jet.ieta;
    iphi = jet.iphi;
    et = jet.et;
    peak_et = jet.peak_et;
  }
  inline bool equal(JetInfo jet) {
    return this->iphi == jet.iphi && this->ieta == jet.ieta;
  }
};
#endif
