set terminal pdf

set xlabel 'Mean Call Arrival Rate N{/Symbol l}' font ",16"
set ylabel 'Blocking probability (O)' font ",16"

set key top right font ",10"

#set key at 99, 30

set xrange [0:150]
set yrange [0:45]

set tics font ", 16"

set output 'fig_blocking_prob.pdf'

plot \
'block_prob_no_sharing_sim.txt'     				 t   'W/o Sha., Sim. {/Symbol t}=25, {/Symbol b}_{tot} = 30'    ps 0.3  dt "-"  lc rgb      "blue"      with linespoints,\
'block_prob_no_sharing_ana.txt'   				     t   'W/o Sha., Ana. {/Symbol t}=25, {/Symbol b}_{tot} = 30'    ps 0.3  dt "-"  lc rgb      "blue"      with linespoints,\
'block_prob_sharing_ana.txt'        				 t   'W/  Sha., Ana. {/Symbol t}=25, {/Symbol b}_{tot} = 30'    ps 0.3  dt "-"  lc rgb      "red"       with linespoints,\
'block_prob_sharing_sim.txt'       				     t   'W/  Sha., Sim. {/Symbol t}=25, {/Symbol b}_{tot} = 30'    ps 0.3  dt "-"  lc rgb      "red"       with linespoints,\
