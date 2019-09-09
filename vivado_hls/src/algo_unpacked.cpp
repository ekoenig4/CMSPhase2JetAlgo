#include <stdlib.h>
#include <stdio.h>

#include "../../../../../APx_Gen0_Algo/VivadoHls/null_algo_unpacked/vivado_hls/src/algo_unpacked.h"
#include "Jet.hh"
#include "Tower3x3.hh"
#include "JetFinder.hh"
// #include "QuickSort.hh"

/*
 * algo_unpacked interface exposes fully unpacked input and output link data.
 * This version assumes use of 10G 8b10b links, and thus providing
 * 192  bits per BX (arranged as an arrray of 3x 64 bits)
 *
 * !!! N.B. Do NOT use the first byte (i.e. link_in/out[x].range(7,0) as this
 * portion is reserved for input/output link alignment markers.
 *
 * The remaining 184 bits (link_in/out[x].range(191,8)) are available for
 * algorithm use.
 *
 * !!! N.B. 2: make sure to assign every bit of link_out[] data. First byte should be assigned zero.
 */

void algo_unpacked(ap_uint<192> link_in[N_CH_IN], ap_uint<192> link_out[N_CH_OUT]) {
  if (DEBUG) printf("Starting Jet Algorithm\n");
  //ap_uint<192> link_in[N_CH_IN];
  //ap_uint<192> link_out[N_CH_IN];
  // !!! Retain these 4 #pragma directives below in your algo_unpacked implementation !!!

#pragma HLS ARRAY_PARTITION variable=link_in complete dim=0
#pragma HLS ARRAY_PARTITION variable=link_out complete dim=0
#pragma HLS PIPELINE II=3
#pragma HLS INTERFACE ap_ctrl_hs port=return

  // Initialize Arrays
#pragma HLS ARRAY_PARTITION variable=tower3x3s complete dim=1
  for (int i = 0; i < M_3x3; i++) {
#pragma HLS UNROLL
    tower3x3s[i] = Tower3x3();
  }
  #pragma HLS ARRAY PARTITION variable=jets complete dim=1
  for (int i = 0; i < M_JET; i++) {
#pragma HLS UNROLL
    jets[i] = Jet();
  }
  njets = 0;

  // null algo specific pragma: avoid fully combinatorial algo by specifying min latency
  // otherwise algorithm clock input (ap_clk) gets optimized away
#pragma HLS latency min=3
  get3x3FirstPass(link_in,tower3x3s);
  get9x9SecondPass(tower3x3s,jets,njets);
  getOverlapThirdPass(tower3x3s,jets,njets);
  // QuickSort(jets,njets);

  for (int i = 0; i < M_JET; i++) {
    Jet& jet = jets[i];
    if ( i >= njets ) continue;
    printf("Jet %i: (%i,%i,%i)\n",i+1,jet.iphi,jet.ieta,jet.et);
    link_out[i].range(5,0) = ap_uint<6>(jet.iphi);
    link_out[i].range(11,6) = ap_uint<6>(jet.ieta);
    link_out[i].range(21,12) = ap_uint<10>(jet.et);
  }
}
