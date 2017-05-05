app: graph connectivity isomorphism kruskal
	gcc -g -o app app.c graph.o connectivity.o isomorphism.o kruskal.o

graph:
	gcc -c graph.c

connectivity:
	gcc -c graph_algorithms/connectivity.c

isomorphism:
	gcc -c graph_algorithms/isomorphism.c

kruskal:
	gcc -c graph_algorithms/kruskal.c

clean:
	rm app *.o
