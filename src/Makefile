all: input_generator input_generate Graph dot_graph executable execute drawgraph #doxygen copy_pdf

input_generate: input_generator 
	./input_generator.out < generator_input.txt > generator_output.txt

input_generator: input_generator.cpp 
	g++ -DDebug input_generator.cpp -o input_generator.out

executable: main.cpp dot_graph Graph
	g++ -o ghs main.cpp dot_graph.o Graph.o

Graph: Graph.h Graph.cpp
	g++ -c Graph.h Graph.cpp

dot_graph: dot_graph.h dot_graph.cpp
	g++ -c dot_graph.h dot_graph.cpp

execute: executable
	./ghs < generator_output.txt > out.txt 

drawgraph: dotTarget.dot
	neato -Tpng input_graph.dot -o input_graph.png
	neato -Tpng mst.dot -o mst.png




doxygen: Doxyfile input_generator.cpp
	doxygen Doxyfile

copy_pdf: latex/refman.pdf
	cp latex/refman.pdf input_generator_documentation.pdf

clean:
