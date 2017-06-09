#ifndef SPANNINGTREES_H_
#define SPANNINGTREES_H_

#include "../graph.h"

struct UnionFind {
    struct UnionFind* parent;
    int rank;
};

void MakeSet(struct UnionFind* x);
struct UnionFind* Find(struct UnionFind* x);
void Union(struct UnionFind* x, struct UnionFind* y);

int edge_weight_comparator(const void *x, const void *y);
struct Edge* sort_edges(struct Graph* G);
struct Graph* kruskal(struct Graph* G);
struct Graph* prim(struct Graph* G);

#endif
