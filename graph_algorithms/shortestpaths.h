#ifndef SHORTESTPATHS_H_
#define SHORTESTPATHS_H_

#include "../graph.h"

#define INF  INT32_MAX - 1
#define NONE -1
#define INFSUM(x, y) (((x) == (INF) || (y) == (INF)) ? (INF) : (x+y))

struct VertexQueue {
    int* V;
    int* D;
    int len;
};

void initss(struct Graph* G, int source, int* d, int* p);
void relax(int v1, int v2, int w, int* d, int* p);
bool bellmanford(struct Graph* G, int source, int* d, int* p);
void initvq(struct VertexQueue* vq, struct Graph* G);
void add2vq(struct VertexQueue* vq, int v, int d);
int extractvq(struct VertexQueue* vq);
bool dijkstra(struct Graph* G, int source, int* d, int* p);
bool shortestpath(struct Graph* G, int source, int terminal, int* path, int* pathorder, int* d, int* p);
bool allpairs(struct Graph* G, int** d, int** p);

#endif
