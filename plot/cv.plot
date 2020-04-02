set title "Cell Voltage"
set xlabel "Time (sec)"
set ylabel "Voltage"
plot 'celllog.txt' using 1:2 with lines title 'Cell1',\
'celllog.txt' using 1:3 with lines title 'Cell2',\
'celllog.txt' using 1:4 with lines title 'Cell3',\
'celllog.txt' using 1:5 with lines title 'Cell4',\
'celllog.txt' using 1:6 with lines title 'Cell5',\
'celllog.txt' using 1:7 with lines title 'Cell6',\
'celllog.txt' using 1:8 with lines title 'Cell7',\
'celllog.txt' using 1:9 with lines title 'Cell8', \
'celllog.txt' using 1:10 with lines title 'Cell9',\
'celllog.txt' using 1:11 with lines title 'Cell10',\
'celllog.txt' using 1:12 with lines title 'Cell11',\
'celllog.txt' using 1:13 with lines title 'Cell12',\
'celllog.txt' using 1:14 with lines title 'Cell13',\
'celllog.txt' using 1:15 with lines title 'Cell14'
 
pause -1
