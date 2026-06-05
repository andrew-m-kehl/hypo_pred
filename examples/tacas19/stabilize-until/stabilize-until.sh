#!/bin/bash

STLE=stle
MKSIGNAL=mksignal

SCRIPT_DIR="${BASH_SOURCE%/*}"
pushd ${SCRIPT_DIR}

SRC_DIR="../../.."
if [ -f "${SRC_DIR}/examples/paths.sh" ]; then
  source "${SRC_DIR}/examples/paths.sh"
fi

"$MKSIGNAL" 0 3000 1 1000 decay 0.3679 250 250 0.004 > stabilize-until.csv
"$STLE" stabilize-until.csv -f "x0" -os 1 -osf g -osn "x" > stabilize_in.txt
"$STLE" stabilize-until.csv -f "(- (Lookup 1 0 x0) x0)" -os 1 -osf g -osn "x'" > stabilize_deriv.txt
"$STLE" stabilize-until.csv -f "(<= (Until (200 inf) inf (- (Max x0) (Min x0)) (>= (abs (- (Lookup 1 0 x0) x0)) 0.1)) 0.1)" -os 1 -osf g -osn "stable" > stabilize_out.txt
gnuplot stabilize-until.gnuplot

popd
