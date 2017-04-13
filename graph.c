#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*  Define se o programa é compilado com impressão de linhas de debug
*/
#define DEBUG

#ifdef DEBUG
    #define DEBUG_MESSAGE(x) printf x
#else
    #define DEBUG_MESSAGE(x) ;
#endif

/*  Macros para escolha de algoritmos de busca em profundidade ou largura
*/
#define DEPTH_FIRST 0
#define BREADTH_FIRST 1

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
    int** ADJ_LISTS;
    int order;
    int size;
    int directed;
};

/*  Função get_index()
    Dados os índices i, j e o comprimento das linhas de uma matriz
    bidimensional, retorna o índice equivalente em uma representação da matriz
    como um vetor unidimensional.
*/
int get_index(int i, int j, int rowsize) {
    return j + i*rowsize;
}

/*  Função get_vertex_degree()
    Retorna o grau de um vértice v no grafo G. O cálculo é feito através da
    contagem de elementos positivos na linha da matriz de adjacência que
    representa o vértice v.
*/
int get_vertex_degree(int vertex, struct Graph* G) {
    int j;
    int vertex_degree = 0;
    for(j=0; j < G->order; j++)
        if(G->ADJ_MATRIX[get_index(vertex, j, G->order)] != 0 && j != vertex)
            vertex_degree++;

    return vertex_degree;
}

/*  Função get_graph_order()
    Retorna a ordem do grafo G.
*/
int get_graph_order(struct Graph* G) {
    return G->order;
}

/*  Função get_graph_size()
    Retorna o tamanho do grafo G.
*/
int get_graph_size(struct Graph* G) {
    return G->size;
}

/*  Função is_directed()
    Retorna 1 se o grafo é orientado, 0 se é não-orientado.
*/
int is_directed(struct Graph* G) {
    return G->directed;
}

