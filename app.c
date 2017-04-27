#include <stdio.h>
#include <stdlib.h>

#include "graph.h"

void main() {

    struct Graph* G = new_graph_from_file("G.graph");
    print_graph_info(G);

    printf("\n*************************************************************\n");
    printf("*************************************************************\n\n");

    struct Graph* G2 = new_graph(G->V, G->ADJ_MATRIX, G->order);
    print_graph_info(G2);

    free_graph(G);
    free_graph(G2);
}
