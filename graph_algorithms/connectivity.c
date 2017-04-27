#include "connectivity.h"

/*  Função dfs()
    Realiza uma busca em profundidade no grafo G, a partir do vértice v.
    A entrada aux deve ser um vetor de comprimento igual à ordem de G.
    A váriavel depth indica a profundidade atual durante a busca.
    Ao fim da busca, os vértices alcançáveis a partir de v são indicados no
    vetor aux, de tal forma que o valor no índice w em aux é 1 se o vértice w
    é alcançável, e 0 caso contrário.
*/
void dfs(struct Graph* G, int v, int* aux, int depth, int rep) {

    if(depth == 0) {
        DEBUG_MESSAGE(("\nExecuting Depth First Traversal from vertex %d\n", v));
        dfs(G, v, aux, ++depth, rep);

    } else {
        aux[v] = 1;
        DEBUG_MESSAGE(("Visiting vertex %d\n", v));

        switch (rep) {
            case USE_ADJ_LISTS: {
                int i;
                for(i=0; i < get_vertex_degree(v, G); i++) {
                    int w = G->ADJ_LISTS[v][i];
                    if(aux[w] == 0) dfs(G, w, aux, depth++, rep);
                }
                break;
            }

            case USE_ADJ_MATRIX: {
                int i;
                for(i=0; i < G->order; i++) {
                    if(G->ADJ_MATRIX[get_index(v, i, G->order)] > 0) {
                        int w = i;
                        if(aux[w] == 0) dfs(G, w, aux, depth++, rep);
                    }
                }
                break;
            }

            case USE_INC_MATRIX: {
                break;
            }

            default: {
                int i;
                for(i=0; i < get_vertex_degree(v, G); i++) {
                    int w = G->ADJ_LISTS[v][i];
                    if(aux[w] == 0) dfs(G, w, aux, depth++, rep);
                }
                break;
            }
        }
    }
}

/*  Função bfs()
    Realiza uma busca em largura no grafo G, a partir do vértice v.
    A entrada aux deve ser um vetor de comprimento igual à ordem de G.
    Ao fim da busca, os vértices alcançáveis a partir de v são indicados no
    vetor aux, de tal forma que o valor no índice w em aux é 1 se o vértice w
    é alcançável, e 0 caso contrário.
*/
void bfs(struct Graph* G, int v, int* aux, int rep) {

    int* Q = calloc(G->order, sizeof(int));
    int Q_start = 0;
    int Q_end = 0;

    DEBUG_MESSAGE(("\nExecuting Breadth First Traversal from vertex %d\n", v));

    aux[v] = 1;
    DEBUG_MESSAGE(("Visiting vertex %d\n", v));
    Q[Q_end++] = v;

    while(Q_start < Q_end) {
        v = Q[Q_start++];

        switch (rep) {
            case USE_ADJ_LISTS: {
                int i;
                for(i=0; i < get_vertex_degree(v, G); i++) {
                    int w = G->ADJ_LISTS[v][i];
                    if(aux[w] == 0) {
                        Q[Q_end++] = w;
                        aux[w] = 1;
                        DEBUG_MESSAGE(("Visiting vertex %d\n", w));
                    }
                }
                break;
            }

            case USE_ADJ_MATRIX: {
                int i;
                for(i=0; i < G->order; i++) {
                    if(G->ADJ_MATRIX[get_index(v, i, G->order)] > 0) {
                        int w = i;
                        if(aux[w] == 0) {
                            Q[Q_end++] = w;
                            aux[w] = 1;
                            DEBUG_MESSAGE(("Visiting vertex %d\n", w));
                        }
                    }
                }
                break;
            }

            case USE_INC_MATRIX: {
                break;
            }

            default: {
                int i;
                for(i=0; i < get_vertex_degree(v, G); i++) {
                    int w = G->ADJ_LISTS[v][i];
                    if(aux[w] == 0) {
                        Q[Q_end++] = w;
                        aux[w] = 1;
                        DEBUG_MESSAGE(("Visiting vertex %d\n", w));
                    }
                }
                break;
            }
        }
    }
}

/*  Função is_connected()
    TLDR: Verifica se G é fortemente conexo.
    Testa se o grafo G é conexo, utilizando o algoritmo indicado (DEPTH_FIRST
    ou BREADTH_FIRST). Caso o grafo seja não-orientado, realiza uma busca a
    partir do vértice 0 e verifica se todos os vértices são alcançáveis. caso
    o grafo seja orientado, realiza uma busca para cada vértice de G e verifica
    se ao final de cada busca, todos os vértices são alcançáveis.
*/
int is_connected(struct Graph* G, int algorithm, int rep) {

    int i, j;

    if(algorithm == DEPTH_FIRST) {

        if(!is_directed(G)) {
            int* aux = calloc(G->order, sizeof(int));
            dfs(G, 0, aux, 0, rep);
            for(i=0; i < G->order; i++) if(aux[i] == 0) return 0;
            return 1;
        } else {
            for(i=0; i < G->order; i++) {
                int* aux = calloc(G->order, sizeof(int));
                dfs(G, i, aux, 0, rep);
                for(j=0; j < G->order; j++) if(aux[j] == 0) return 0;
            }
            return 1;
        }

    } else if(algorithm = BREADTH_FIRST) {

        if(!is_directed(G)) {
            int* aux = calloc(G->order, sizeof(int));
            bfs(G, 0, aux, rep);
            for(i=0; i < G->order; i++) if(aux[i] == 0) return 0;
            return 1;
        } else {
            for(i=0; i < G->order; i++) {
                int* aux = calloc(G->order, sizeof(int));
                bfs(G, i, aux, rep);
                for(j=0; j < G->order; j++) if(aux[j] == 0) return 0;
            }
            return 1;
        }
    }
}
