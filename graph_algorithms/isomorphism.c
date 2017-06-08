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

bool equals_matrix(int* matrix1, int* matrix2, int rowsize, int collumnsize) {
    int i, j;
    for(i=0; i < rowsize; i++)
        for(j=0; j < collumnsize; j++)
            if(matrix1[get_index(i, j, collumnsize)] != matrix2[get_index(i, j, collumnsize)])
                return false;
    return true;
}

// Função partition com escolha de pivô normal
int partition(int *v, int start, int end) {
    // Índice do pivô
    int pivot_index = end-1;
    // Pivô
    int pivot = v[pivot_index];
    /* Variável que guarda o índice do último valor menor ou igual ao pivô;
       no fim, será a posição final do pivô*/
    int pre = start-1;

    /* Percorre o vetor, joga os valores menores que o pivô para a esquerda,
       atualiza a posição final do pivô */
    int i;
    for(i=start; i < pivot_index; i++) {
        if(v[i] <= pivot) {
            pre++;
            swap(v, i, pre);
        }
    }

    // Define a posição final do pivô, troca o pivô para essa posição
    pre++;
    swap(v, pre, pivot_index);
    return pre;
}

// Função quicksort
void quicksort(int *v, int start, int end) {

    if(start < end) {

        int pos;

        pos = partition(v, start, end);

        // Chama recursivamente quicksort p/ lados esquerdo e direito do vetor
        quicksort(v, start, pos);
        quicksort(v, pos+1, end);
    }
}

bool isomap(struct Graph* G1, struct Graph* G2, int* mapping, int rep) {

    switch (rep) {

        case USE_ADJ_LISTS: {

            int i, j;

            bool output = true;

            int* inv_mapping = malloc(G1->order * sizeof(int));
            for(i=0; i < G1->order; i++) inv_mapping[mapping[i]] = i;

            for(i=0; i < G1->order && output; i++) {

                int vertex_degree = get_vertex_degree(mapping[i], G1);
                int* tmp = malloc(vertex_degree * sizeof(int));
                for(j=0; j < vertex_degree; j++) {
                    tmp[j] = inv_mapping[G1->ADJ_LISTS[mapping[i]][j]];
                }

                quicksort(tmp, 0, vertex_degree);

                for(j=0; j < vertex_degree && output; j++) {
                    if(tmp[j] != G2->ADJ_LISTS[i][j]) output = false;
                }

                free(tmp);
            }

            free(inv_mapping);

            return output;
        }

        case USE_ADJ_MATRIX: {

            int* pmatrix = getpmatrix(mapping, G1->order, false);
            int* ptmatrix = getpmatrix(mapping, G1->order, true);

            int* tmp = multmatrix(pmatrix, G1->ADJ_MATRIX, G1->order);
            tmp = multmatrix(tmp, ptmatrix, G1->order);

            bool output = equals_matrix(tmp, G2->ADJ_MATRIX, G2->order, G2->order);

            free(pmatrix);
            free(ptmatrix);
            free(tmp);

            return output;
        }

        case USE_INC_MATRIX: {

            int i, j;
            for(i=0; i < G2->size; i++) {
                int v1 = G2->E[i].v1;
                int v2 = G2->E[i].v2;
                bool match = false;
                for(j=0; j < G1->size && !match; j++) {
                    if(G1->INC_MATRIX[get_index(mapping[v1], j, G1->size)]
                        && G1->INC_MATRIX[get_index(mapping[v2], j, G1->size)]) {
                            match = true;
                        }
                }
                if(!match) return false;
            }
            return true;
        }

        default: {
            break;
        }
    }
}

bool isomorphic(struct Graph* G1, struct Graph* G2, int* mapping, int rep) {

    if(G1->order != G2->order) return false;
    if(G1->size != G2->size) return false;

    bool isomorphic = false;

    int i;
    int* permutation = malloc(G1->order * sizeof(int));
    for(i=0; i < G1->order; i++) permutation[i] = i;

    do {

        if(isomap(G1, G2, permutation, rep)) {
            memcpy(mapping, permutation, G1->order * sizeof(int));
            //printv(mapping, G1->order);
            isomorphic = true;
            return isomorphic;
        }

    } while(next_permutation(permutation, G1->order));

    free(permutation);

    return isomorphic;
}
