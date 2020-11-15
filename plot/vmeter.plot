set title "Voltage"
set xlabel "Time (min)"
set ylabel "Voltage"
plot 'voltmeter.txt' using ($1/60):2  with lines title 'Voltage'
 
#pause -1
pause 10
reread
