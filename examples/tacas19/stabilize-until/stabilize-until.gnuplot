set terminal pdf
set output 'stabilize-until.pdf'
set yrange [-0.4:0.4]
set y2range [-1.25:1.25]
set y2tics 1
set key right bottom
plot \
'stabilize_in.txt' i 0 u 1:2 w steps title columnheader(1), \
'stabilize_out.txt' i 0 u 1:2 w steps axes x1y2 title columnheader(1), \


