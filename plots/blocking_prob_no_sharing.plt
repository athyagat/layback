set terminal pdf

set xlabel 'Mean call rate {/Symbol l}' font ",10"
set ylabel 'Blocking probability' font ",10"

set key bottom right font ",14"
set xrange [0:200]
set yrange [0:100]

set tics font ", 16"

set output 'fig_blocking_probability_no_sharing.pdf'

plot \
'results_block_prob_no_sharing_tau_25_ana.txt'    t   'Analtical.'   ps 0.5  dt "-"  lc rgb            "red"      with linespoints,\
'results_block_prob_no_sharing_tau_25_sim.txt'    t   'Simulation.'    ps 0.5  dt "-"  lc rgb            "blue"     with linespoints,\



