CC = g++

CFLAGS = -Wall -Wextra -pedantic -DDebug -lpthread 

#all: ghs kruskal documentation

all: execghs execkruskal visualize

execghs: in.txt ghs 
	./ghs < in.txt > out_ghs.txt
	cat out_ghs.txt

execkruskal: in.txt kruskal
	./kruskal < in.txt > out_kruskal.txt
	cat out_kruskal.txt

documentation: doxygen make_pdf

ghs: main.cpp GHSNode.o
	$(CC) $(CFLAGS) -o ghs main.cpp GHSNode.o

kruskal: in.txt input_generator main_bu.cpp dot_graph.o Graph.o
	$(CC) $(CFLAGS) -o kruskal main_bu.cpp dot_graph.o Graph.o

in.txt: input_generator generator_input.txt 
	./input_generator < generator_input.txt > in.txt

input_generator: input_generator.cpp 
	$(CC) $(CFLAGS) input_generator.cpp -o input_generator

GHSNode.o: GHSNode.h GHSNode.cpp
	$(CC) $(CFLAGS) -c GHSNode.h GHSNode.cpp

dot_graph.o: dot_graph.cpp dot_graph.h
	$(CC) $(CFLAGS) -c dot_graph.cpp dot_graph.h

Graph.o: Graph.cpp Graph.h
	$(CC) $(CFLAGS) -c Graph.cpp Graph.h	

visualize: drawgraph showgraphs

drawgraph: input_graph.dot mst.dot
	neato -Tpng input_graph.dot -o input_graph.png
	neato -Tpng mst.dot -o mst.png

showgraphs: mst.png input_graph.png
	eog mst.png &
	eog input_graph.png &

doxygen: Doxyfile input_generator.cpp
	doxygen Doxyfile

make_pdf: latex/refman.tex
	cd latex && pdflatex refman.tex
	cp latex/refman.pdf doxygen_doc.pdf

clean: 
	mv generator_input.txt xyz.tmp
	rm -rf *.h.gch html *.dot *.o latex *.txt *.png 
	mv xyz.tmp generator_input.txt
