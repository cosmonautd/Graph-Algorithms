#include "shortestpaths.h"

void initss(struct Graph* G, int source, int* d, int* p) {
    int i;
    for(i=0; i < G->order; i++) {
        d[i] = INF;
        p[i] = NONE;
    }
    d[source] = 0;
    p[source] = source;
}

void relax(int v1, int v2, int w, int* d, int* p) {
    if(d[v1] + w < d[v2]) {
        d[v2] = d[v1] + w;
        p[v2] = v1;
    }
}

bool bellmanford(struct Graph* G, int source, int* d, int* p) {

    initss(G, source, d, p);

    int i, j;
    for(i=1; i < G->order - 1; i++) {
        for(j=0; j < G->size; j++) {
            int v1 = G->E[j].v1;
            int v2 = G->E[j].v2;
            int weight = G->E[j].weight;
            relax(v1, v2, weight, d, p);
        }
    }

    for(j=0; j < G->size; j++) {
        int v1 = G->E[j].v1;
        int v2 = G->E[j].v2;
        int weight = G->E[j].weight;
        if(d[v2] > d[v1] + weight) {
            return false;
        }
    }

    return true;
}

int minindex(int* v, int len) {
    int min = v[0];
    int index = 0;

    int i;
    for (i=1; i < len; i++) {
        if (v[i] < min) {
            min = v[i];
            index = i;
        }
    }

    return index;
}

void initvq(struct VertexQueue* vq, struct Graph* G) {
    vq->V = malloc(G->order*sizeof(int));
    vq->D = malloc(G->order*sizeof(int));
    vq->len = 0;
}

void add2vq(struct VertexQueue* vq, int v, int d) {
    vq->V[vq->len] = v;
    vq->D[vq->len] = d;
    vq->len++;
}

int extractvq(struct VertexQueue* vq) {
    int index = minindex(vq->D, vq->len);
    int v = vq->V[index];

    int i;
    for(i=index; i < vq->len - 1; i++) {
        vq->V[i] = vq->V[i+1];
        vq->D[i] = vq->D[i+1];
    }

    vq->len--;

    return v;
}

bool dijkstra(struct Graph* G, int source, int* d, int* p) {

    int i;
    for(i=0; i < G->size; i++) if(G->E[i].weight < 0) return false;

    initss(G, source, d, p);

    struct VertexQueue* vq = malloc(sizeof(struct VertexQueue));
    initvq(vq, G);
    for(i=0; i < G->order; i++)
        add2vq(vq, i, d[i]);

    while(vq->len > 0) {
        int v1 = extractvq(vq);
        int v1_degree = get_vertex_degree(v1, G);
        for(i=0; i < v1_degree; i++) {
            int v2 = G->ADJ_LISTS[v1][i];
            int w  = G->ADJ_MATRIX[get_index(v1, v2, G->order)];
            relax(v1, v2, w, d, p);
        }
    }

    return true;
}

bool shortestpath(struct Graph* G, int source, int terminal, int* path, int* pathorder, int* d, int* p) {

    int* pathtmp = malloc(G->order*sizeof(int));
    *pathorder = 0;

    int current = terminal;
    while(current != source && current != NONE) {
        pathtmp[*pathorder] = current;
        *pathorder = *pathorder + 1;
        current = p[current];
    }

    if(current == source) {
        pathtmp[*pathorder] = current;
        *pathorder = *pathorder + 1;
    }
    else {
        if(current == NONE) return false;
    }

    int i;
    path = realloc(path, (*pathorder)*sizeof(int));
    for(i=*pathorder - 1; i > -1; i--) path[*pathorder-1-i] = pathtmp[i];

    return true;
}

bool allpairs(struct Graph* G, int** d, int** p) {

    int i, j, k, l;

    int** W = malloc(G->order*sizeof(int*));
    for(i=0; i < G->order; i++) {
        W[i] = malloc(G->order*sizeof(int));
        for(j=0; j < G->order; j++) {
            if(i == j) {
                W[i][j] = 0;
            } else {
                if(G->ADJ_MATRIX[get_index(i,j,G->order)] == 0) W[i][j] = INF;
                else W[i][j] = G->ADJ_MATRIX[get_index(i,j,G->order)];
            }
        }
    }

    int** L = malloc(G->order*sizeof(int*));
    for(i=0; i < G->order; i++) {
        L[i] = malloc(G->order*sizeof(int));
        for(j=0; j < G->order; j++) {
            L[i][j] = W[i][j];
        }
    }

    for(l=2; l < G->order; l++) {

        int** L_next = malloc(G->order*sizeof(int*));
        for(i=0; i < G->order; i++) {
            L_next[i] = malloc(G->order*sizeof(int));
            for(j=0; j < G->order; j++) {
                L_next[i][j] = INF;
            }
        }

        for(i=0; i < G->order; i++) {
            for(j=0; j < G->order; j++) {
                p[i][j] = NONE;
                for(k=0; k < G->order; k++) {
                    if(INFSUM(L[i][k], W[k][j]) < L_next[i][j]) {
                        L_next[i][j] = L[i][k] + W[k][j];
                        p[i][j] = k;
                    }
                }
            }
        }

        for(i=0; i < G->order; i++) {
            for(j=0; j < G->order; j++) {
                L[i][j] = L_next[i][j];
            }
        }
    }

    for(i=0; i < G->order; i++) {
        for(j=0; j < G->order; j++) {
            d[i][j] = L[i][j];
        }
    }

    return true;
}

bool fasterallpairs(struct Graph* G, int** d) {

    int i, j, k, l;

    int** W = malloc(G->order*sizeof(int*));
    for(i=0; i < G->order; i++) {
        W[i] = malloc(G->order*sizeof(int));
        for(j=0; j < G->order; j++) {
            if(i == j) {
                W[i][j] = 0;
            } else {
                if(G->ADJ_MATRIX[get_index(i,j,G->order)] == 0) W[i][j] = INF;
                else W[i][j] = G->ADJ_MATRIX[get_index(i,j,G->order)];
            }
        }
    }

    int** L = malloc(G->order*sizeof(int*));
    for(i=0; i < G->order; i++) {
        L[i] = malloc(G->order*sizeof(int));
        for(j=0; j < G->order; j++) {
            L[i][j] = W[i][j];
        }
    }

    for(l=1; l < G->order - 1; l*=2) {

        int** L_next = malloc(G->order*sizeof(int*));
        for(i=0; i < G->order; i++) {
            L_next[i] = malloc(G->order*sizeof(int));
            for(j=0; j < G->order; j++) {
                L_next[i][j] = INF;
            }
        }

        for(i=0; i < G->order; i++) {
            for(j=0; j < G->order; j++) {
                for(k=0; k < G->order; k++) {
                    if(INFSUM(L[i][k], L[k][j]) < L_next[i][j]) {
                        L_next[i][j] = L[i][k] + L[k][j];
                    }
                }
            }
        }

        for(i=0; i < G->order; i++) {
            for(j=0; j < G->order; j++) {
                L[i][j] = L_next[i][j];
            }
        }
    }

    for(i=0; i < G->order; i++) {
        for(j=0; j < G->order; j++) {
            d[i][j] = L[i][j];
        }
    }

    return true;
}
