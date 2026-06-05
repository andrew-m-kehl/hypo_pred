#!/bin/bash

STLE=stle
MKSIGNAL=mksignal

SCRIPT_DIR="${BASH_SOURCE%/*}"
pushd ${SCRIPT_DIR}

SRC_DIR="../../.."
if [ -f "${SRC_DIR}/examples/paths.sh" ]; then
  source "${SRC_DIR}/examples/paths.sh"
fi

"$MKSIGNAL" 0 1000 1 1000 decay 1 250 250 0 > localmax.csv
"$STLE" localmax.csv -f x0 -os 1 -osf g -osn x > localmax_in.txt
"$STLE" localmax.csv -ff localmax_max.stl -os 1 -osf g -osn "On[0,85] Max x" > localmax_max.txt
"$STLE" localmax.csv -ff localmax.stl -os 1 -osf g -osn "Localmax[0,85]" > localmax_out.txt
gnuplot localmax.gnuplot

popd
