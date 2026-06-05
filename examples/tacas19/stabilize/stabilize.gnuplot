set terminal pdf lw 2 size 13cm,6.5cm
set output 'stabilize.pdf'
set xrange [0:750]
set yrange [-0.3:0.3]
set y2range [-1.25:1.25]
set y2tics 1
set key right bottom
plot \
'stabilize_in.txt' i 0 u 1:2 w steps lt rgb "red" title columnheader(1), \
'stabilize_max.txt' i 0 u 1:2 w steps lt rgb "blue" title columnheader(1), \
'stabilize_min.txt' i 0 u 1:2 w steps lt rgb "forest-green" title columnheader(1), \
'stabilize_out.txt' i 0 u 1:2 w steps axes x1y2 lt rgb 'orange' title columnheader(1), \
