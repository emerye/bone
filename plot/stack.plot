set title "Stack Voltage"
set xlabel "Time (s)"
set ylabel "Voltage"
plot 'celllog.txt' using 1:19 with lines title 'Stack Voltage'
 
pause -1
