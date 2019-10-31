#ifndef CONFIG_HH
#define CONFIG_HH

// Current Et is stored as 0.25 GeV
// Current maximum Et is 256 GeV

const bool DEBUG = false;
const bool DEBUG_3x3 = false;
const bool DEBUG_9x9 = false;
const bool DEBUG_Ovl = false;
const int N_T_CH = 18;   // Number of towers per channel
const int M_TOWERS = 816;
const int M_SUPERS = 96;
const int M_PHI = 24;    // Maximum Phi Value for Calorimeter Slice
const int M_ETA = 34;    // Maximum Eta Value for Calorimeter Slice
const int M_TOWERS_PER_PHI = 102;
const int M_SUPERS_PER_PHI = 12;
const int M_JET = M_SUPERS;    // Maximum Number of Jets to be saved
const int M_ET = 10;     // Threshold for Jet seed in 0.25 GeV

const int RING_3x3[9][2] = { {-1,1},{0,1},{1,1},
			     {-1,0},{0,0},{1,0},
			     {-1,-1},{0,-1},{1,-1} };

inline int getSuperTower(int iphi,int ieta) {
  return 12*(iphi/3) + (ieta-1)/3;
}

inline int getTower(int iphi,int ieta) {
  return 136*((iphi-1)/4) + (iphi-1)%4 + 4*(ieta-1);
}

#endif
