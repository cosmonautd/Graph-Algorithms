#include "graph.h"

struct Graph* new_graph(int* V, int* ADJ_MATRIX, int order) {

    int i, j, k;
    struct Graph* G = malloc(sizeof(struct Graph));

    G->order = order;
    G->V = malloc(G->order * sizeof(int));
    G->ADJ_MATRIX = malloc(G->order * G->order * sizeof(int));
    G->ADJ_LISTS = malloc(G->order * sizeof(int*));
    G->size = 0;
    G->oriented = false;

    DEBUG_MESSAGE(("Copying vertices...\n"));
    for(i=0; i < G->order; i++) G->V[i] = V[i];

    DEBUG_MESSAGE(("Copying Adjacency Matrix...\n"));
    for(i=0; i < G->order * G->order; i++) G->ADJ_MATRIX[i] = ADJ_MATRIX[i];

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

    DEBUG_MESSAGE(("Checking if oriented...\n"));
    for(i=0; i < G->order && !G->oriented; i++) {
        for(j=i+1; j < G->order && !G->oriented; j++){
            if(G->ADJ_MATRIX[get_index(i,j,G->order)] != G->ADJ_MATRIX[get_index(j,i,G->order)])
                G->oriented = true;
        }
    }

    DEBUG_MESSAGE(("Counting edges...\n"));
    for(i=0; i < G->order; i++)
        for(j=i+1; j < G->order; j++)
            if(G->ADJ_MATRIX[get_index(i,j,G->order)] != 0)
                G->size++;
    if(G->oriented)
        for(i=0; i < G->order; i++)
            for(j=0; j < i; j++)
                if(G->ADJ_MATRIX[get_index(i,j,G->order)] != 0)
                    G->size++;

    DEBUG_MESSAGE(("Generating edge list...\n"));
    G->E = malloc(G->size*sizeof(struct Edge));
    if(!G->oriented) {
        for(i=0, k=0; i < G->order; i++)
            for(j=i+1; j < G->order; j++)
                if(G->ADJ_MATRIX[get_index(i,j,G->order)] != 0) {
                    G->E[k].v1 = i;
                    G->E[k].v2 = j;
                    G->E[k++].weight = G->ADJ_MATRIX[get_index(i,j,G->order)];
                }
    } else {
        for(i=0, k=0; i < G->order; i++)
            for(j=0; j < G->order; j++)
                if(G->ADJ_MATRIX[get_index(i,j,G->order)] != 0 && i != j) {
                    G->E[k].v1 = i;
                    G->E[k].v2 = j;
                    G->E[k++].weight = G->ADJ_MATRIX[get_index(i,j,G->order)];
                }
    }

