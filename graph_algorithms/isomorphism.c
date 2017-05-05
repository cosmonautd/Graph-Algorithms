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

void relabel(int* base_matrix, int* copy_matrix, int len, int* mapping) {

    int i, j;
    for(i=0; i < len; i++) {
        for(j=0; j < len; j++) {
            copy_matrix[get_index(i, j, len)] = base_matrix[get_index(mapping[i], j, len)];
        }
    }
}

void fixref(int* base_matrix, int* copy_matrix, int len, int* mapping) {

    int i, j;
    int* tmp_matrix = calloc(len*len, sizeof(int));

    for(i=0; i < len; i++) {
        for(j=0; j < len; j++) {
            if(copy_matrix[get_index(i, j, len)] > 0) {
                int aux = mapping[j];
                tmp_matrix[get_index(i, aux, len)] = copy_matrix[get_index(i, j, len)];
            }
        }
    }
    memcpy(copy_matrix, tmp_matrix, len*len*sizeof(int));
}

bool equals_adj_matrix(int* adj_matrix1, int* adj_matrix2, int len) {
    int i, j;
    for(i=0; i < len; i++)
        for(j=0; j < len; j++)
            if(adj_matrix1[get_index(i, j, len)] != adj_matrix2[get_index(i, j, len)])
                return false;
    return true;
}

bool isomorphic(struct Graph* G1, struct Graph* G2, int* mapping, int rep) {

    if(G1->order != G2->order) return false;
    if(G1->size != G2->size) return false;

    bool isomorphic = false;

    switch (rep) {

        case USE_ADJ_LISTS: {
            break;
        }

        case USE_ADJ_MATRIX: { // TODO: Works os tests, but need more testing

            int i, j;
            int* permutation = malloc(G1->order * sizeof(int));
            for(i=0; i < G1->order; i++) permutation[i] = i;

            do {

                int* adj_matrix_copy = calloc(G1->order*G1->order, sizeof(int));

                relabel(G1->ADJ_MATRIX, adj_matrix_copy, G1->order, permutation);
                fixref(G1->ADJ_MATRIX, adj_matrix_copy, G1->order, permutation);

                if(equals_adj_matrix(adj_matrix_copy, G2->ADJ_MATRIX, G2->order)) {
                    memcpy(mapping, permutation, G1->order * sizeof(int));
                    isomorphic = true;
                    return isomorphic;
                }

                free(adj_matrix_copy);

            } while(next_permutation(permutation, G1->order));

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
