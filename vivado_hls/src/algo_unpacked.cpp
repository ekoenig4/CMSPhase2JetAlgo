#include <stdlib.h>
#include <stdio.h>

#include "../../../../../APx_Gen0_Algo/VivadoHls/null_algo_unpacked/vivado_hls/src/algo_unpacked.h"
#include "Jet.hh"
#include "Tower.hh"
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

  ap_uint<192> tmp_link_out[N_CH_OUT];
#pragma HLS ARRAY_PARTITION variable=tmp_link_out    complete dim=0
 OutputInit:for (int idx = 0; idx < N_CH_OUT; idx++){
#pragma HLS UNROLL
    tmp_link_out[idx]         = 0;
  }

  Tower towers[M_TOWERS];
 TowerInit:for (int i = 0; i < M_TOWERS; i++) {
#pragma HLS UNROLL
    int channel = i/18;
    int bitLo = 10*(i%18);
    int bitHi = bitLo + 10;
    ap_uint<192> link = link_in[channel]; 
    ap_uint<10> u_et(link.range(bitHi,bitLo));
    int et(u_et);
    towers[i].et = et;
  }
 
  Jet jets[M_JET];
#pragma HLS ARRAY PARTITION variable=jets complete dim=1
 JetInit:for (int i = 0; i < M_JET; i++) {
#pragma HLS UNROLL
    jets[i].et = 0;
    jets[i].peak_et = 0;
    jets[i].ieta = 0;
    jets[i].iphi = 0;
    jets[i].isSet = false;
  }

  // null algo specific pragma: avoid fully combinatorial algo by specifying min latency
  // otherwise algorithm clock input (ap_clk) gets optimized away
#pragma HLS latency min=3
  findJets(towers,jets);

//  OutputWrite:for (int i = 0; i < M_JET; i++) {
// #pragma HLS UNROLL
//     Jet& jet = jets[i];
//     if ( !jet.isSet ) continue;
//     // printf("Jet %i: (%i,%i,%i)\n",i,jet.iphi,jet.ieta,jet.et);
//     tmp_link_out[i].range(5,0) = ap_uint<6>(jet.iphi);
//     tmp_link_out[i].range(11,6) = ap_uint<6>(jet.ieta);
//     tmp_link_out[i].range(21,12) = ap_uint<10>(jet.et);
//   }

//  OutputCopy:for(int i = 0; i < N_CH_OUT; i++){
//     link_out[i] = tmp_link_out[i];
//   }
}
