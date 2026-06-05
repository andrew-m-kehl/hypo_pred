set SCRIPT_DIR=%~dp0
pushd %SCRIPT_DIR%

call stabilize\stabilize.bat
call localmax\localmax.bat
call spike\spike.bat
call stabilize-until\stabilize-until.bat

pdfcrop stabilize\stabilize.pdf stabilize-crop.pdf
pdfcrop localMax\localmax.pdf localmax-crop.pdf
pdfcrop spike\spike.pdf spike-crop.pdf
pdfcrop stabilize-until\stabilize-until.pdf stabilize-until-crop.pdf

popd