#!/bin/bash

set -x

STLE=stle
MKSIGNAL=mksignal

SCRIPT_DIR="${BASH_SOURCE%/*}"
pushd ${SCRIPT_DIR}

SRC_DIR="../../.."
if [ -f "${SRC_DIR}/examples/paths.sh" ]; then
  source "${SRC_DIR}/examples/paths.sh"
fi

$MKSIGNAL 0 10000 1 1000 decay 0.3679 250 250 0.004 > decay10k.csv
$MKSIGNAL 0 100000 1 1000 decay 0.3679 250 250 0.004 > decay100k.csv
$MKSIGNAL 0 1000000 1 1000 decay 0.3679 250 250 0.004 > decay1M.csv
$MKSIGNAL 0 10000 1 1000 decay 1 250 250 0 > sin10k.csv
$MKSIGNAL 0 100000 1 1000 decay 1 250 250 0 > sin100k.csv
$MKSIGNAL 0 1000000 1 1000 decay 1 250 250 0 > sin1M.csv
$MKSIGNAL 0 10000 1 125 gauss 1 50 10 > spike10k.csv
$MKSIGNAL 0 100000 1 125 gauss 1 50 10 > spike100k.csv
$MKSIGNAL 0 1000000 1 125 gauss 1 50 10 > spike1M.csv
time $STLE -db 1 -ff gf_stabilize.stl decay10k.csv -pc 2>&1
time $STLE -db 1 -ff gf_stabilize.stl decay100k.csv -pc 2>&1
time $STLE -db 1 -ff gf_stabilize.stl decay1M.csv -pc 2>&1
time $STLE -db 1 -ff gf_stabilize0.stl decay10k.csv -pc 2>&1
time $STLE -db 1 -ff gf_stabilize0.stl decay100k.csv -pc 2>&1
time $STLE -db 1 -ff gf_stabilize0.stl decay1M.csv -pc 2>&1
time $STLE -db 1 -ff gf_stabilize_until.stl decay10k.csv -pc 2>&1
time $STLE -db 1 -ff gf_stabilize_until.stl decay100k.csv -pc 2>&1
time $STLE -db 1 -ff gf_stabilize_until.stl decay1M.csv -pc 2>&1
time $STLE -db 1 -ff g_max_f_min.stl sin10k.csv -pc 2>&1
time $STLE -db 1 -ff g_max_f_min.stl sin100k.csv -pc 2>&1
time $STLE -db 1 -ff g_max_f_min.stl sin1M.csv -pc 2>&1
time $STLE -db 1 -ff g_above_f_below.stl sin10k.csv -pc 2>&1
time $STLE -db 1 -ff g_above_f_below.stl sin100k.csv -pc 2>&1
time $STLE -db 1 -ff g_above_f_below.stl sin1M.csv -pc 2>&1
time $STLE -db 1 -ff f_spike.stl spike10k.csv -pc 2>&1
time $STLE -db 1 -ff f_spike.stl spike100k.csv -pc 2>&1
time $STLE -db 1 -ff f_spike.stl spike1M.csv -pc 2>&1
time $STLE -db 1 -ff f_spike_stlib.stl spike10k.csv -pc 2>&1
time $STLE -db 1 -ff f_spike_stlib.stl spike100k.csv -pc 2>&1
time $STLE -db 1 -ff f_spike_stlib.stl spike1M.csv -pc 2>&1

popd
