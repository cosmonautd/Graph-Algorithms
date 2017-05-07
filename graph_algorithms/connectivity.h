#ifndef CONNECTIVITY_H_
#define CONNECTIVITY_H_

#include "../graph.h"

/*  Macros para escolha de algoritmos de busca em profundidade ou largura
*/
#define DEPTH_FIRST     0
#define BREADTH_FIRST   1

#ifndef USE_ADJ_LISTS
#define USE_ADJ_LISTS   2
#define USE_ADJ_MATRIX  3
#define USE_INC_MATRIX  4
#endif

struct Component {
    int order;
    int* elements;
    struct Component* next;
};

void dfs(struct Graph* G, int v, int* aux, int depth, int rep);
void bfs(struct Graph* G, int v, int* aux, int rep);
bool connected(struct Graph* G, int algorithm, int rep);
struct Component* connected_components(struct Graph* G);
int count_connected_components(struct Component* component);
void print_connected_components(struct Component* component);
void free_component(struct Component* component);

#endif
