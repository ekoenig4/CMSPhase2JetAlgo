#ifndef SecondPass_cpp
#define SecondPass_cpp

#include "SecondPass.hh"
#include <stdio.h>

void SetJet(SuperTower supertowers[9],Jet &jet) {
#pragma HLS PIPELINE II=3
#pragma HLS ARRAY PARTITION variable=supertowers complete dim=0

  jet.peak_et = supertowers[4].peak_et;
  jet.isSet = jet.peak_et > M_ET;
  // printf("---Seeding Jet Peak Et: %i\n",jet.peak_et);
 SetJet:for (int r = 0; r < 9; r++) {
#pragma HLS UNROLL
    SuperTower supertower = supertowers[r];
    if ( r < 4 && supertower.peak_et >= jet.peak_et )
      jet.isSet = false;
    else if ( r > 4 && supertower.peak_et > jet.peak_et )
      jet.isSet = false;
    else
      jet.et += supertower.et;
    // printf("----Adding SuperTower %i: %i -> %i | valid:%i\n",r,supertower.et,jet.et,(int)jet.isSet);
  }
}

void RegionSecondPass(SuperTower supertowers[3*M_SUPERS_PER_PHI],Jet jets[M_SUPERS_PER_PHI]) {
#pragma HLS PIPELINE II=3
#pragma HLS ARRAY PARTITION variable=supertowers complete dim=0
#pragma HLS ARRAY PARTITION variable=jets complete dim=0

  SuperTower tmp_supers[9];
#pragma HLS ARRAY PARTITION variable=tmp_supers complete dim=0
 EtaRegion_Second:for (int iRegion = 0; iRegion < 12; iRegion++) {
#pragma HLS UNROLL
    jets[iRegion].init();

    // printf("--Processing Eta Region %i\n",iRegion);
    for (int isup = 0; isup < 9; isup++) {
#pragma HLS UNROLL
      int super_index = 3*iRegion - 3 + isup;
      if (super_index < 0 || super_index >= 3*M_SUPERS_PER_PHI){
	// printf("SuperTower %i out of bounds\n",isup);
	tmp_supers[isup].init();
      }
      else{
	tmp_supers[isup].set(supertowers[super_index]);
	// printf("SuperTower %i/%i: (%i,%i)\n",isup,super_index,tmp_supers[isup].et,tmp_supers[isup].peak_et);
      }
    }

    SetJet(tmp_supers,jets[iRegion]);
  }
}

void get9x9SecondPass(SuperTower supertowers[M_SUPERS],Jet jets[M_JET]) {
#pragma HLS PIPELINE II=3
#pragma HLS ARRAY PARTITION variable=supertowers complete dim=0
#pragma HLS ARRAY PARTITION variable=jets complete dim=0

  SuperTower tmp_supers[3*M_SUPERS_PER_PHI];
  Jet tmp_jets[M_SUPERS_PER_PHI];
#pragma HLS ARRAY PARTITION variable=tmp_supers complete dim=0
#pragma HLS ARRAY PARTITION variable=tmp_jets complete dim=0

 PhiRegion_Second:for (int iRegion = 0; iRegion < 1; iRegion++) {
#pragma HLS UNROLL

  TmpJetInit:for (int ijet = 0; ijet < M_SUPERS_PER_PHI; ijet++) {
#pragma HLS UNROLL
      tmp_jets[ijet].init();
    }

  TmpSuperInit:for (int isup = 0; isup < 3*M_SUPERS_PER_PHI; isup++) {
#pragma HLS UNROLL
      int iphi = 3*(isup%3) - 1 + iRegion*3;
      int ieta = 3*(isup/3) + 1;
      int super_index = getSuperTower(iphi,ieta);
      if ( iphi <= 0 || iphi > M_PHI )
	tmp_supers[isup].init();
      else
	tmp_supers[isup].set(supertowers[super_index]);
      // printf("Init SuperTower %i: (%i,%i) | Coor: (%i,%i,%i)\n",isup,tmp_supers[isup].et,tmp_supers[isup],iphi,ieta,super_index);
    }
    
    // printf("Processing Phi Region %i\n",iRegion);
    RegionSecondPass(tmp_supers,tmp_jets);

  WriteJet:for (int ijet = 0; ijet < M_SUPERS_PER_PHI; ijet++) {
#pragma HLS UNROLL
      if (!tmp_jets[ijet].isSet) continue;
      int jet_index = M_SUPERS_PER_PHI*iRegion + ijet;
      jets[jet_index].set(tmp_jets[ijet]);
      printf("Setting Jet %i: (%i,%i)\n",jet_index,jets[jet_index].et,jets[jet_index].peak_et);
    }
  }
}

#endif