/*  Função dfs()
    Realiza uma busca em profundidade no grafo G, a partir do vértice v.
    A entrada aux deve ser um vetor de comprimento igual à ordem de G.
    A váriavel depth indica a profundidade atual durante a busca.
    Ao fim da busca, os vértices alcançáveis a partir de v são indicados no
    vetor aux, de tal forma que o valor no índice w em aux é 1 se o vértice w
    é alcançável, e 0 caso contrário.
*/
void dfs(struct Graph* G, int v, int* aux, int depth) {

    if(depth == 0) {
        DEBUG_MESSAGE(("\nExecuting Depth First Traversal from vertex %d\n", v));
        dfs(G, v, aux, ++depth);

    } else {
        aux[v] = 1;
        DEBUG_MESSAGE(("Visiting vertex %d\n", v));
        int j;
        for(j=0; j < get_vertex_degree(v, G); j++) {
            int w = G->ADJ_LISTS[v][j];
            if(aux[w] == 0)
                dfs(G, w, aux, depth++);
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
void bfs(struct Graph* G, int v, int* aux) {

    int* Q = calloc(G->order, sizeof(int));
    int Q_start = 0;
    int Q_end = 0;

    DEBUG_MESSAGE(("\nExecuting Breadth First Traversal from vertex %d\n", v));

    aux[v] = 1;
    DEBUG_MESSAGE(("Visiting vertex %d\n", v));
    Q[Q_end++] = v;

    while(Q_start < Q_end) {
        v = Q[Q_start++];
        int j;
        for(j=0; j < get_vertex_degree(v, G); j++) {
            int w = G->ADJ_LISTS[v][j];
            if(aux[w] == 0) {
                Q[Q_end++] = w;
                aux[w] = 1;
                DEBUG_MESSAGE(("Visiting vertex %d\n", w));
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
int is_connected(struct Graph* G, int algorithm) {

    int i, j;

    if(algorithm == DEPTH_FIRST) {

        if(!is_directed(G)) {
            int* aux = calloc(G->order, sizeof(int));
            dfs(G, 0, aux, 0);
            for(i=0; i < G->order; i++) if(aux[i] == 0) return 0;
            return 1;
        } else {
            for(i=0; i < G->order; i++) {
                int* aux = calloc(G->order, sizeof(int));
                dfs(G, i, aux, 0);
                for(j=0; j < G->order; j++) if(aux[j] == 0) return 0;
            }
            return 1;
        }

    } else if(algorithm = BREADTH_FIRST) {

        if(!is_directed(G)) {
            int* aux = calloc(G->order, sizeof(int));
            bfs(G, 0, aux);
            for(i=0; i < G->order; i++) if(aux[i] == 0) return 0;
            return 1;
        } else {
            for(i=0; i < G->order; i++) {
                int* aux = calloc(G->order, sizeof(int));
                bfs(G, i, aux);
                for(j=0; j < G->order; j++) if(aux[j] == 0) return 0;
            }
            return 1;
        }
    }
}

/*  Função is_tree()
    Verifica se o grafo é uma árvore.
*/
int is_tree(struct Graph* G) {
    return (is_connected(G, DEPTH_FIRST) && G->size == G->order - 1);
}


struct Graph* new_graph(int* V, int* ADJ_MATRIX, int order) {

    int i, j, k;
    struct Graph* G = malloc(sizeof(struct Graph));

    G->order = order;
    G->V = malloc(G->order * sizeof(int));
    G->ADJ_MATRIX = malloc(G->order * G->order * sizeof(int));
    G->ADJ_LISTS = malloc(G->order * sizeof(int*));
    G->size = 0;
    G->directed = 0;

    DEBUG_MESSAGE(("Copying vertices...\n"));
    for(i=0; i < G->order; i++) G->V[i] = V[i];

    DEBUG_MESSAGE(("Copying adjacency matrix...\n"));
    for(i=0; i < G->order * G->order; i++) G->ADJ_MATRIX[i] = ADJ_MATRIX[i];

    DEBUG_MESSAGE(("Counting edges...\n"));
    for(i=0; i < G->order; i++)
        for(j=i+1; j < G->order; j++)
            if(G->ADJ_MATRIX[get_index(i,j,G->order)] != 0)
                G->size++;

    DEBUG_MESSAGE(("Generating Adjacency Lists...\n\n"));
    for(i=0; i < G->order; i++) {
        int vertex_degree = get_vertex_degree(i, G);
        G->ADJ_LISTS[i] = malloc(vertex_degree * sizeof(int));
        for(j=0, k=0; j < G->order && k < vertex_degree; j++)
            if(G->ADJ_MATRIX[get_index(i, j, G->order)] != 0 && i != j) {
                G->ADJ_LISTS[i][k] = j;
                k++;
            }
    }

    DEBUG_MESSAGE(("Checking directness...\n"));
    for(i=0; i < G->order && !G->directed; i++) {
        for(j=i+1; j < G->order && !G->directed; j++){
            if(G->ADJ_MATRIX[get_index(i,j,G->order)] != G->ADJ_MATRIX[get_index(j,i,G->order)])
                G->directed = 1;
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
        int vertex_degree = get_vertex_degree(i, G);
        for(j=0; j < vertex_degree; j++) DEBUG_MESSAGE(("%d ", G->ADJ_LISTS[i][j]));
        DEBUG_MESSAGE(("\n"));
    }
    DEBUG_MESSAGE(("\n"));

    return G;
}

/*  Função graph_from_file()
    Realiza a leitura do arquivo de caminho path e preenche a estrutura Graph,
    cujo endereço é dado pelo ponteiro G.
*/
struct Graph* new_graph_from_file(const char* path) {

    int i;

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

    int* V = malloc(n_vertices * sizeof(int));
    int* ADJ_MATRIX = malloc(n_vertices * n_vertices * sizeof(int));

    DEBUG_MESSAGE(("Generating Vertices...\n"));
    tokenize = strtok(vertices_list, ", ");
    for(i=0; tokenize != NULL; i++) {
        V[i] = atoi(tokenize);
        tokenize = strtok(NULL, ", ");
    }

    free(vertices_list);

    DEBUG_MESSAGE(("Generating Adjacency Matrix...\n"));
    tokenize = strtok(adj_matrix, ", \n");
    for(i=0; tokenize != NULL; i++) {
        ADJ_MATRIX[i] = atoi(tokenize);
        tokenize = strtok(NULL, ", \n");
    }

    free(adj_matrix);

    return new_graph(V, ADJ_MATRIX, n_vertices);
}

void main() {

    int i;

    struct Graph* G = new_graph_from_file("G.graph");

    if(is_directed(G)) printf("\nDirected Graph\n");
    else printf("\nUndirected Graph\n");

    //if(is_connected(G, DEPTH_FIRST))
    if(is_connected(G, BREADTH_FIRST))
        printf("\nConnected\n");
    else printf("\nNot Connected\n");

    if(is_tree(G)) printf("\nTree\n");

    printf("Graph order: %d\n", get_graph_order(G));
    printf("Graph size: %d\n", get_graph_size(G));

    printf("\n\n");
    struct Graph* G2 = new_graph(G->V, G->ADJ_MATRIX, G->order);

    if(is_connected(G2, BREADTH_FIRST))
        printf("\nConnected\n");
    else printf("\nNot Connected\n");

    free(G->V);
    free(G->ADJ_MATRIX);
    for(i=0; i < G->order; i++) free(G->ADJ_LISTS[i]);
    free(G->ADJ_LISTS);
    free(G);
}
