#ifndef CONFIG_HH
#define CONFIG_HH

// Current Et is stored as 0.25 GeV
// Current maximum Et is 256 GeV

const bool DEBUG = false;
const bool DEBUG_3x3 = false;
const bool DEBUG_9x9 = false;
const bool DEBUG_Ovl = false;
const int N_T_CH = 18;   // Number of towers per channel
const int M_PHI = 24;    // Maximum Phi Value for Calorimeter Slice
const int M_ETA = 34;    // Maximum Eta Value for Calorimeter Slice
const int M_3x3 = 96;    // Maximum Number of 3x3 Towers
const int M_JET = 15;    // Maximum Number of Jets to be saved
const int M_ET = 10;     // Threshold for Jet seed in 0.25 GeV

#endif
