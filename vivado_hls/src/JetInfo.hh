#ifndef JETINFO_HH
#define JETINFO_HH

class JetInfo {
public:
  int iphi;
  int ieta;
  int et;

  JetInfo() {};
  JetInfo(int iphi,int ieta,int et) {
    this->iphi = iphi;
    this->ieta = ieta;
    this->et = et;
  }
};

#endif
