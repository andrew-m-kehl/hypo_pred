STLE=stle
MKSIGNAL=mksignal

set SCRIPT_DIR=%~dp0
pushd %SCRIPT_DIR%

set SRC_DIR=..\..\..
call %SRC_DIR%\examples\paths.bat

call %MKSIGNAL% 0 1000 1 1000 decay 1 250 250 0 > localmax.csv
call %STLE% localmax.csv -f x0 -os 1 -osf g -osn x > localmax_in.txt
call %STLE% localmax.csv -ff localmax_max.stl -os 1 -osf g -osn "On[0,85] Max x" > localmax_max.txt
call %STLE% localmax.csv -ff localmax.stl -os 1 -osf g -osn "Localmax[0,85]" > localmax_out.txt
call gnuplot localmax.gnuplot

popd