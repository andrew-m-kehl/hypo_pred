STLE=stle
MKSIGNAL=mksignal

set SCRIPT_DIR=%~dp0
pushd %SCRIPT_DIR%

set SRC_DIR=..\..\..
call %SRC_DIR%\examples\paths.bat

call %MKSIGNAL% 0 1000 1 125 gauss 1 50 10 > spike.csv
call %STLE% spike.csv -f "x0" -os 1 -osf g -osn "x" > spike_in.txt
call %STLE% spike.csv -ff spike.stl -os 1 -osf g -osn "Spike start" > spike_out.txt
call %STLE% spike.csv -ff spike_stlib.stl -os 1 -osf g -osn "Spike start" > spike_stlib_out.txt
call gnuplot spike.gnuplot
call gnuplot spike_stlib.gnuplot

popd