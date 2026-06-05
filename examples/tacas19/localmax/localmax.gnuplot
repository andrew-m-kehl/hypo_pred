set terminal pdf lw 2 size 11cm,6.5cm
set output 'localmax.pdf'
set xrange [0:550]
set yrange [-1.25:1.25]
set key right bottom
plot \
'localmax_in.txt' i 0 u 1:2 w steps lt rgb "red" title columnheader(1), \
'localmax_max.txt' i 0 u 1:2 w steps lt rgb "blue" title columnheader(1), \
'localmax_out.txt' i 0 u 1:2 w steps lt rgb 'orange' title columnheader(1), \
