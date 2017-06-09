grapp: graph connectivity isomorphism spanningtrees shortestpaths
	gcc -g -o grapp grapp.c graph.o connectivity.o isomorphism.o spanningtrees.o shortestpaths.o

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

spanningtrees:
	gcc -c graph_algorithms/spanningtrees.c

shortestpaths:
	gcc -c graph_algorithms/shortestpaths.c

clean:
	if [ -f grapp ] ; then rm grapp ; fi
	if ls *.o >/dev/null 2>&1 ; then rm *.o ; fi
