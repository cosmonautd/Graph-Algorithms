#ifndef CONNECTIVITY_H_
#define CONNECTIVITY_H_

#include "../graph.h"

/*  Macros para escolha de algoritmos de busca em profundidade ou largura
*/
#define DEPTH_FIRST     0
#define BREADTH_FIRST   1

#define USE_ADJ_LISTS   2
#define USE_ADJ_MATRIX  3
#define USE_INC_MATRIX  4

void dfs(struct Graph* G, int v, int* aux, int depth, int rep);
void bfs(struct Graph* G, int v, int* aux, int rep);
int is_connected(struct Graph* G, int algorithm, int rep);

#endif
