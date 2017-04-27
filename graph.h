#ifndef GRAPH_H_
#define GRAPH_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

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
    directed: indica se o grafo é orientado (1) ou não (0)
*/
struct Graph {
    int* V;
    int* ADJ_MATRIX;
    int* INC_MATRIX;
    int** ADJ_LISTS;
    struct Edge* EDGE_LIST;
    int order;
    int size;
    int directed;
};

#include "graph_algorithms/connectivity.h"
#include "graph_algorithms/kruskal.h"

struct Graph* new_graph(int* V, int* ADJ_MATRIX, int order);
struct Graph* new_graph_from_file(const char* path);
int get_index(int i, int j, int rowsize);
int get_vertex_degree(int vertex, struct Graph* G);
void print_vertices(struct Graph* G);
void print_adj_matrix(struct Graph* G);
void print_inc_matrix(struct Graph* G);
void print_adj_lists(struct Graph* G);
void print_graph_info(struct Graph* G);
int get_graph_order(struct Graph* G);
int get_graph_size(struct Graph* G);
int is_directed(struct Graph* G);
int is_tree(struct Graph* G);
void add_edge(struct Graph* G, int v1, int v2, int weight);
void free_graph(struct Graph* G);

#endif