set datafile separator ","
N = 10
version = "-"
name = "2".version.N

set terminal pdf enhanced
set output name.".pdf"

#set terminal pngcairo
#set output name.".png"

set autoscale
set size square
set key
set key outside right
set key width 0
set grid
set xr [0:1]
#set yr [-5.5:5.5]
#set mxtics 10
set mytics
set xlabel "x"
set ylabel "y"
#set logscale x
set samples 12000

plot name.".csv" u 3:1 w p title "approx N=".N, \
exp(x) - exp(-6 * x) w l title "exact: y = e^x - e^{-6x}", \
2.716 w l title "y = 2.716 (x = 1.0)"
