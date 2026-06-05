#!/bin/bash

STLE=stle
MKSIGNAL=mksignal

SCRIPT_DIR="${BASH_SOURCE%/*}"
pushd ${SCRIPT_DIR}

SRC_DIR="../../.."
if [ -f "${SRC_DIR}/examples/paths.sh" ]; then
  source "${SRC_DIR}/examples/paths.sh"
fi

"$MKSIGNAL" 0 1000 1 125 gauss 1 50 10 > spike.csv
"$STLE" spike.csv -f "x0" -os 1 -osf g -osn "x" > spike_in.txt
"$STLE" spike.csv -ff spike.stl -os 1 -osf g -osn "Spike start" > spike_out.txt
"$STLE" spike.csv -ff spike_stlib.stl -os 1 -osf g -osn "Spike start" > spike_stlib_out.txt
gnuplot spike.gnuplot
gnuplot spike_stlib.gnuplot

popd
