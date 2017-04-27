#ifndef CONNECTIVITY_H_
#define CONNECTIVITY_H_

#include "../graph.h"

/*  Macros para escolha de algoritmos de busca em profundidade ou largura
*/
#define DEPTH_FIRST 0
#define BREADTH_FIRST 1

void dfs(struct Graph* G, int v, int* aux, int depth);
void bfs(struct Graph* G, int v, int* aux);
int is_connected(struct Graph* G, int algorithm);

#endif
