#!/usr/bin/gnuplot -persist


#Vectormap
reset

# png
set terminal pngcairo size 1000,800 enhanced font 'Verdana,10'
set output 'vector_fields1.png'

unset key

#Set palate from here
set palette defined ( 0 '#FFFFFF', \
1 '#0125a2', \
2 '#f4f70e',\
3 '#ff7000',\
4 '#ee0000',\
5 '#000000')




set autoscale xfix
set autoscale yfix
#set autoscale cbfix
set dgrid3d 100,100,4





# functions to calculate the arrow offsets
h = 0.3 # vector size

xf(phi) = h*cos(phi)
yf(phi) = h*sin(phi)

plot 'outputtestvector.txt' \
    u ($1-xf($3)):($2-yf($3)):(2*xf($3)):(2*yf($3)):4 \
    with vectors head size 0.1,20,60 filled lc palette



#Heatmap
reset



# png
set terminal pngcairo size 1000,800 enhanced font 'Verdana,10'
set output 'Heat_map.png'

set autoscale xfix
set autoscale yfix
set autoscale cbfix



#Set palate from here
set palette defined ( 0 '#000000', \
1 '#0026a9',\
2 '#FFFFFF',\
3 '#ffda0b',\
4 '#cd1b03')


set autoscale xfix
set autoscale yfix
set autoscale cbfix

plot 'outputtestheatmap.txt' matrix nonuniform with image notitle
