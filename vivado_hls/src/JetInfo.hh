#ifndef JETINFO_HH
#define JETINFO_HH

class JetInfo {
public:
  int iphi;
  int ieta;
  int ecal_et;

  JetInfo() {};
  JetInfo(int & iphi,int & ieta,int & ecal_et) {
    this->iphi = iphi;
    this->ieta = ieta;
    this->ecal_et = ecal_et;
  }
};

#endif
