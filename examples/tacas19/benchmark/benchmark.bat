set STLE=stle
set MKSIGNAL=mksignal

set SCRIPT_DIR=%~dp0
pushd %SCRIPT_DIR%

set SRC_DIR=..\..\..
call %SRC_DIR%\examples\paths.bat

call %MKSIGNAL% 0 10000 1 1000 decay 0.3679 250 250 0.004 > decay10k.csv
call %MKSIGNAL% 0 100000 1 1000 decay 0.3679 250 250 0.004 > decay100k.csv
call %MKSIGNAL% 0 1000000 1 1000 decay 0.3679 250 250 0.004 > decay1M.csv
call %MKSIGNAL% 0 10000 1 1000 decay 1 250 250 0 > sin10k.csv
call %MKSIGNAL% 0 100000 1 1000 decay 1 250 250 0 > sin100k.csv
call %MKSIGNAL% 0 1000000 1 1000 decay 1 250 250 0 > sin1M.csv
call %MKSIGNAL% 0 10000 1 125 gauss 1 50 10 > spike10k.csv
call %MKSIGNAL% 0 100000 1 125 gauss 1 50 10 > spike100k.csv
call %MKSIGNAL% 0 1000000 1 125 gauss 1 50 10 > spike1M.csv
call %STLE% -db 1 -ff gf_stabilize.stl decay10k.csv -pc 2>&1
call %STLE% -db 1 -ff gf_stabilize.stl decay100k.csv -pc 2>&1
call %STLE% -db 1 -ff gf_stabilize.stl decay1M.csv -pc 2>&1
call %STLE% -db 1 -ff gf_stabilize0.stl decay10k.csv -pc 2>&1
call %STLE% -db 1 -ff gf_stabilize0.stl decay100k.csv -pc 2>&1
call %STLE% -db 1 -ff gf_stabilize0.stl decay1M.csv -pc 2>&1
call %STLE% -db 1 -r -ff gf_stabilize0.stl decay10k.csv -pc 2>&1
call %STLE% -db 1 -r -ff gf_stabilize0.stl decay100k.csv -pc 2>&1
call %STLE% -db 1 -r -ff gf_stabilize0.stl decay1M.csv -pc 2>&1
call %STLE% -db 1 -ff gf_stabilize_until.stl decay10k.csv -pc 2>&1
call %STLE% -db 1 -ff gf_stabilize_until.stl decay100k.csv -pc 2>&1
call %STLE% -db 1 -ff gf_stabilize_until.stl decay1M.csv -pc 2>&1
call %STLE% -db 1 -ff g_max_f_min.stl sin10k.csv -pc 2>&1
call %STLE% -db 1 -ff g_max_f_min.stl sin100k.csv -pc 2>&1
call %STLE% -db 1 -ff g_max_f_min.stl sin1M.csv -pc 2>&1
call %STLE% -db 1 -ff g_above_f_below.stl sin10k.csv -pc 2>&1
call %STLE% -db 1 -ff g_above_f_below.stl sin100k.csv -pc 2>&1
call %STLE% -db 1 -ff g_above_f_below.stl sin1M.csv -pc 2>&1
call %STLE% -db 1 -r -ff g_above_f_below.stl sin10k.csv -pc 2>&1
call %STLE% -db 1 -r -ff g_above_f_below.stl sin100k.csv -pc 2>&1
call %STLE% -db 1 -r -ff g_above_f_below.stl sin1M.csv -pc 2>&1
call %STLE% -db 1 -ff f_spike.stl spike10k.csv -pc 2>&1
call %STLE% -db 1 -ff f_spike.stl spike100k.csv -pc 2>&1
call %STLE% -db 1 -ff f_spike.stl spike1M.csv -pc 2>&1
call %STLE% -db 1 -ff f_spike_stlib.stl spike10k.csv -pc 2>&1
call %STLE% -db 1 -ff f_spike_stlib.stl spike100k.csv -pc 2>&1
call %STLE% -db 1 -ff f_spike_stlib.stl spike1M.csv -pc 2>&1
call %STLE% -db 1 -r -ff f_spike_stlib.stl spike10k.csv -pc 2>&1
call %STLE% -db 1 -r -ff f_spike_stlib.stl spike100k.csv -pc 2>&1
call %STLE% -db 1 -r -ff f_spike_stlib.stl spike1M.csv -pc 2>&1

popd