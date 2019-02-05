#!/bash/gnuplot -persist

reset

# png
set terminal pngcairo size 1000,800 enhanced font 'Verdana,10'
set output 'vector_fields1.png'

unset key

#Set palate from here
set palette defined ( 0 '#FFFFFF', \
1 '#ccde19',\
2 '#ff7000',\
3 '#ee0000',\
4 '#000000')

set autoscale xfix
set autoscale yfix
set autoscale cbfix

# functions to calculate the arrow offsets
h = 0.3 # vector size

xf(phi) = h*cos(phi)
yf(phi) = h*sin(phi)

plot 'outputtestvector.txt' \
    u ($1-xf($3)):($2-yf($3)):(2*xf($3)):(2*yf($3)):4 \
    with vectors head size 0.1,20,60 filled lc palette
    
reset

# png
set terminal pngcairo size 1000,800 enhanced font 'Verdana,10'
set output 'Heat_map.png'


#Set palate from here
set palette defined ( 0 '#000000', \
1 '#ee0000',\
2 '#ff7000',\
3 '#ccde19',\
4 '#FFFFFF')

set autoscale xfix
set autoscale yfix
set autoscale cbfix

plot 'outputtestheatmap.txt' matrix nonuniform with image notitle
