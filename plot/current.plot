set title "Charge Current"
set xlabel "Time (s)"
set ylabel "Current"
plot 'celllog.txt' using 1:20 with lines title 'Charge Current'
 
pause -1
