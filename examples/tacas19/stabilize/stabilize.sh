#!/bin/bash

STLE=stle
MKSIGNAL=mksignal

SCRIPT_DIR="${BASH_SOURCE%/*}"
pushd ${SCRIPT_DIR}

SRC_DIR="../../.."
if [ -f "${SRC_DIR}/examples/paths.sh" ]; then
  source "${SRC_DIR}/examples/paths.sh"
fi

"$MKSIGNAL" 0 1000 1 1000 decay 0.3679 250 250 0.004 > stabilize.csv
"$STLE" stabilize.csv -f x0 -os 1 -osf g -osn x > stabilize_in.txt
"$STLE" stabilize.csv -ff stabilize_max.stl -os 1 -osf g -osn "On[0,200] Max x" > stabilize_max.txt
"$STLE" stabilize.csv -ff stabilize_min.stl -os 1 -osf g -osn "On[0,200] Min x" > stabilize_min.txt
"$STLE" stabilize.csv -ff stabilize.stl -os 1 -osf g -osn "Stable[0,200]" > stabilize_out.txt
gnuplot stabilize.gnuplot

popd
