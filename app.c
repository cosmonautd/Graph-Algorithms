#include <stdio.h>
#include <stdlib.h>

#include "graph.h"

void connected_components_example() {

    struct Graph* G = new_graph_from_file("graph_files/G2.graph");
    print_graph_info(G);

    printf("\n*************************************************************\n");
    printf("*************************************************************\n\n");

    struct Component* component = connected_components(G);

    printf("Found %d connected components\n\n", count_connected_components(component));
    print_connected_components(component);

    free_graph(G);
    free_component(component);
}

void isomorphism_example() {

    struct Graph* G1 = new_graph_from_file("graph_files/G4.graph");
    print_graph_info(G1);

    printf("\n*************************************************************\n");
    printf("*************************************************************\n\n");

    struct Graph* G2 = new_graph_from_file("graph_files/G5.graph");
    print_graph_info(G2);

    printf("\n*************************************************************\n");
    printf("*************************************************************\n\n");

    int* mapping = malloc(G1->order * sizeof(int));

    if(isomorphic(G1, G2, mapping, USE_INC_MATRIX)) {
        printf("G1 and G2 are isomorphic\n\n");
        printf("f: G2->G1\n");
        int i;
        for(i=0; i < G1->order; i++) printf(" f(%d) = %d\n", i, mapping[i]);
    }
    else printf("G1 and G2 are not isomorphic\n");

    free_graph(G1);
    free_graph(G2);
    free(mapping);
}

void kruskal_example() {

    struct Graph* G = new_graph_from_file("graph_files/G1.graph");
    print_graph_info(G);

    printf("\n*************************************************************\n");
    printf("*************************************************************\n\n");

    struct Graph* T = kruskal(G);
}

void main() {

    isomorphism_example();
}
