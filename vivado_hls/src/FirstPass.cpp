#ifndef FirstPass_cpp
#define FirstPass_cpp

#include "FirstPass.hh"
#include <stdio.h>

void SetSuperTower(Tower towers[9],SuperTower &supertower) {
#pragma HLS PIPELINE II=3
#pragma HLS ARRAY PARTITION variable=towers complete dim=0
  
 SetSuperTower:for (int r = 0; r < 9; r++) {
    Tower tower = towers[r];
    supertower.et += tower.et;
    if (tower.et > supertower.peak_et) supertower.peak_et = tower.et;
//printf("----Adding Tower %i -> (%i,%i)\n",tower.et,supertower.et,supertower.peak_et);
  }
}

void RegionFirstPass(Tower towers[M_TOWERS_PER_PHI],SuperTower supertowers[M_SUPERS_PER_PHI]) {
#pragma HLS PIPELINE II=3
#pragma HLS ARRAY PARTITION variable=towers complete dim=0
#pragma HLS ARRAY PARTITION variable=supertowers complete dim=0

  Tower tmp_towers[9];
#pragma HLS ARRAY PARTITION variable=tmp_towers complete dim=0
 EtaRegion_First:for (int iRegion = 0; iRegion < 12; iRegion++) {
#pragma HLS UNROLL
    supertowers[iRegion].init();
    
    for (int itow = 0; itow < 9; itow++) {
#pragma HLS UNROLL
      int tower_index = 9*iRegion - 3 + itow;
      if (tower_index < 0 || tower_index >= M_TOWERS_PER_PHI)
	tmp_towers[itow].et = 0;
      else
	tmp_towers[itow].et = towers[tower_index].et;
    }

//printf("--Processing Eta Region %i\n",iRegion);
    SetSuperTower(tmp_towers,supertowers[iRegion]);
  }
}

void get3x3FirstPass(Tower towers[M_TOWERS],SuperTower supertowers[M_SUPERS]) {
#pragma HLS PIPELINE II=3
#pragma HLS ARRAY PARTITION variable=towers complete dim=0
#pragma HLS ARRAY PARTITION variable=supertowers complete dim=0

  Tower tmp_towers[M_TOWERS_PER_PHI];
  SuperTower tmp_supers[M_SUPERS_PER_PHI];
#pragma HLS ARRAY PARTITION variable=tmp_towers complete dim=0
#pragma HLS ARRAY PARTITION variable=tmp_supers complete dim=0
  
 PhiRegion_First:for (int iRegion = 0; iRegion < 8; iRegion++) {
#pragma HLS UNROLL
    
  TmpSuperInit:for (int isup = 0; isup < M_SUPERS_PER_PHI; isup++) {
#pragma HLS UNROLL
      tmp_supers[isup].init();
    }

  TmpTowerInit:for (int itow = 0; itow < M_TOWERS_PER_PHI; itow++) {
#pragma HLS UNROLL
      int iphi = itow%3 + 1 + iRegion*3;
      int ieta = itow/3 + 1;
      int tower_index = getTower(iphi,ieta);
      tmp_towers[itow].et = towers[tower_index].et;
    }

//printf("Processing Phi Region %i\n",iRegion);
    RegionFirstPass(tmp_towers,tmp_supers);

  WriteSuper:for (int isup = 0; isup < M_SUPERS_PER_PHI; isup++) {
#pragma HLS UNROLL
      int super_index = M_SUPERS_PER_PHI*iRegion + isup;
      supertowers[super_index].set(tmp_supers[isup]);
      // printf("Setting SuperTower %i: (%i,%i)\n",super_index,supertowers[super_index].et,supertowers[super_index].peak_et);
    }
  }
}

#endif
