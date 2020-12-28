set title "Charge I/V"
set xlabel "Time (s)"
set ylabel "Voltage"
set y2label "Charge Current"

set y2tics 0, 2 
set ytics nomirror
set autoscale y
set autoscale y2
plot 'celllog.txt' using ($1/60):19 axis x1y1 with lines title 'Stack Voltage', \
'' using ($1/60):20 axis x1y2 with lines title 'Charge Current'
	
 
#pause -1
pause 10
reread
