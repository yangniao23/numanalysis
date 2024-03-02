set datafile separator ","
version = ""
name = "report3-1-input".version

set terminal pdf enhanced
set output name.".pdf"

#set terminal pngcairo
#set output name.".png"

set style fill solid
set autoscale
set size square
set key
set key outside right
set key width 0
set grid
set xr [0:3.5]
#set yr [-5.5:5.5]
set mxtics 10
set mytics
set xlabel "x"
set ylabel "y"
#set logscale x
set samples 12000

plot 1.96*exp(1.01*x) w l title "a_0 = 1.96, a_1 = 1.01", \
    name.".csv" u 1:2 w p title "points"