#!/bin/sh
if [ ! -d reports ]; then mkdir reports; fi
date=$(date '+%Y%m%d')
label=$1
if [ -z $label ]; then label="report"; fi
    
directory=${date}_$label
output=reports/$directory/
if [ ! -d $output ]; then mkdir $output; fi
cp vivado_hls.log $output
cp proj/solution1/syn/report/* $output


