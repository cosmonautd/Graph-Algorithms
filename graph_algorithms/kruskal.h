#ifndef KRUSKAL_H_
#define KRUSKAL_H_

#include "../graph.h"

int edge_weight_comparator(const void *x, const void *y);
struct Edge* sort_edges(struct Graph* G);
struct Graph* kruskal(struct Graph* G);

#endif
