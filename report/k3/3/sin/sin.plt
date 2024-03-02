set datafile separator ","

set terminal pdf enhanced
set output "sin.pdf"

#set terminal pngcairo enhanced
#set output name.".png"


set autoscale
set size square
set key
set key right
set key width 0
set grid
set xr [1:100]
#set yr [0:1.1]
#set mxtics 10
set mytics
set xlabel "N"
set ylabel "error"
set logscale x
set logscale y

plot "trapezoid.csv" using 1:3 w lp title "trapezoid", \
        "simpson.csv" using 1:3 w lp title "simpson", \
    "romberg.csv" using 1:3 w lp title "romberg"