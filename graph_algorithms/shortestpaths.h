#ifndef SHORTESTPATHS_H_
#define SHORTESTPATHS_H_

#include "../graph.h"


void init_ss(struct Graph* G, int source, int* d, int* p);
void relax(int v1, int v2, int w, int* d, int* p);
bool bellmanford(struct Graph* G, int source, int* d, int* p);

#endif
