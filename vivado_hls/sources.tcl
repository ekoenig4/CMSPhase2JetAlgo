## Set the top level module
set_top algo_unpacked
##
#### Add source code
add_files src/algo_unpacked.cpp
add_files src/JetFinder.cpp
add_files src/QuickSort.cpp
#
### Add testbed files
add_files -tb src/algo_unpacked_tb.cpp 

### Add test input files
add_files -tb data/jetalgo_tv_0_inp.txt
add_files -tb data/jetalgo_tv_1_inp.txt
add_files -tb data/jetalgo_tv_2_inp.txt
add_files -tb data/jetalgo_tv_3_inp.txt
add_files -tb data/jetalgo_tv_4_inp.txt
