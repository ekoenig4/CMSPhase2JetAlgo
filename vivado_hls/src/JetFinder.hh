#ifndef JETFINDER_HH
#define JETFINDER_HH

#include "../../../../../APx_Gen0_Algo/VivadoHls/null_algo_unpacked/vivado_hls/src/algo_unpacked.h"
#include "Jet.hh"
#include "Tower3x3.hh"

// Current Et is stored as 0.25 GeV
// Current maximum Et is 256 GeV

const bool DEBUG = false;
const bool DEBUG_3x3 = false;
const bool DEBUG_9x9 = false;
const bool DEBUG_Ovl = false;
const int N_T_CH = 18; // Number of towers per channel
const int M_PHI = 24; // Maximum Phi Value for Calorimeter Slice
const int M_ETA = 34; // Maximum Eta Value for Calorimeter Slice
const int M_3x3 = 96; // Maximum Number of 3x3 Towers
const int M_JET = 25; // Maximum Number of Jets that can be clustered

const int RING_3x3[8][2] = { {-1,1},{0,1},{1,1},
			     {-1,0},{1,0},
			     {-1,-1},{0,-1},{1,-1} };

static Tower3x3* tower3x3s[M_3x3];

static int njets = 0;
static Jet* jets[M_JET];


int getTowerEt(int iphi,int ieta,ap_uint<192> link_in[N_CH_IN]);
int getTower3x3(int iphi,int ieta);
void get3x3FirstPass(ap_uint<192> link_in[N_CH_IN],Tower3x3* tower3x3s[M_3x3]);
void get9x9SecondPass(Tower3x3* tower3x3s[M_3x3],Jet* jets[M_JET],int &njets);
void getOverlapThirdPass(Tower3x3* tower3x3s[M_3x3],Jet* jets[M_JET],int njets);

#endif
