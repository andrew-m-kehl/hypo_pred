#!/bin/bash

SCRIPT_DIR="${BASH_SOURCE%/*}"
pushd ${SCRIPT_DIR}

localmax/localmax.sh
spike/spike.sh
stabilize/stabilize.sh
stabilize-until/stabilize-until.sh

pdfcrop localmax/localmax.pdf localmax-crop.pdf
pdfcrop spike/spike.pdf spike-crop.pdf
pdfcrop stabilize/stabilize.pdf stabilize-crop.pdf
pdfcrop sstabilize-until/tabilize-until.pdf stabilize-until-crop.pdf

popd

