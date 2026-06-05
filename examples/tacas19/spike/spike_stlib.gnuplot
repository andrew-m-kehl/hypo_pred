set terminal pdf lw 2
set output 'spike_stlib.pdf'
set xrange [0:500]
set yrange [-0.1:1.1]
plot \
'spike_stlib_out.txt' i 0 u 1:2 w steps lt rgb 'red' title columnheader(1), \
'spike_in.txt' i 0 u 1:2 w steps lt rgb 'blue' title columnheader(1), \
