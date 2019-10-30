#include "JetFinder.hh"

#include <iostream>
#include <bitset>

const int RING_3x3[8][2] = { {-1,1},{0,1},{1,1},
			     {-1,0},      {1,0},
			     {-1,-1},{0,-1},{1,-1} };

// void setJetSeed(Jet& jet,SuperTower& supertower) {
//   jet.isSet = true;
//   jet.et = supertower.et;
//   jet.peak_et = supertower.peak_et;
// }

// void setTowerSeed(SuperTower& supertower,int iphi,int ieta,int et) {
//   supertower.et = et;
//   supertower.peak_et = et;
//   supertower.njets = 0;
// }

// void addJet(SuperTower& supertower,Jet& jet) {
//   JetInfo jetinfo = JetInfo(jet.iphi,jet.ieta,jet.et);
//   supertower.jetlist[ supertower.njets ] = jetinfo;
//   supertower.njets++;
  
// }

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

inline int getSuperTower(int iphi,int ieta) {
  return 12*(iphi/3) + (ieta-1)/3;
}

inline int getTower(int iphi,int ieta) {
  return 136*((iphi-1)/4) + (iphi-1)%4 + 4*(ieta-1);
}

void get3x3FirstPass(Tower towers[M_TOWERS],SuperTower supertowers[M_SUPERS]) {
#pragma HLS PIPELINE II=3
#pragma HLS ARRAY PARTITION variable=towers complete dim=0
#pragma HLS ARRAY PARTITION variable=supertowers complete dim=0

 FirstPass:for (int iphi = 2; iphi <= M_PHI; iphi += 3) {
#pragma HLS UNROLL
    
  EtaSlice3x3:for (int ieta = 1; ieta <= M_ETA; ieta += 3) {
#pragma HLS UNROLL

      int super_index = getSuperTower(iphi,ieta);
      SuperTower& supertower = supertowers[super_index];
      int seed_index = getTower(iphi,ieta);
      Tower seed_tower = towers[seed_index];
      supertower.et = seed_tower.et;
      supertower.peak_et = seed_tower.et;
      // printf("Getting SuperTower %i Center: (%i,%i)\n",super_index,iphi,ieta);
    SetSuperTower:for (int r = 0; r < 8; r++) {
	int nphi = iphi + RING_3x3[r][0];
	int neta = ieta + RING_3x3[r][1];

	if ( nphi <= 0 || nphi > M_PHI ) continue;
	if ( neta <= 0 || neta > M_ETA ) continue;
	int index = getTower(nphi,neta);
	Tower tower = towers[index];
	// printf("Adding Tower %i: (%i,%i)\n",index,nphi,neta);
	supertower.et += tower.et;
	if (tower.et > supertower.peak_et) supertower.peak_et = tower.et;
      }
    }
  }
}


void get9x9SecondPass(SuperTower supertowers[M_SUPERS],Jet jets[M_JET]) {
#pragma HLS PIPELINE II=3
#pragma HLS ARRAY PARTITION variable=supertowers complete dim=0
#pragma HLS ARRAY PARTITION variable=jets complete dim=0

 SecondPass:for (int iphi = 2; iphi <= M_PHI; iphi += 3) {
#pragma HLS UNROLL

  EtaSlice9x9:for(int ieta = 1; ieta <= M_ETA; ieta += 3) {
      #pragma HLS UNROLL

      int seed_index = getSuperTower(iphi,ieta);
      SuperTower& seed_tower = supertowers[seed_index];
      // printf("Seeding %i Jet: (%i,%i,%i,%i)\n",seed_index,iphi,ieta,seed_tower.et,seed_tower.peak_et);
      if (seed_tower.peak_et < M_ET) continue;
      Jet& jet = jets[seed_index];
      jet.isSet = true;
      jet.iphi = iphi;
      jet.ieta = ieta;
      jet.et = seed_tower.et;
      jet.peak_et = seed_tower.peak_et;

    SetJet:for(int r = 0; r < 8; r++) {
#pragma HLS UNROLL
	int dphi = 3*RING_3x3[r][0];
	int deta = 3*RING_3x3[r][1];
	int nphi = iphi + dphi;
	int neta = ieta + deta;

	if ( nphi <= 0 || nphi > M_PHI ) continue;
	if ( neta <= 0 || neta > M_ETA ) continue;
	int index = getSuperTower(nphi,neta);
	SuperTower tower = supertowers[index];
	// printf("Checking %i Tower: (%i,%i,%i,%i): ",index,nphi,neta,tower.et,tower.peak_et);
	if ( dphi > 0 || ( dphi == 0 && deta > 0) )
	  if ( tower.peak_et > jet.peak_et ) {
	    jet.isSet = false;
	    break;
	  }
	if ( dphi < 0 || ( dphi == 0 && deta < 0) )
	  if ( tower.peak_et >= jet.peak_et ) {
	    jet.isSet = false;
	    break;
	  }
	// printf("valid\n");
	jet.et += tower.et;
      }
      // if (!jet.isSet) printf("skip\n");
    }
  }
}

void findJets(Tower towers[M_TOWERS],Jet jets[M_JET]) {
#pragma HLS PIPELINE II=3
#pragma HLS ARRAY PARTITION variable=towers complete dim=0
#pragma HLS ARRAY PARTITION variable=jets   complete dim=0

  SuperTower supertowers[M_SUPERS];
#pragma HLS ARRAY PARTITION variable=supertowers complete dim=0
 SuperTowerInit:for(int i = 0; i < M_SUPERS; i++) {
#pragma HLS UNROLL
    supertowers[i].et = 0;
    supertowers[i].peak_et = 0;
  }
  get3x3FirstPass(towers,supertowers);
  get9x9SecondPass(supertowers,jets);
}
