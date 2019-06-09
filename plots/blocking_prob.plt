set terminal pdf

set xlabel 'Mean call rate {/Symbol r}' font ",16"
set ylabel 'Blocking probability' font ",16"

set key bottom right font ",14"
set xrange [0:200]
set yrange [0:100]

set tics font ", 16"

set output 'fig_blocking_probability.pdf'

plot \
'results_block_prob.txt'    t   'Block. Prob.'    ps 0.5  dt "-"  lc rgb  "red"      with linespoints,\