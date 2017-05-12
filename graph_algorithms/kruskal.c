#include "kruskal.h"

int edge_weight_comparator(const void *x, const void *y) {
    const struct Edge *E1 = (struct Edge *) x;
    const struct Edge *E2 = (struct Edge *) y;

    if(E1->weight < E2->weight) return -1;
    else if(E1->weight > E2->weight) return 1;
    else return 0;
}

struct Edge* sort_edges(struct Graph* G) {

    int i, j, k = 0;

    struct Edge* edges = malloc(G->size * sizeof(struct Edge));
    memcpy(edges, G->EDGE_LIST, G->size * sizeof(struct Edge));

    DEBUG_MESSAGE(("\nSorting Edges...\n"));
    qsort(edges, G->size, sizeof(struct Edge), edge_weight_comparator);

    DEBUG_MESSAGE(("\n"));
    for(i=0; i < G->size; i++)
        DEBUG_MESSAGE(("Edge: %d-%d, weight: %d\n", edges[i].v1, edges[i].v2, edges[i].weight));

    return edges;
}

void MakeSet(struct UnionFind* x) {
    x->parent = x;
    x->rank = 0;
}

struct UnionFind* Find(struct UnionFind* x) {
    if(x->parent != x) x->parent = Find(x->parent);
    return x->parent;
}

void Union(struct UnionFind* x, struct UnionFind* y) {
    struct UnionFind* xroot = Find(x);
    struct UnionFind* yroot = Find(y);
    if(xroot == yroot) return;
    if(xroot->rank < yroot->rank) xroot->parent = yroot;
    else if(xroot->rank > yroot->rank) yroot->parent = xroot;
    else {
        yroot->parent = xroot;
        xroot->rank++;
    }
}

struct Graph* kruskal(struct Graph* G) {

    assert(!G->oriented);

    int* empty_adj_matrix = calloc(G->order * G->order, sizeof(int));
    struct Graph* T = new_graph(G->V, empty_adj_matrix, G->order);

    free(empty_adj_matrix);

    struct Edge *sorted_edges = sort_edges(G);
}
