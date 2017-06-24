#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "graph.h"

void connected_components_example() {

    struct Graph* G = new_graph_from_file("graph_files/G2.graph");
    print_graph_info(G);

    printf("\n************************************************************\n");
    printf("************************************************************\n\n");

    struct Component* component = connected_components(G);

    printf("Found %d connected components\n\n", count_connected_components(component));
    print_connected_components(component);

    free_graph(G);
    free_component(component);
}

void isomorphism_example() {

    struct Graph* G1 = new_graph_from_file("graph_files/G4.graph");
    print_graph_info(G1);

    printf("\n************************************************************\n");
    printf("************************************************************\n\n");

    struct Graph* G2 = new_graph_from_file("graph_files/G5.graph");
    print_graph_info(G2);

    printf("\n************************************************************\n");
    printf("************************************************************\n\n");

    int* mapping = malloc(G1->order * sizeof(int));

    if(isomorphic(G1, G2, mapping, USE_ADJ_LISTS)) {
        printf("G1 and G2 are isomorphic\n\n");
        printf("f: G2->G1\n");
        int i;
        for(i=0; i < G1->order; i++) printf(" f(%d) = %d\n", i, mapping[i]);
    }
    else printf("G1 and G2 are not isomorphic\n");

    free_graph(G1);
    free_graph(G2);
    free(mapping);
}

void strongly_connected_components_example() {

    struct Graph* G = new_graph_from_file("graph_files/G8.graph");
    print_graph_info(G);

    printf("\n************************************************************\n");
    printf("************************************************************\n\n");

    struct Component* component = connected_components(G);

    printf("Found %d strongly connected components\n\n", count_connected_components(component));
    print_connected_components(component);

    free_graph(G);
    free_component(component);
}

void kruskal_example() {

    struct Graph* G = new_graph_from_file("graph_files/G9.graph");
    print_graph_info(G);

    printf("\n************************************************************\n");
    printf("************************************************************\n\n");

    struct Graph* T = kruskal(G);
    printf("A mimimum spanning tree computed by Kruskal's Algorithm:\n\n");
    print_graph_info(T);

    free_graph(G);
    free_graph(T);
}

void prim_example() {

    struct Graph* G = new_graph_from_file("graph_files/G9.graph");
    print_graph_info(G);

    printf("\n************************************************************\n");
    printf("************************************************************\n\n");

    struct Graph* T = prim(G);
    printf("A mimimum spanning tree computed by Prim's Algorithm:\n\n");
    print_graph_info(T);

    free_graph(G);
    free_graph(T);
}

void random_graphs_example() {

    struct Graph* G1 = new_random_graph(20, 0.1, 0, 100);
    print_graph_info(G1);
    free_graph(G1);

    printf("\n************************************************************\n");
    printf("************************************************************\n\n");

    struct Graph* G2 = new_random_graph(20, 0.5, 0, 100);
    print_graph_info(G2);
    free_graph(G2);

    printf("\n************************************************************\n");
    printf("************************************************************\n\n");

    struct Graph* G3 = new_random_graph(20, 0.9, 0, 100);
    print_graph_info(G3);
    free_graph(G3);
}

void prim_kruskal_benchmark() {

    void start_timer(unsigned long *timer) {
        struct timeval tv_start;
        gettimeofday(&tv_start, NULL);
        *timer = 1000000 * tv_start.tv_sec + tv_start.tv_usec;
    }

    void end_timer(unsigned long *timer) {
        struct timeval tv_end;
        gettimeofday(&tv_end, NULL);
        *timer = 1000000 * tv_end.tv_sec + tv_end.tv_usec - *timer;
    }

    int i;
    int v = 40;
    int n = 1000;
    float sd = 0.1;
    float dd = 0.9;

    printf("Prim and Kruskal Benchmark\n");
    printf("%d random graphs, %d vertices each\n", n, v);
    printf("Sparse graphs density: %.2f\n", sd);
    printf("Dense  graphs density: %.2f\n", dd);

    unsigned long* prim_sparse = malloc(n*sizeof(int));
    unsigned long* prim_dense  = malloc(n*sizeof(int));
    unsigned long* kruskal_sparse = malloc(n*sizeof(int));
    unsigned long* kruskal_dense  = malloc(n*sizeof(int));

    for(i=0; i < n; i++) {

        struct Graph* G1;
        struct Graph* G2;

        do { G1 = new_random_graph(v, sd, 0, 100); }
        while(!connected(G1, DEPTH_FIRST, USE_ADJ_LISTS));

        do { G2 = new_random_graph(v, dd, 0, 100); }
        while(!connected(G2, DEPTH_FIRST, USE_ADJ_LISTS));

        unsigned long *timer = malloc(sizeof(unsigned long));

        start_timer(timer);
        struct Graph* T1 = prim(G1);
        end_timer(timer);
        prim_sparse[i] = *timer;

        start_timer(timer);
        struct Graph* T2 = prim(G2);
        end_timer(timer);
        prim_dense[i] = *timer;

        start_timer(timer);
        struct Graph* T3 = kruskal(G1);
        end_timer(timer);
        kruskal_sparse[i] = *timer;

        start_timer(timer);
        struct Graph* T4 = kruskal(G2);
        end_timer(timer);
        kruskal_dense[i] = *timer;
    }

    unsigned long prim_sparse_avg = 0;
    unsigned long prim_dense_avg  = 0;
    unsigned long kruskal_sparse_avg = 0;
    unsigned long kruskal_dense_avg  = 0;

    for(i=0; i < n; i++) {
        prim_sparse_avg += prim_sparse[i];
        prim_dense_avg  += prim_dense[i];
        kruskal_sparse_avg += kruskal_sparse[i];
        kruskal_dense_avg  += kruskal_dense[i];
    }

    prim_sparse_avg /= n;
    prim_dense_avg  /= n;
    kruskal_sparse_avg /= n;
    kruskal_dense_avg  /= n;

    printf("--------------------------------------------------\n");
    printf("|  Graph/Algorithm  |     Prim     |   Kruskal   |\n");
    printf("|----------------------------------|-------------|\n");
    printf("|      Sparse       |  %6lu us   | %6lu us   |\n", prim_sparse_avg, kruskal_sparse_avg);
    printf("|      Dense        |  %6lu us   | %6lu us   |\n", prim_dense_avg, kruskal_dense_avg);
    printf("--------------------------------------------------\n");
}

