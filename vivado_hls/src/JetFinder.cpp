#include "JetFinder.hh"

#include <iostream>
#include <bitset>

int getTowerEt(int iphi,int ieta,ap_uint<192> link_in[N_CH_IN]) {
  int index = 136*( (iphi-1)/4 ) + (iphi-1)%4 + (ieta-1)*4;
  const int channel = index/18;
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

void setJetSeed(Jet& jet,Tower3x3& tower3x3) {
  jet.isSet = true;
  jet.iphi = tower3x3.iphi;
  jet.ieta = tower3x3.ieta;
  jet.et = tower3x3.et;
  jet.highest_et = tower3x3.highest_et;
}

void setTowerSeed(Tower3x3& tower3x3,int iphi,int ieta,int et) {
  tower3x3.iphi = iphi;
  tower3x3.ieta = ieta;
  tower3x3.et = et;
  tower3x3.highest_et = et;
  tower3x3.njets = 0;
}

void addJet(Tower3x3& tower3x3,Jet& jet) {
  JetInfo jetinfo = JetInfo(jet.iphi,jet.ieta,jet.et);
  tower3x3.jetlist[ tower3x3.njets ] = jetinfo;
  tower3x3.njets++;
  
}

// void addCluster(Jet& jet,Tower3x3& tower3x3,int dphi,int deta) {
//   if ( dphi > 0 || (dphi == 0 && deta > 0) ) {
//     if ( tower3x3.highest_et > jet.highest_et )
//       jet.isSet = false;
//   } else if ( dphi < 0 || (dphi == 0  && deta < 0) ) {
//     if ( tower3x3.highest_et >= jet.highest_et )
//       jet.isSet = false;
//   } else {
//     jet.et += tower3x3.et;
//   }
// }

void checkOverlap(Jet& jet,Tower3x3& tower3x3) {
  for (int ijet = 0; ijet < M_JET_OVR; ijet++) {
    if (ijet >= tower3x3.njets) continue;
    JetInfo overlap = tower3x3.jetlist[ijet];
    if ( !jet.equal(overlap) ) {
      // printf("---Overlap Jet Found: (%i,%i,%i)\n",overlap.iphi,overlap.ieta,overlap.et);
      if ( jet.et < overlap.et ) {
	// printf("-----Removing Tower: (%i)\n",tower3x3.et);
	jet.et -= tower3x3.et;
	// printf("-------Result: (%i,%i,%i)\n",jet.iphi,jet.ieta,jet.et);
	return;
      } 
    }
  }
}

void get3x3FirstPass(ap_uint<192> link_in[N_CH_IN],Tower3x3 tower3x3s[M_3x3]) {
  /**
     Loop through towers and build 3x3 Tower groups 
  **/
  // printf("Starting 3x3 First Pass\n");
  int ntower3x3 = 0;
  for (int ieta = 1; ieta <= M_ETA; ieta += 3) {
#pragma HLS UNROLL
    for (int iphi = 2; iphi <= M_PHI; iphi += 3) {
#pragma HLS UNROLL
      Tower3x3& tower3x3 = tower3x3s[ntower3x3];
      int seed_et = getTowerEt(iphi,ieta,link_in);
      setTowerSeed(tower3x3,iphi,ieta,seed_et);
      // printf("Tower3x3: (%i,%i,%i)\n",tower3x3.iphi,tower3x3.ieta,tower3x3.et);
      for (int r = 0; r < 8; r++) {
#pragma HLS UNROLL
      	int nphi = iphi + RING_3x3[r][0];
      	int neta = ieta + RING_3x3[r][1];
      	if ( (nphi <= 0 || nphi > M_PHI) || (neta <= 0 || neta > M_ETA) ) continue;
      	int et = getTowerEt(nphi,neta,link_in);
      	tower3x3.addTower( et );
      	// printf("---Adding: (%i,%i,%i)\n",nphi,neta,et);
      }
      // printf("Final: (%i,%i,%i)\n\n",tower3x3.iphi,tower3x3.ieta,tower3x3.et);
      ntower3x3++;
    }
  }
  // printf("Finished 3x3 First Pass\n");
}

void get9x9SecondPass(Tower3x3 tower3x3s[M_3x3],Jet jets[M_3x3]) {
  /**
     Loop through 3x3 Tower groups and build 9x9 Jets
  **/
  // printf("Starting 9x9 Second Pass\n");
  for (int ieta = 1; ieta <= M_ETA; ieta += 3) {
#pragma HLS UNROLL
    for (int iphi = 2; iphi <= M_PHI; iphi += 3) {
#pragma HLS UNROLL
      const int seed_index = getTower3x3(iphi,ieta);
      Tower3x3& seed = tower3x3s[seed_index];
      if (seed.highest_et < M_ET) continue;
      Jet& jet = jets[seed_index];
      setJetSeed(jet,seed);
      // printf("Seeding Jet: (%i,%i,%i,%i)\n",jet.iphi,jet.ieta,jet.et,jet.highest_et);
      for (int r = 0; r < 8; r++) {
#pragma HLS UNROLL
      	int nphi = iphi + 3*RING_3x3[r][0];
      	int neta = ieta + 3*RING_3x3[r][1];
      	if ( (nphi <= 0 || nphi > M_PHI) || (neta <= 0 || neta > M_ETA) ) continue;
	const int tower_index = getTower3x3(nphi,neta);
	Tower3x3& tower3x3 = tower3x3s[ tower_index ];

	// addCluster( jet,tower3x3,RING_3x3[r][0],RING_3x3[r][1] );
	if ( RING_3x3[r][0] > 0 || (RING_3x3[r][0] == 0 && RING_3x3[r][1] > 0) ) {
	  if ( tower3x3.highest_et > jet.highest_et )
	    jet.isSet = false;
	} else if ( RING_3x3[r][0] < 0 || (RING_3x3[r][0] == 0  && RING_3x3[r][1] < 0) ) {
	  if ( tower3x3.highest_et >= jet.highest_et )
	    jet.isSet = false;
	} else {
	  jet.et += tower3x3.et;
	}
// 	// printf("---Adding Tower: (%i,%i,%i,%i) | valid %i\n",tower3x3.iphi,tower3x3.ieta,tower3x3.et,tower3x3.highest_et,valid);
	if (!jet.isSet) break;
      }
    }
  }
}

void getOverlapThirdPass(Tower3x3 tower3x3s[M_3x3],Jet jets[M_3x3]) {
  // printf("Starting Overlap Third Pass | %i Total Jets\n",njets);
  for (int i = 0; i < M_3x3; i++) {
#pragma HLS UNROLL
    Jet& jet = jets[i];
    if (!jet.isSet) continue;
    
    // printf("Checking Jet: (%i,%i,%i)\n",jet.iphi,jet.ieta,jet.et);
    for (int r = 0; r < 8; r++) {
#pragma HLS UNROLL
      int nphi = jet.iphi + 3*RING_3x3[r][0];
      int neta = jet.ieta + 3*RING_3x3[r][1];
      if ( (nphi <= 0 || nphi > M_PHI) || (neta <= 0 || neta > M_ETA) ) continue;
      checkOverlap( jet,tower3x3s[ getTower3x3(nphi,neta) ] );
    }
  }
}
