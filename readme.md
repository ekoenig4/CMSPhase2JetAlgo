# CMS Phase 2 Jet Algorithm

This project uses APxL1TAlgoDev from Ales Svetek (https://github.com/APxL1TAlgoDev)

## STEP-1: Vivado HLS
```
## Set up Xilinx env and licensing (uwlogin.cern.ch specific)
source /data/tools/Xilinx/Vivado/2018.2/settings64.sh (/opt/Xilinx/Vivado/2018.2/settings64.sh - on UW Madison machine)
source /data/setup_xilinx_lic.sh

## CMSSW enviroment used for plotting software
cmsrel CMSSW_10_2_10
cd CMSSW_10_2_10/src/
cmsenv
mkdir -p CMSPhase2HLS
cd CMSPhase2HLS

git lfs install
git version
git-lfs version
## Clone APx algo repos
git clone --recursive git@github.com:APxL1TAlgoDev/APx_Gen0_Algo.git
mkdir APx_Gen0_Algo/build
cd APx_Gen0_Algo/VivadoHls/ 
## Keep all your Vivado HLS projects in this dir and follow file structure in null_algo_unpacked
## Good way to start is copy null_algo_unpacked; rename and start making changes

## Complete build example for reference “null_algo_unpacked” project is below:
cd null_algo_unpacked/vivado_hls #Currently using null_algo_unpacked, make changes accordingly to use null_algo_stream

## Build HLS algo project
vivado_hls -f run_hls.tcl #more parameters in section below
```

## STEP-2: Running JetAlgo Project
```
cd $CMSSW_BASE/src/CMSPhase2HLS/APx_Gen0_Algo/VivadoHls
git clone https://github.com/ekoenig4/CMSPhase2JetAlgo.git
cd CMSPhase2JetAlgo/vivado_hls
vivado_hls -f run_hls.tcl synth=0 csim=1 cosim=0 export=0 tv=jetalgo_tv_0
```

# Vivado_hls command:
Internally, the “run_hls.tcl” script uses 5 parameters that steer the build process:
```
synth: 1 (run) OR 0 (skip): do C synthesis
csim: 1 (run) OR 0 (skip): run C simulation
cosim: 1 (run) OR 0 (skip): run C/RTL cosimulation
export: 1 (run) OR 0 (skip): export algo HDL (previously we were using *.dcp)
tv: specify test vector (defaults is test1, they are placed in data directory and path needed to updated in sources.tcl)
```
By default if you pass no parameters to the build script, it runs with the following configuration:
```
vivado_hls -f run_hls.tcl synth=1 csim=1 cosim=1 export=1 tv=test1
```

If one wants to execute only C simulation (useful for quickly iterating on many initial algo changes), you can do so:
```
vivado_hls -f run_hls.tcl synth=0 csim=1 cosim=0 export=0 tv=test1
```
As another example, if you want to skip both simulations steps altogether and just build 
the project, you can do that as well:
```
vivado_hls -f run_hls.tcl csim=0 cosim=0 (this will run synth and export the HLS IP while skipping both sim steps)
```
