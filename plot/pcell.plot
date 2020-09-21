set title "Cell Voltage"
set xlabel "Time (Minutes)"
set ylabel "Voltage"
plot 'celllog.txt' using ($1/60):2 with lines title 'Cell1',\
'celllog.txt' using ($1/60):3 with lines title 'Cell2',\
'celllog.txt' using ($1/60):4 with lines title 'Cell3',\
'celllog.txt' using ($1/60):5 with lines title 'Cell4',\
'celllog.txt' using ($1/60):6 with lines title 'Cell5',\
'celllog.txt' using ($1/60):7 with lines title 'Cell6',\
'celllog.txt' using ($1/60):8 with lines title 'Cell7',\
'celllog.txt' using ($1/60):9 with lines title 'Cell8', \
'celllog.txt' using ($1/60):10 with lines title 'Cell9',\
'celllog.txt' using ($1/60):11 with lines title 'Cell10',\
'celllog.txt' using ($1/60):12 with lines title 'Cell11',\
'celllog.txt' using ($1/60):13 with lines title 'Cell12',\
'celllog.txt' using ($1/60):14 with lines title 'Cell13',\
'celllog.txt' using ($1/60):15 with lines title 'Cell14'
 
#pause -1
pause 10 
reread 
