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

  if (DEBUG) printf("Filling link_in with input data\n");
  string line; int link;
  while ( getline(ifs,line) ) {
    if ( link == N_CH_IN ) break;
    ap_uint<192> tmp(line.c_str(),2);
    link_in[link] = tmp;
    link++;
    if (ifs.eof()) break;
  }

  algo_unpacked(link_in,link_out);

  for (int link = 0; link < N_CH_OUT; link++) {
    ofs << bitset<192>(link_out[link]) << std::endl;
  }
  
}

