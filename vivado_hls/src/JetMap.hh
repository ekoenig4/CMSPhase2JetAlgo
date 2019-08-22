#ifndef JETMAP_HH
#define JETMAP_HH

#include "Jet.hh"

class JetMap {
private:
  const static int maxJets = 23;
  int njets;
  Jet* jets[maxJets];
  inline int key(Jet* jet) { return (100*jet->seed.iphi + jet->seed.ieta) % maxJets ; }

public:
  JetMap(){
    njets = 0;
    for (int i = 0; i < maxJets; i++)
      jets[i] = 0;
  }
  void add(Jet* jet) {
    jets[ key(jet) ] = jet;
    njets++;
  }
  inline int max() { return maxJets; }
  inline int size() { return njets; }
  inline Jet* &operator[](int index) { return jets[index]; }
  inline Jet* get(int index) { return jets[index]; }
  
  
};

#endif
