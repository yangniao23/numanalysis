set datafile separator ","
N = 5
version = "-2-"
name = "1".version.N

set terminal pdf enhanced
set output name.".pdf"

#set terminal pngcairo
#set output name.".png"

set parametric
set autoscale
set size square
set key
set key outside right
set key width 0
set grid
#set xr [0:3.5]
#set yr [-5.5:5.5]
#set mxtics 10
set trange [0:2]
set mytics
set xlabel "x"
set ylabel "y"
#set logscale x
set samples 12000

plot name.".csv" u 1:2 w p title "approx N=".N, \
-2*exp(-2*t)-t+3, exp(-2*t)+1 w l title "exact"
