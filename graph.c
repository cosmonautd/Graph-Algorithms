#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG

#ifdef DEBUG
    #define DEBUG_MESSAGE(x) printf x
#else
    #define DEBUG_MESSAGE(x) ;
#endif

/*  Estrutura Grafo
    V: vértices
    ADJ_MATRIX: matriz de adjacência
    order: ordem do grafo
*/
struct Graph {
    int* V;
    int* ADJ_MATRIX;
    int** ADJ_LISTS;
    int order;
    int directed;
};

/*  Função get_index()
    Dados os índices i, j e o comprimento das linhas de uma matriz
    bidimensional, retorna o índice equivalente em uma representação da matriz
    como um vetor unidimensional.
*/
int get_index(int i, int j, int rowsize) {
    return i + j*rowsize;
}

int get_vertex_degree(int vertex, int* adj_matrix, int order) {
    int j;
    int vertex_degree = 0;
    for(j=0; j < order; j++)
        if(adj_matrix[get_index(vertex, j, order)] > 0 && j != vertex)
            vertex_degree++;

    return vertex_degree;
}

int get_graph_order(struct Graph* G) {
    return G->order;
}

int is_directed(struct Graph* G) {
    return G->directed;
}

/*  Função graph_from_file()
    Realiza a leitura do arquivo de caminho path e preenche a estrutura Graph,
    cujo endereço é dado pelo ponteiro G.
*/
int graph_from_file(struct Graph* G, const char* path) {

    int i, j, k;

    DEBUG_MESSAGE(("Opening file at: %s\n", path));
    FILE* file;
    file = fopen(path, "r");

    if(file != NULL)
        DEBUG_MESSAGE(("File opened successfully\n"));
    else {
        DEBUG_MESSAGE(("Error opening file\n"));
        exit(EXIT_FAILURE);
    }

    DEBUG_MESSAGE(("Computing file size...\n"));
    fseek(file, SEEK_SET, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    DEBUG_MESSAGE(("File size is %ld bytes\n", file_size));

    DEBUG_MESSAGE(("Allocating %ld bytes in memory...\n", file_size));
    char* file_buffer = malloc(file_size*sizeof(char));

    DEBUG_MESSAGE(("Reading file to memory...\n"));
    fread(file_buffer, 1, file_size, file);

    DEBUG_MESSAGE(("File content:\n%s\n", file_buffer));

    DEBUG_MESSAGE(("Reading vertices from file to memory...\n"));
    char* end_of_row = strchr(file_buffer, '\n');
    int vertices_list_size = end_of_row - file_buffer + 1;

    DEBUG_MESSAGE(("Vertices list has %d characters\n", vertices_list_size));

    char* vertices_list = malloc((vertices_list_size)*sizeof(char));
    strncpy(vertices_list, file_buffer, vertices_list_size);

    DEBUG_MESSAGE(("%s\n", vertices_list));

    DEBUG_MESSAGE(("Reading Adjacency Matrix from file to memory...\n"));
    int adj_matrix_size = strlen(file_buffer + vertices_list_size);

    DEBUG_MESSAGE(("Adjacency Matrix has %d characters\n", adj_matrix_size));

    char* adj_matrix = malloc((adj_matrix_size)*sizeof(char));
    strncpy(adj_matrix, file_buffer + vertices_list_size, adj_matrix_size);

    free(file_buffer);
    fclose(file);

    DEBUG_MESSAGE(("%s\n", adj_matrix));

    char* vertices_list_copy = malloc((vertices_list_size)*sizeof(char));
    strcpy(vertices_list_copy, vertices_list);

    DEBUG_MESSAGE(("Counting Vertices\n\n"));
    int n_vertices = 0;
    char* tokenize = strtok(vertices_list_copy, ", ");
    for(;tokenize != NULL;) {
        DEBUG_MESSAGE(("%s\n", tokenize));
        n_vertices++;
        tokenize = strtok(NULL, ", ");
    }

    DEBUG_MESSAGE(("Vertices found: %d\n", n_vertices));

    DEBUG_MESSAGE(("Allocating memory for %d vertices "
                        "and a %dx%d Adjacency Matrix\n\n",
                            n_vertices, n_vertices, n_vertices));

    G->V = malloc(n_vertices * sizeof(int));
    G->ADJ_MATRIX = malloc(n_vertices * n_vertices * sizeof(int));
    G->ADJ_LISTS = malloc(n_vertices * sizeof(int*));
    G->order = n_vertices;

    DEBUG_MESSAGE(("Generating Vertices...\n"));
    tokenize = strtok(vertices_list, ", ");
    for(i=0; tokenize != NULL; i++) {
        G->V[i] = atoi(tokenize);
        tokenize = strtok(NULL, ", ");
    }

    free(vertices_list);

    DEBUG_MESSAGE(("Generating Adjacency Matrix...\n"));
    tokenize = strtok(adj_matrix, ", \n");
    for(i=0; tokenize != NULL; i++) {
        G->ADJ_MATRIX[i] = atoi(tokenize);
        tokenize = strtok(NULL, ", \n");
    }

    free(adj_matrix);

    DEBUG_MESSAGE(("Generating Adjacency Lists...\n\n"));
    for(i=0; i < G->order; i++) {
        int vertex_degree = get_vertex_degree(i, G->ADJ_MATRIX, G->order);
        G->ADJ_LISTS[i] = malloc(vertex_degree * sizeof(int));
        for(j=0, k=0; j < G->order && k < vertex_degree; j++)
            if(G->ADJ_MATRIX[get_index(i, j, G->order)] > 0 && i != j) {
                G->ADJ_LISTS[i][k] = j;
                k++;
            }
    }

    DEBUG_MESSAGE(("Generated vertices:\n\n"));
    for(i=0; i < G->order; i++) DEBUG_MESSAGE(("%d ", G->V[i]));
    DEBUG_MESSAGE(("\n\n"));

    DEBUG_MESSAGE(("Generated Adjacency Matrix:\n\n"));
    for(i=0; i < G->order; i++) {
        for(j=0; j < G->order; j++) DEBUG_MESSAGE(("%d ", G->ADJ_MATRIX[get_index(i,j,G->order)]));
        DEBUG_MESSAGE(("\n"));
    }
    DEBUG_MESSAGE(("\n"));

    DEBUG_MESSAGE(("Generated Adjacency Lists:\n\n"));
    for(i=0; i < G->order; i++) {
        DEBUG_MESSAGE(("Vertex %d: ", i));
        int vertex_degree = get_vertex_degree(i, G->ADJ_MATRIX, G->order);
        for(j=0; j < vertex_degree; j++) DEBUG_MESSAGE(("%d ", G->ADJ_LISTS[i][j]));
        DEBUG_MESSAGE(("\n"));
    }
    DEBUG_MESSAGE(("\n"));

    DEBUG_MESSAGE(("Checking directness...\n"));
    int symmetric = 1;
    for(i=0; i < G->order && symmetric; i++) {
        for(j==i+1; j < G->order && symmetric; j++)
            if(G->ADJ_MATRIX[get_index(i,j,G->order)] != G->ADJ_MATRIX[get_index(j,i,G->order)]) {
                G->directed = 1;
                symmetric = 0;
            }
    }
}

void main() {

    int i;

    struct Graph* G = malloc(sizeof(struct Graph));
    graph_from_file(G, "G.graph");

    if(is_directed(G)) printf("\nDirected Graph\n");
    else printf("\nUndirected Graph\n");

    printf("Graph order: %d\n", get_graph_order(G));

    free(G->V);
    free(G->ADJ_MATRIX);
    for(i=0; i < G->order; i++) free(G->ADJ_LISTS[i]);
    free(G->ADJ_LISTS);
    free(G);
}
