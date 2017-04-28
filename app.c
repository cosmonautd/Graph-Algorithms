#include <stdio.h>
#include <stdlib.h>

#include "graph.h"

void main() {

    struct Graph* G = new_graph_from_file("graph_files/G.graph");
    print_graph_info(G);

    printf("\n*************************************************************\n");
    printf("*************************************************************\n\n");

    struct Component* component = connected_components(G);

    printf("Found %d connected components\n\n", count_connected_components(component));
    print_connected_components(component);

    free_graph(G);
}
