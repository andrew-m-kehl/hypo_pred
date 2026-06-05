STLE=stle
MKSIGNAL=mksignal

set SCRIPT_DIR=%~dp0
pushd %SCRIPT_DIR%

set SRC_DIR=..\..\..
call %SRC_DIR%\examples\paths.bat

call %MKSIGNAL% 0 3000 1 1000 decay 0.3679 250 250 0.004 > stabilize-until.csv
call %STLE% stabilize-until.csv -f "x0" -os 1 -osf g -osn "x" > stabilize_in.txt
call %STLE% stabilize-until.csv -f "(- (Lookup 1 0 x0) x0)" -os 1 -osf g -osn "x'" > stabilize_deriv.txt
call %STLE% stabilize-until.csv -f "(<= (Until (200 inf) inf (- (Max x0) (Min x0)) (>= (abs (- (Lookup 1 0 x0) x0)) 0.1)) 0.1)" -os 1 -osf g -osn "stable" > stabilize_out.txt
call gnuplot stabilize-until.gnuplot

popd