    DEBUG_MESSAGE(("Generating Incidence Matrix...\n"));
    G->INC_MATRIX = calloc(G->order * G->size, sizeof(int));
    for(i=0; i < G->size; i++) {
        G->INC_MATRIX[get_index(G->E[i].v1, i, G->size)] = 1;
        G->INC_MATRIX[get_index(G->E[i].v2, i, G->size)] = 1;
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

    DEBUG_MESSAGE(("Generated Incidence Matrix:\n\n"));
    for(i=0; i < G->order; i++) {
        for(j=0; j < G->size; j++) DEBUG_MESSAGE(("%d ", G->INC_MATRIX[get_index(i,j,G->size)]));
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

struct Graph* new_random_graph(int n, float p, int min_weight, int max_weight) {

    int i, j;
    int* vertices = malloc(n * sizeof(int));
    int* adj_matrix = calloc(n * n, sizeof(int));

    srand(time(NULL));

    for(i=0; i < n; i++) {
        vertices[i] = i;
        for(j=i+1; j < n; j++) {
            int random = rand() % 100;
            if(random < p*100) {
                int random_weight = min_weight + rand() % (max_weight - min_weight + 1);
                adj_matrix[get_index(i, j, n)] = random_weight;
                adj_matrix[get_index(j, i, n)] = random_weight;
            }
        }
    }

    return new_graph(vertices, adj_matrix, n);
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

    if(file != NULL) {
        DEBUG_MESSAGE(("File opened successfully\n"));
    }
    else {
        printf("Error opening file: %s\n", path);
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

    free(vertices_list_copy);

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

// Função para impressão de um vetor v, de comprimento len
void printv(int *v, int len) {
    int i;
    for(i=0; i < len; i++) printf("%d ", v[i]);
    printf("\n");
}

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

void print_vertices(struct Graph* G) {
    int i;
    for(i=0; i < G->order; i++) printf("%d ", G->V[i]);
    printf("\n");
}

void print_adj_matrix(struct Graph* G) {
    int i, j;
    for(i=0; i < G->order; i++) {
        for(j=0; j < G->order; j++) printf("%d ", G->ADJ_MATRIX[get_index(i,j,G->order)]);
        printf("\n");
    }
}

void print_inc_matrix(struct Graph* G) {
    int i, j;
    for(i=0; i < G->order; i++) {
        for(j=0; j < G->size; j++) printf("%d ", G->INC_MATRIX[get_index(i,j,G->size)]);
        printf("\n");
    }
}

void print_adj_lists(struct Graph* G) {
    int i, j;
    for(i=0; i < G->order; i++) {
        printf("Vertex %d: ", i);
        int vertex_degree = get_vertex_degree(i, G);
        for(j=0; j < vertex_degree; j++) printf("%d ", G->ADJ_LISTS[i][j]);
        printf("\n");
    }
}

void print_graph_info(struct Graph* G) {

    if(G->oriented) printf("Oriented Graph\n");
    else printf("Not Oriented Graph\n");

    printf("\nVertices:\n\n");
    print_vertices(G);

    printf("\nAdjacency Matrix:\n\n");
    print_adj_matrix(G);

    printf("\nIncidence Matrix:\n\n");
    print_inc_matrix(G);

    printf("\nAdjacencies List:\n\n");
    print_adj_lists(G);

    printf("\n");

    if(connected(G, DEPTH_FIRST, USE_ADJ_MATRIX))
    //if(connected(G, BREADTH_FIRST, USE_ADJ_MATRIX))
        printf("Connected\n");
    else printf("Not Connected\n");

    if(tree(G)) printf("Tree\n");

    printf("Graph order: %d\n", get_graph_order(G));
    printf("Graph size: %d\n", get_graph_size(G));
}

/*  Função generate_dotfile()
    Gera um arquivo de representação de grafo que pode ser desenhado com graphviz
*/
void generate_dotfile(struct Graph* G, const char* filename, int* v_highlight) {

    FILE* dotfile;
	dotfile = fopen(filename, "w");

	if(dotfile != NULL) {
	    DEBUG_MESSAGE(("File opened successfully\n"));
	}
	else {
	    printf("Error opening file: %s\n", filename);
	    exit(EXIT_FAILURE);
	}

	if(oriented(G)) fputs("digraph {\n", dotfile);
	else fputs("graph {\n", dotfile);

	fputs("node[shape=circle]\n", dotfile);

	int i;
	for(i=0; i < G->order; i++) {
		char v[100];
		snprintf(v, 100, "%d", G->V[i]);
		fputs(v, dotfile);
		if(v_highlight[i]) fputs(" [style = filled, fillcolor = \"#82A5C5\"]", dotfile);
		fputs(";\n", dotfile);
	}

	for(i=0; i < G->size; i++) {
		char v1[100];
		char v2[100];
		snprintf(v1, 100, "%d", G->E[i].v1);
		snprintf(v2, 100, "%d", G->E[i].v2);
		fputs(v1, dotfile);
		if(oriented(G)) fputs("->", dotfile);
		else fputs("--", dotfile);
		fputs(v2, dotfile);
		fputs(";\n", dotfile);
	}

	fputs("}", dotfile);
	fclose(dotfile);
}

void graphviz_show(struct Graph* G) {
	int* v_highlight = calloc(G->order, sizeof(int));
	generate_dotfile(G, "G.gv", v_highlight);
	system("dot G.gv -Tpng -o G.png");
	system("feh G.png");
	system("rm G.gv G.png");
}

void graphviz_show_v(struct Graph* G, int* v_highlight) {
	generate_dotfile(G, "G.gv", v_highlight);
	system("dot G.gv -Tpng -o G.png");
	system("feh G.png");
	system("rm G.gv G.png");
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

/*  Função oriented()
    Retorna 1 se o grafo é orientado, 0 se é não-orientado.
*/
bool oriented(struct Graph* G) {
    return G->oriented;
}

bool complete(struct Graph* G) {
    if (!G->oriented) return (2*G->size == G->order*(G->order - 1));
    else return (G->size == G->order*(G->order - 1));
}

/*  Função tree()
    Verifica se o grafo é uma árvore.
*/
bool tree(struct Graph* G) {
    return (connected(G, DEPTH_FIRST, USE_ADJ_LISTS) && G->size == G->order - 1);
}

void add_edge(struct Graph* G, int v1, int v2, int weight) {

    assert(weight != 0);
    assert(v1 < G->order && v2 < G->order);

    if(!G->oriented) {

        assert(G->ADJ_MATRIX[get_index(v1,v2,G->order)] == 0);
        assert(G->ADJ_MATRIX[get_index(v2,v1,G->order)] == 0);

        int d1 = get_vertex_degree(v1, G);
        int d2 = get_vertex_degree(v2, G);

        G->ADJ_MATRIX[get_index(v1,v2,G->order)] = weight;
        G->ADJ_MATRIX[get_index(v2,v1,G->order)] = weight;

        G->ADJ_LISTS[v1] = realloc(G->ADJ_LISTS[v1], d1 + 1);
        G->ADJ_LISTS[v2] = realloc(G->ADJ_LISTS[v2], d2 + 1);

        G->ADJ_LISTS[v1][d1] = v2;
        G->ADJ_LISTS[v2][d2] = v1;

        G->size++;

        struct Edge* tmp_edgelist = malloc(G->size * sizeof(struct Edge));
        memcpy(tmp_edgelist, G->E, (G->size - 1) * sizeof(struct Edge));
        tmp_edgelist[G->size - 1].v1 = v1;
        tmp_edgelist[G->size - 1].v2 = v2;
        tmp_edgelist[G->size - 1].weight = weight;

        free(G->E);
        G->E = tmp_edgelist;

        int i;
        int* tmp = calloc(G->order * G->size, sizeof(int));
        for(i=0; i < G->size; i++) {
            tmp[get_index(G->E[i].v1, i, G->size)] = 1;
            tmp[get_index(G->E[i].v2, i, G->size)] = 1;
        }

        free(G->INC_MATRIX);
        G->INC_MATRIX = tmp;

    } else {

        assert(G->ADJ_MATRIX[get_index(v1,v2,G->order)] == 0);

        int d = get_vertex_degree(v1, G);

        G->ADJ_MATRIX[get_index(v1,v2,G->order)] = weight;
        G->ADJ_LISTS[v1] = realloc(G->ADJ_LISTS[v1], d + 1);
        G->ADJ_LISTS[v1][d] = v2;
    }
}

void free_graph(struct Graph* G) {

    int i;

    free(G->V);
    free(G->ADJ_MATRIX);
    free(G->INC_MATRIX);
    for(i=0; i < G->order; i++) free(G->ADJ_LISTS[i]);
    free(G->ADJ_LISTS);
    free(G->E);
    free(G);
}