void graph_show_example() {

	struct Graph* G = new_graph_from_file("graph_files/G8.graph");
    print_graph_info(G);
	graphviz_show(G);

	int* v_highlight = calloc(G->order, sizeof(int));
	v_highlight[0] = 1;
	v_highlight[1] = 1;
	v_highlight[3] = 1;
	v_highlight[4] = 1;
	graphviz_show_v(G, v_highlight);
}

void bellmanford_example() {

    struct Graph* G = new_graph_from_file("graph_files/G8.graph");
    print_graph_info(G);

    int* d = malloc(G->order * sizeof(int));
    int* p = malloc(G->order * sizeof(int));

    if(bellmanford(G, 0, d, p)) {
        printf("\nResults of Bellman-Ford algorithm\n");
        printf("\nShortest path distances:\n");
        printv(d, G->order);
        printf("\nShortest path predecesssors:\n");
        printv(p, G->order);
    } else {
        printf("\nSorry. Can't compute answer. Graph has negative-weight cycle\n");
    }

    graphviz_show(G);
}

void dijkstra_example() {

    struct Graph* G = new_graph_from_file("graph_files/G8.graph");
    print_graph_info(G);

    int* d = malloc(G->order * sizeof(int));
    int* p = malloc(G->order * sizeof(int));

    if(dijkstra(G, 0, d, p)) {
        printf("\nResults of Dijkstra algorithm\n");
        printf("\nShortest path distances:\n");
        printv(d, G->order);
        printf("\nShortest path predecesssors:\n");
        printv(p, G->order);
    } else {
        printf("\nSorry. Can't compute answer.\n");
    }

    graphviz_show(G);
}

void shortestpath_example() {

    struct Graph* G = new_graph_from_file("graph_files/G8.graph");
    print_graph_info(G);

    int source = 0;

    int* d = malloc(G->order * sizeof(int));
    int* p = malloc(G->order * sizeof(int));

    if(dijkstra(G, source, d, p)) {
        printf("\nResults of Single-Source Shortest Path algorithm\n");
        printf("\nShortest path distances:\n");
        printv(d, G->order);
        printf("\nShortest path predecesssors:\n");
        printv(p, G->order);
    } else {
        printf("\nSorry. Can't compute answer.\n");
    }

    int* path = malloc(G->order*sizeof(int));
    int* pathorder = malloc(sizeof(int));

    int terminal = 4;

    printf("\nFinding a path from %d to %d\n", source, terminal);
    if(shortestpath(G, source, terminal, path, pathorder, d, p)) {
        printf("Path found\n");
        printv(path, *pathorder);
        printf("Length: %d\n", d[terminal]);

        int i;
        int* highlight = calloc(G->order, sizeof(int));
        for(i=0; i < *pathorder; i++) highlight[path[i]] = 1;
        graphviz_show_v(G, highlight);

    } else {
        printf("No path found\n");
    }
}

void allpairs_example() {

    struct Graph* G = new_graph_from_file("graph_files/G8.graph");
    print_graph_info(G);

    int i, j;

    int** d = malloc(G->order*sizeof(int*));
    for(i=0; i < G->order; i++) {
        d[i] = calloc(G->order, sizeof(int));
    }

    int** p = malloc(G->order*sizeof(int*));
    for(i=0; i < G->order; i++) {
        p[i] = malloc(G->order*sizeof(int));
        for(j=0; j < G->order; j++) {
            p[i][j] = NONE;
        }
    }

    if(allpairs(G, d, p)) {
        printf("\nResults of All-Pairs Shortest Paths algorithm\n");
        printf("\nShortest path distances:\n");
        for(i=0; i < G->order; i++) printv(d[i], G->order);
        printf("\nShortest path predecesssors:\n");
        for(i=0; i < G->order; i++) printv(p[i], G->order);
    } else {
        printf("\nSorry. Can't compute answer.\n");
    }

    graphviz_show(G);
}

void main() {

    allpairs_example();
}
