set terminal epslatex size 2.2,2.2 standalone color colortext 8
set output 'CoM_disp_sq.tex'

set size square

unset key

stats 'CoM_displacement.dat' u 1 name "x" nooutput
stats 'CoM_displacement.dat' u 5 name "y" nooutput

set xtics offset 0,-0.3
# unset xtics
# set xtics ( '\fontsize{8}{60}\selectfont$0.0$' 0.0, '\fontsize{8}{60}\selectfont$0.5$' 0.5, '\fontsize{8}{60}\selectfont$1.0$' 1.0)
set xtics x_max/3
set xlabel '\fontsize{10}{60}\selectfont$t/\tau$'
set xlabel offset 0.0,-0.5

# set format y "%.1f"

set ytics y_max/3
set ylabel '\fontsize{10}{60}\selectfont${ds^2}_{CoM}$ (nm)'
set ylabel offset 0.4,0.0


plot 'CoM_displacement.dat' u 1:5 w l

