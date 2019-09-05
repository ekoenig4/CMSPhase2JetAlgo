#include "JetFinder.hh"

#include <iostream>
#include <bitset>

int getTowerEt(int iphi,int ieta,ap_uint<192> link_in[N_CH_IN]) {
  int index = 136*( (iphi-1)/4 ) + (iphi-1)%4 + (ieta-1)*4;
  int channel = index/18;
  int bitLo = 10*(index%18);
  int bitHi = bitLo + 10;
  ap_uint<192> link = link_in[channel]; 
  ap_uint<10> u_et(link.range(bitHi,bitLo));
  int et(u_et);
  return et;
}

int getTower3x3(int iphi,int ieta) {
  return (iphi-1)/3 + 8*( ieta/3 );
}

void setSeed(Jet& jet,Tower3x3& tower3x3) {
  jet.setSeed(tower3x3.iphi,tower3x3.ieta,tower3x3.ecal_et,tower3x3.highest_ecal_et);
}

void addJet(Tower3x3& tower3x3,Jet& jet) {
  JetInfo jetinfo = JetInfo(jet.iphi,jet.ieta,jet.ecal_et);
  tower3x3.addJet( jetinfo );
}

bool addCluster(Jet& jet,Tower3x3& tower3x3,int dphi,int deta) {
  bool valid = jet.addCluster(dphi,deta,tower3x3.ecal_et,tower3x3.highest_ecal_et);
  if (valid) addJet(tower3x3,jet);
  return valid;
}

void checkOverlap(Jet& jet,Tower3x3& tower3x3) {
  jet.checkOverlap(tower3x3.jetlist,tower3x3.njets,tower3x3.ecal_et);
}

void get3x3FirstPass(ap_uint<192> link_in[N_CH_IN],Tower3x3 tower3x3s[M_3x3]) {
  /**
     Loop through towers and build 3x3 Tower groups 
   **/
  if (DEBUG_3x3) printf("Starting 3x3 First Pass\n");
  int ntower3x3 = 0;
  for (int ieta = 1; ieta <= M_ETA; ieta += 3) {
#pragma HLS UNROLL
    for (int iphi = 2; iphi <= M_PHI; iphi += 3) {
#pragma HLS UNROLL
      Tower3x3& tower3x3 = tower3x3s[ntower3x3];
      tower3x3.setSeed(iphi,ieta,getTowerEt(iphi,ieta,link_in));
      if (DEBUG_3x3) printf("Tower3x3: (%i,%i,%i)\n",tower3x3.iphi,tower3x3.ieta,tower3x3.ecal_et);
      for (int r = 0; r < 8; r++) {
#pragma HLS UNROLL
      	int nphi = iphi + RING_3x3[r][0];
      	int neta = ieta + RING_3x3[r][1];
      	if ( (nphi <= 0 || nphi > M_PHI) || (neta <= 0 || neta > M_ETA) ) continue;
      	int et = getTowerEt(nphi,neta,link_in);
      	tower3x3.addTower( et );
      	if (DEBUG_3x3) printf("---Adding: (%i,%i,%i)\n",nphi,neta,et);
      }
      if (DEBUG_3x3) printf("Final: (%i,%i,%i)\n\n",tower3x3.iphi,tower3x3.ieta,tower3x3.ecal_et);
      ntower3x3++;
    }
  }
  if (DEBUG_3x3) printf("Finished 3x3 First Pass\n");
}

void get9x9SecondPass(Tower3x3 tower3x3s[M_3x3],Jet jets[M_JET],int &njets) {
  /**
     Loop through 3x3 Tower groups and build 9x9 Jets
   **/
  if (DEBUG_9x9) printf("Starting 9x9 Second Pass\n");
  for (int ieta = 1; ieta <= M_ETA; ieta += 3) {
#pragma HLS UNROLL
    for (int iphi = 2; iphi <= M_PHI; iphi += 3) {
#pragma HLS UNROLL
      int index = getTower3x3(iphi,ieta);
      Tower3x3& tower3x3 = tower3x3s[index];
      if (tower3x3.highest_ecal_et < M_ET) continue;
      Jet jet = Jet();
      setSeed(jet,tower3x3);
      if (DEBUG_9x9) printf("Seeding Jet: (%i,%i,%i,%i)\n",jet.iphi,jet.ieta,jet.ecal_et,jet.highest_ecal_et);
      bool valid = true;
      for (int r = 0; r < 8; r++) {
#pragma HLS UNROLL
      	int nphi = iphi + 3*RING_3x3[r][0];
      	int neta = ieta + 3*RING_3x3[r][1];
      	if ( (nphi <= 0 || nphi > M_PHI) || (neta <= 0 || neta > M_ETA) ) continue;
	Tower3x3& tower3x3 = tower3x3s[ getTower3x3(nphi,neta) ];
	valid = addCluster( jet,tower3x3,RING_3x3[r][0],RING_3x3[r][1] );
	if (DEBUG_9x9) printf("---Adding Tower: (%i,%i,%i,%i) | valid %i\n",tower3x3.iphi,tower3x3.ieta,tower3x3.ecal_et,tower3x3.highest_ecal_et,valid);
	if (!valid) break;
      }
      if (valid) {
	if ( jet.ecal_et > 0 ) {
	  if (DEBUG_9x9) printf("Clustering Jet: (%i,%i,%i,%i)\n\n",jet.iphi,jet.ieta,jet.ecal_et,jet.highest_ecal_et);
	  jets[njets] = jet;
	  njets++;
	}
      }
    }
  }
  if (DEBUG_9x9) {
    printf("Finishing 9x9 Second Pass | %i Total Jets Found\n",njets);
    for (int i = 0; i < M_JET; i++) {
      Jet& jet = jets[i];
      if ( i >= njets ) continue;
      printf("Jet %i: (%i,%i,%i)\n",i+1,jet.iphi,jet.ieta,jet.ecal_et);
    }
  }
  
}

void getOverlapThirdPass(Tower3x3 tower3x3s[M_3x3],Jet jets[M_JET],int &njets) {
  if (DEBUG_Ovl) printf("Starting Overlap Third Pass | %i Total Jets\n",njets);
  for (int i = 0; i < M_JET; i++) {
#pragma HLS UNROLL
    if ( i >= njets ) continue;
    Jet& jet = jets[i];
    
    if (DEBUG_Ovl) printf("Checking Jet: (%i,%i,%i)\n",jet.iphi,jet.ieta,jet.ecal_et);
    for (int r = 0; r < 8; r++) {
#pragma HLS UNROLL
      int nphi = jet.iphi + 3*RING_3x3[r][0];
      int neta = jet.ieta + 3*RING_3x3[r][1];
      if ( (nphi <= 0 || nphi > M_PHI) || (neta <= 0 || neta > M_ETA) ) continue;
      checkOverlap( jet,tower3x3s[ getTower3x3(nphi,neta) ] );
    }
  }
}
