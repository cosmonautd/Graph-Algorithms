grapp: graph connectivity isomorphism kruskal prim
	gcc -g -o grapp grapp.c graph.o connectivity.o isomorphism.o kruskal.o prim.o

install:
	cp grapp /usr/bin
	chmod +x /usr/bin/grapp

uninstall:
	rm /usr/bin/grapp

graph:
	gcc -c graph.c

connectivity:
	gcc -c graph_algorithms/connectivity.c

isomorphism:
	gcc -c graph_algorithms/isomorphism.c

kruskal:
	gcc -c graph_algorithms/kruskal.c

prim:
	gcc -c graph_algorithms/prim.c

clean:
	if [ -f grapp ] ; then rm grapp ; fi
	if ls *.o >/dev/null 2>&1 ; then rm *.o ; fi
