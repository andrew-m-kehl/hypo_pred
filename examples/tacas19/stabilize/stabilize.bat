STLE=stle
MKSIGNAL=mksignal

set SCRIPT_DIR=%~dp0
pushd %SCRIPT_DIR%

set SRC_DIR=..\..\..
call %SRC_DIR%\examples\paths.bat

call %MKSIGNAL% 0 1000 1 1000 decay 0.3679 250 250 0.004 > stabilize.csv
call %STLE% stabilize.csv -f x0 -os 1 -osf g -osn x > stabilize_in.txt
call %STLE% stabilize.csv -ff stabilize_max.stl -os 1 -osf g -osn "On[0,200] Max x" > stabilize_max.txt
call %STLE% stabilize.csv -ff stabilize_min.stl -os 1 -osf g -osn "On[0,200] Min x" > stabilize_min.txt
call %STLE% stabilize.csv -ff stabilize.stl -os 1 -osf g -osn "Stable[0,200]" > stabilize_out.txt
call gnuplot stabilize.gnuplot

popd