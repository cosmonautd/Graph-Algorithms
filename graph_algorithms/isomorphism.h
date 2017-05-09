#ifndef ISOMORPHISM_H_
#define ISOMORPHISM_H_

#include "../graph.h"

#ifndef USE_ADJ_LISTS
#define USE_ADJ_LISTS   2
#define USE_ADJ_MATRIX  3
#define USE_INC_MATRIX  4
#endif

void printv(int *v, int len);
void swap(int* v, int x, int y);
bool next_permutation(int* v, int len);
int* getpmatrix(int* mapping, int len, bool transpose);
int* multmatrix(int* M1, int* M2, int len);
bool equals_matrix(int* matrix1, int* matrix2, int rowsize, int collumnsize);
bool isomap(struct Graph* G1, struct Graph* G2, int* mapping, int rep);
bool isomorphic(struct Graph* G1, struct Graph* G2, int* mapping, int rep);

#endif
