#include "JetFinder.hh"

#include <iostream>
#include <bitset>

#include "FirstPass.hh"
#include "SecondPass.hh"

// void addCluster(Jet& jet,SuperTower& supertower,int dphi,int deta) {
//   if ( dphi > 0 || (dphi == 0 && deta > 0) ) {
//     if ( supertower.peak_et > jet.peak_et )
//       jet.isSet = false;
//   } else if ( dphi < 0 || (dphi == 0  && deta < 0) ) {
//     if ( supertower.peak_et >= jet.peak_et )
//       jet.isSet = false;
//   } else {
//     jet.et += supertower.et;
//   }
// }

// void checkOverlap(Jet& jet,SuperTower& supertower) {
//   for (int ijet = 0; ijet < M_JET_OVR; ijet++) {
//     if (ijet >= supertower.njets) continue;
//     JetInfo overlap = supertower.jetlist[ijet];
//     if ( !jet.equal(overlap) ) {
//       // printf("---Overlap Jet Found: (%i,%i,%i)\n",overlap.iphi,overlap.ieta,overlap.et);
//       if ( jet.et < overlap.et ) {
// 	// printf("-----Removing Tower: (%i)\n",supertower.et);
// 	jet.et -= supertower.et;
// 	// printf("-------Result: (%i,%i,%i)\n",jet.iphi,jet.ieta,jet.et);
// 	return;
//       } 
//     }
//   }
// }

// void getOverlapThirdPass(SuperTower supertowers[M_3x3],Jet jets[M_3x3]) {
//   // printf("Starting Overlap Third Pass | %i Total Jets\n",njets);
//   for (int i = 0; i < M_3x3; i++) {
// #pragma HLS UNROLL
//     Jet& jet = jets[i];
//     if (!jet.isSet) continue;
    
//     // printf("Checking Jet: (%i,%i,%i)\n",jet.iphi,jet.ieta,jet.et);
//     for (int r = 0; r < 8; r++) {
// #pragma HLS UNROLL
//       int nphi = jet.iphi + 3*RING_3x3[r][0];
//       int neta = jet.ieta + 3*RING_3x3[r][1];
//       if ( (nphi <= 0 || nphi > M_PHI) || (neta <= 0 || neta > M_ETA) ) continue;
//       checkOverlap( jet,supertowers[ getSuperTower(nphi,neta) ] );
//     }
//   }
// }

void findJets(Tower towers[M_TOWERS],Jet jets[M_JET]) {
#pragma HLS PIPELINE II=3
#pragma HLS ARRAY PARTITION variable=towers complete dim=0
#pragma HLS ARRAY PARTITION variable=jets   complete dim=0

  SuperTower supertowers[M_SUPERS];
#pragma HLS ARRAY PARTITION variable=supertowers complete dim=0
 SuperTowerInit:for(int i = 0; i < M_SUPERS; i++) {
#pragma HLS UNROLL
    supertowers[i].init();
  }
  get3x3FirstPass(towers,supertowers);
  get9x9SecondPass(supertowers,jets);
}
