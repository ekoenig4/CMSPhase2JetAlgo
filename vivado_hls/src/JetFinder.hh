#ifndef JETFINDER_HH
#define JETFINDER_HH

#include "../../../../../APx_Gen0_Algo/VivadoHls/null_algo_unpacked/vivado_hls/src/algo_unpacked.h"
#include "Config.hh"
#include "JetInfo.hh"
#include "Jet.hh"
#include "Tower.hh"

void findJets(Tower towers[M_TOWERS],Jet jets[M_JET]);

#endif
