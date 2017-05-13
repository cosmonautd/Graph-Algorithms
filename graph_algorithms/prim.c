#include "prim.h"

struct Graph* prim(struct Graph* G) {

    assert(!G->oriented);

    int iv = 0;
    int* included = calloc(G->order, sizeof(int));
    included[iv] = 1;

    int* empty_adj_matrix = calloc(G->order * G->order, sizeof(int));
    struct Graph* T = new_graph(G->V, empty_adj_matrix, G->order);
    free(empty_adj_matrix);

    struct Edge *sorted_edges = sort_edges(G);

    int i;
    bool all_included = false;

    while(!all_included) {
        bool found = false;
        for(i=0; i < G->size && !found; i++) {
            int v1 = sorted_edges[i].v1;
            int v2 = sorted_edges[i].v2;
            int weight = sorted_edges[i].weight;
            if((included[v1] && !included[v2]) || (included[v2] && !included[v1])) {
                included[v1] = 1;
                included[v2] = 1;
                add_edge(T, v1, v2, weight);
                found = true;
            }
        }
        all_included = true;
        for(i=0; i < G->order; i++) if(included[i] == 0) all_included = false;
    }

    return T;
}
