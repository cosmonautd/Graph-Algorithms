app: graph connectivity kruskal
	gcc -g -o app app.c graph.o connectivity.o kruskal.o

graph:
	gcc -c graph.c

connectivity:
	gcc -c graph_algorithms/connectivity.c

kruskal:
	gcc -c graph_algorithms/kruskal.c

clean:
	rm app *.o
