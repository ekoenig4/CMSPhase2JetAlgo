#ifndef JETFINDER_HH
#define JETFINDER_HH

#include "../../../../../APx_Gen0_Algo/VivadoHls/null_algo_unpacked/vivado_hls/src/algo_unpacked.h"
#include "Config.hh"
#include "JetInfo.hh"
#include "Jet.hh"
#include "Tower3x3.hh"

const int RING_3x3[8][2] = { {-1,1},{0,1},{1,1},
			     {-1,0},{1,0},
			     {-1,-1},{0,-1},{1,-1} };

static Tower3x3 tower3x3s[M_3x3];
static Jet jets[M_JET];
static int njets;

void get3x3FirstPass(ap_uint<192> link_in[N_CH_IN],Tower3x3 tower3x3s[M_3x3]);
void get9x9SecondPass(Tower3x3 tower3x3s[M_3x3],Jet jets[M_JET],int &njets);
void getOverlapThirdPass(Tower3x3 tower3x3s[M_3x3],Jet jets[M_JET],int &njets);

#endif
