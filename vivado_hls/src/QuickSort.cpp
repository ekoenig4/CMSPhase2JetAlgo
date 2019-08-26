#include "QuickSort.hh"
#include "stdlib.h"

static int num_jets = 0;
static int niter = 0;

void print(Jet* jets[M_JET]) {
  for (int i = 0; i < num_jets; i++) {
    if (jets[i] != 0)
      printf(" %i |",jets[i]->ecal_et);
  }
  printf("\n");
}

void swap(Jet* jets[M_JET],int i,int j) {
  if (i == j) return;
  Jet* tmp = jets[i];
  jets[i] = jets[j];
  jets[j] = tmp;
  printf("-------"); print(jets);
}

int Partition(Jet* jets[M_JET],int start,int end) {
  int pivot = jets[end]->ecal_et;
  int i = start - 1;
  for (int j = start; j < end; j++) {
    if ( jets[j]->ecal_et >= pivot ) {
      i++;
      swap(jets,i,j);
    }
  }
  swap(jets,i + 1,end);
  return i + 1;
}

void QuickSortHelper(Jet* jets[M_JET],int start,int end) {
  if (start >= end) return;
  printf("Iter %i: ",niter); print(jets); niter++;
  int pi = Partition(jets,start,end);
  QuickSortHelper(jets,start,pi-1);
  QuickSortHelper(jets,pi,end);
}


void QuickSort(Jet* jets[M_JET],int njets) {
  num_jets = njets;
  QuickSortHelper(jets,0,njets-1);
  printf("nJets: %i nIters: %i\n",njets,niter);
}
