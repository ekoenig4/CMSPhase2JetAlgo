#include "QuickSort.hh"

void swap(Jet* jets[M_JET],int i,int j) {
  Jet* tmp = jets[i];
  jets[i] = jets[j];
  jets[j] = tmp;
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
  int pi = Partition(jets,start,end);
  QuickSortHelper(jets,start,pi-1);
  QuickSortHelper(jets,pi,end);
}


void QuickSort(Jet* jets[M_JET],int njets) {
  QuickSortHelper(jets,0,njets-1);
}
