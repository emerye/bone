set title "Stack Voltage"
set xlabel "Time (s)"
set ylabel "Voltage"
#set yrange [52000:]
plot 'celllog.txt' using 1:19 with lines title 'Stack Voltage'
 
#pause -1
pause 10
reread

