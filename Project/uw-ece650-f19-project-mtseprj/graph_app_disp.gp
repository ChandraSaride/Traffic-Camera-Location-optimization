
## An example of a plot with yerror bars
##

# To create the graph use
# gnuplot ex1.gp

## uncomment the next two lines to save plot into a file
set terminal png enhanced  fontscale 1.0 size 600, 400
set output 'graph_app.png'

# Set titles of the graph and axis
set style data lines
#set title "Approximation ration for VC-1,VC-2"
set xlabel "Vertices"
set ylabel "Approximation Ratio"
set xtics 0,5
set ytics 0,0.2
set yrange [0.8:2]

# plot the file
# restrict the plot to xrange between 0 and 10
# read data from cpu.dat
# use first 3 columns of the file
# set the title of the plot
# connect dots with lines
# AND
# plot the same file, but without title, and only show error bars
# the combination of the two plots gives both a connecting line and yerror bars
plot [0:20] "vc1_app_file.dat" using 1:2:3 title "VC-1" with lines,\
 "vc1_app_file.dat" using 1:2:3 notitle with yerr,\
 "vc2_app_file.dat" using 1:2:3 title "VC-2" with lines,\
 "vc2_app_file.dat" using 1:2:3 notitle with yerr
	
