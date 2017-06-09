#include "shortestpaths.h"

void init_ss(struct Graph* G, int source, int* d, int* p) {
    int i;
    for(i=0; i < G->order; i++) {
        d[i] = INT32_MAX;
        p[i] = -1;
    }
    d[source] = 0;
    p[source] = source;
}

void relax(int v1, int v2, int w, int* d, int* p) {
    if(d[v2] > d[v1] + w) {
        d[v2] = d[v1] + w;
        p[v2] = v1;
    }
}

bool bellmanford(struct Graph* G, int source, int* d, int* p) {

    init_ss(G, source, d, p);

    int i, j;
    for(i=1; i < G->order - 1; i++) {
        for(j=0; j < G->size; j++) {
            int v1 = G->E[j].v1;
            int v2 = G->E[j].v2;
            int weight = G->E[j].weight;
            relax(v1, v2, weight, d, p);
        }
    }

    for(j=0; j < G->size; j++) {
        int v1 = G->E[j].v1;
        int v2 = G->E[j].v2;
        int weight = G->E[j].weight;
        if(d[v2] > d[v1] + weight) {
            return false;
        }
    }

    return true;
}
