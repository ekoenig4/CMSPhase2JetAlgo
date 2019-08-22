#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <bitset>

#include "../../../../../APx_Gen0_Algo/VivadoHls/null_algo_unpacked/vivado_hls/src/algo_unpacked.h"
//#include "algo_unpacked.h"

const bool DEBUG = true;

using namespace std;

ap_uint<192> link_in[N_CH_IN];
ap_uint<192> link_out[N_CH_OUT];

int main(int argc, char ** argv) {
  if (DEBUG) printf("Starting Test Bench\n");

  string test_vector;
  test_vector = argv[1];

  string ifname(test_vector + "_inp.txt"); // input test vector
  string ofname(test_vector + "_csim_out.txt"); // output test vector
  string orfname(test_vector + "_out_ref.txt"); // reference output vector

  // Open input stream...
  ifstream ifs(ifname.c_str());
  if (!ifs.is_open()) {
    cerr << "Error opening input file: " << ifname << endl;
    exit(1);
  }
  
  //...and position at the beginning of input test data
  string line;
  while (true) {
    ifs >> line;
    if (line.compare("#BeginData") == 0)
      break;
  }
  
  // Open output stream and write a header
  ofstream ofs(ofname.c_str());
  if (!ofs.is_open()) {
    cerr << "Error opening output file: " << ofname << endl;
    exit(1);
  }

  // Open output reference stream...
  // ifstream orfs(orfname.c_str());
  // if (!orfs.is_open()) {
  //   cerr << "Error opening output reference file: " << orfname << endl;
  //   exit(1);
  // }

  uint32_t wordCnt = 0;

  uint64_t data[N_CH_IN];
  
  ofs << "=====================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================" << endl;
  ofs << "WordCnt             LINK_00               LINK_01               LINK_02               LINK_03               LINK_04               LINK_05               LINK_06               LINK_07               LINK_08               LINK_09               LINK_10               LINK_11               LINK_12               LINK_13               LINK_14               LINK_15               LINK_16               LINK_17               LINK_18               LINK_19               LINK_20               LINK_21               LINK_22               LINK_23               LINK_24               LINK_25               LINK_26               LINK_27               LINK_28               LINK_29               LINK_30               LINK_31               LINK_32               LINK_33               LINK_34               LINK_35               LINK_36               LINK_37               LINK_38               LINK_39               LINK_40               LINK_41               LINK_42               LINK_43               LINK_44               LINK_45               LINK_46               LINK_47" << endl;
  ofs << "#BeginData" << endl;
  
  while (!ifs.eof()) {
    for (int cyc = 0; cyc < 3; cyc++) {
      ifs >> hex >> wordCnt;
      if  (ifs.eof())
	break;

      for (int link = 0; link < N_CH_IN; link++)
	{
	  ap_uint<64> tmp;
	  if (cyc == 0) {
	    ifs >> hex >> tmp;
	    link_in[link].range(63, 0) = tmp;
	  }
	  else if (cyc == 1) {
	    ifs >> hex >> tmp;
	    link_in[link].range(127, 64) = tmp;
	  }
	  else {
	    ifs >> hex >> tmp;
	    link_in[link].range(191, 128) = tmp;
	  }

	  if  (ifs.eof())
	    break;
	}
    }

    if  (ifs.eof())
      break;

    algo_unpacked(link_in, link_out);

    wordCnt-=2;

    for (int cyc = 0; cyc < 3; cyc++) {
      ofs << "0x" << setfill('0') << setw(4) << hex << wordCnt++ << "   ";
      for (int link = 0; link < N_CH_OUT; link++) {

	if (cyc == 0)
	  ofs << "0x" << setfill('0') << setw(16) << hex << link_out[link].range(63,0).to_int64() << "    ";
	else if (cyc == 1)
	  ofs << "0x" << setfill('0') << setw(16) << hex << link_out[link].range(127,64).to_int64() << "    ";
	else
	  ofs << "0x" << setfill('0') << setw(16) << hex << link_out[link].range(191,128).to_int64() << "    ";

      }
      ofs<< std::endl;
    }
  }
}

