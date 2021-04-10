CC = g++

CFLAGS = -Wall -DDebug -lpthread 

all: input_generator input_generate Graph.o GHSNode.o dot_graph.o ghs execute printoutput #drawgraph showgraphs doxygen make_pdf clean

input_generate: input_generator 
	./input_generator < generator_input.txt > generator_output.txt

input_generator: input_generator.cpp 
	$(CC) $(CFLAGS) input_generator.cpp -o input_generator

ghs: main.cpp dot_graph.o Graph.o GHSNode.o
	$(CC) $(CFLAGS) -o ghs main.cpp dot_graph.o Graph.o GHSNode.o

Graph.o: Graph.h Graph.cpp
	$(CC) $(CFLAGS) -c Graph.h Graph.cpp

GHSNode.o: GHSNode.h GHSNode.cpp
	$(CC) $(CFLAGS) -c GHSNode.h GHSNode.cpp

dot_graph.o: dot_graph.h dot_graph.cpp
	$(CC) $(CFLAGS) -c dot_graph.h dot_graph.cpp

execute: ghs
	./ghs < generator_output.txt > out.txt 

drawgraph: input_graph.dot mst.dot
	neato -Tpng input_graph.dot -o input_graph.png
	neato -Tpng mst.dot -o mst.png

printoutput: out.txt
	cat out.txt

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
