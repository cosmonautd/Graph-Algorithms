#include "isomorphism.h"

void printv(int *v, int len) {
    int i;
    for(i=0; i < len; i++) printf("%d ", v[i]);
    printf("\n");
}

void swap(int* v, int x, int y) {
    int tmp = v[y];
    v[y] = v[x];
    v[x] = tmp;
}

bool next_permutation(int* v, int len) {
    int i;
    for(i=len-2; i > -1 && !(v[i] < v[i+1]); i--) {}
    if(i==-1) return false;

    int j;
    for(j=len-1; j > i && !(v[i] < v[j]); j--) {}

    swap(v, i, j);

    int k;
    for(k=0; i+1+k < len-1-k; k++) swap(v, i+1+k, len-1-k);
}

int* getpmatrix(int* mapping, int len, bool transpose) {
    int i;
    int* pmatrix = calloc(len*len, sizeof(int));

    if(!transpose)
        for(i=0; i < len; i++)
            pmatrix[get_index(i, mapping[i], len)] = 1;
    else
        for(i=0; i < len; i++)
            pmatrix[get_index(mapping[i], i, len)] = 1;

    return pmatrix;
}

int* multmatrix(int* M1, int* M2, int len) {
    int i, j, k;
    int* tmp = calloc(len*len, sizeof(int));
    for(i=0; i < len; i++) {
        for(j=0; j < len; j++) {
            for(k=0; k < len; k++) {
                tmp[get_index(i,j,len)] += M1[get_index(i,k,len)] * M2[get_index(k,j,len)];
            }
        }
    }
    return tmp;
}

bool equals_adjmatrix(int* adjmatrix1, int* adjmatrix2, int len) {
    int i, j;
    for(i=0; i < len; i++)
        for(j=0; j < len; j++)
            if(adjmatrix1[get_index(i, j, len)] != adjmatrix2[get_index(i, j, len)])
                return false;
    return true;
}

bool isomap(struct Graph* G1, struct Graph* G2, int* mapping) {

    int* pmatrix = getpmatrix(mapping, G1->order, false);
    int* ptmatrix = getpmatrix(mapping, G1->order, true);

    int* tmp = multmatrix(pmatrix, G1->ADJ_MATRIX, G1->order);
    tmp = multmatrix(tmp, ptmatrix, G1->order);

    bool output = equals_adjmatrix(tmp, G2->ADJ_MATRIX, G2->order);

    free(pmatrix);
    free(ptmatrix);
    free(tmp);

    return output;
}

bool isomorphic(struct Graph* G1, struct Graph* G2, int* mapping, int rep) {

    if(G1->order != G2->order) return false;
    if(G1->size != G2->size) return false;

    bool isomorphic = false;

    switch (rep) {

        case USE_ADJ_LISTS: {
            break;
        }

        case USE_ADJ_MATRIX: {

            int i;
            int* permutation = malloc(G1->order * sizeof(int));
            for(i=0; i < G1->order; i++) permutation[i] = i;

            do {

                if(isomap(G1, G2, permutation)) {
                    memcpy(mapping, permutation, G1->order * sizeof(int));
                    isomorphic = true;
                    return isomorphic;
                }

            } while(next_permutation(permutation, G1->order));

            free(permutation);

            break;
        }

        case USE_INC_MATRIX: {
            break;
        }

        default: {
            break;
        }
    }

    return isomorphic;
}
