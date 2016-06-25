reset
set ylabel 'time(sec)'
set style fill solid
set title 'perfomance comparison'
set term png enhanced font 'Verdana,10'
set output 'runtime.png'

plot [:][:0.100]'output.txt' \
using 2:xtic(1) with histogram title 'original', \
'' using ($0-0.06):($2+0.001):2 with labels title ' ', \
\
'' using 3:xtic(1) with histogram title 'hash'  , \
'' using ($0+0.1):($3+0.001):3 with labels title ' ', \
\
'' using 4:xtic(1) with histogram title 'pool'  , \
'' using ($0+0.25):($4+0.001):4 with labels title ' ', \
\
'' using 5:xtic(1) with histogram title 'hash+pool'  , \
'' using ($0+0.4):($5+0.001):5 with labels title ' '
