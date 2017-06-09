#ifndef GRAPH_H_
#define GRAPH_H_

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

/*  Define se o programa é compilado com impressão de linhas de debug
*/
//#define DEBUG

#ifdef DEBUG
    #define DEBUG_MESSAGE(x) printf x
#else
    #define DEBUG_MESSAGE(x) ;
#endif

struct Edge {
    int v1;
    int v2;
    int weight;
};

/*  Estrutura Grafo
    V: vértices
    ADJ_MATRIX: matriz de adjacência
    ADJ_LISTS: listas de adjacência
    order: ordem do grafo (número de vértices)
    size: tamanho do grafo (número de arestas)
    oriented: indica se o grafo é orientado (1) ou não (0)
*/
struct Graph {
    int* V;
    int* ADJ_MATRIX;
    int* INC_MATRIX;
    int** ADJ_LISTS;
    struct Edge* E;
    int order;
    int size;
    bool oriented;
};

#include "graph_algorithms/connectivity.h"
#include "graph_algorithms/isomorphism.h"
#include "graph_algorithms/spanningtrees.h"
#include "graph_algorithms/shortestpaths.h"

struct Graph* new_graph(int* V, int* ADJ_MATRIX, int order);
struct Graph* new_random_graph(int order, float p, int min_weight, int max_weight);
struct Graph* new_graph_from_file(const char* path);
void printv(int *v, int len);
int get_index(int i, int j, int rowsize);
int get_vertex_degree(int vertex, struct Graph* G);
void print_vertices(struct Graph* G);
void print_adj_matrix(struct Graph* G);
void print_inc_matrix(struct Graph* G);
void print_adj_lists(struct Graph* G);
void print_graph_info(struct Graph* G);
void generate_dotfile(struct Graph* G, const char* filename, int* v_highlight);
void graphviz_show(struct Graph* G);
void graphviz_show_v(struct Graph* G, int* v_highlight);
int get_graph_order(struct Graph* G);
int get_graph_size(struct Graph* G);
bool oriented(struct Graph* G);
bool complete(struct Graph* G);
bool tree(struct Graph* G);
void add_edge(struct Graph* G, int v1, int v2, int weight);
void free_graph(struct Graph* G);

#endif
