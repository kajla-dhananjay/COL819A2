all: input_generator input_generate Graph GHSNode dot_graph executable execute drawgraph showgraphs doxygen make_pdf clean

input_generate: input_generator 
	./input_generator.out < generator_input.txt > generator_output.txt

input_generator: input_generator.cpp 
	g++ -DDebug input_generator.cpp -o input_generator.out

executable: main.cpp dot_graph Graph GHSNode
	g++ -DDebug -o ghs main.cpp dot_graph.o Graph.o GHSNode.o

Graph: Graph.h Graph.cpp
	g++ -c Graph.h Graph.cpp

GHSNode: GHSNode.h GHSNode.cpp
	g++ -c GHSNode.h GHSNode.cpp

dot_graph: dot_graph.h dot_graph.cpp
	g++ -c dot_graph.h dot_graph.cpp

execute: executable
	./ghs < generator_output.txt > out.txt 

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
	rm -rf *.h.gch html *.dot *.o latex 
