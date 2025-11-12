//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 100
#define MAX_EDGES 1000

struct Edge {
    int src;
    int dest;
    int weight;
};

struct Graph {
    int num_nodes;
    int num_edges;
    struct Edge edges[MAX_EDGES];
};

void init_graph(struct Graph *g, int nodes, int edges) {
    if (g == NULL || nodes <= 0 || nodes > MAX_NODES || edges < 0 || edges > MAX_EDGES) {
        fprintf(stderr, "Invalid graph parameters\n");
        exit(1);
    }
    g->num_nodes = nodes;
    g->num_edges = edges;
    memset(g->edges, 0, sizeof(g->edges));
}

int add_edge(struct Graph *g, int src, int dest, int weight) {
    if (g == NULL || src < 0 || src >= g->num_nodes || dest < 0 || dest >= g->num_nodes) {
        return 0;
    }
    if (g->num_edges >= MAX_EDGES) {
        return 0;
    }
    struct Edge *e = g->edges + g->num_edges;
    e->src = src;
    e->dest = dest;
    e->weight = weight;
    g->num_edges++;
    return 1;
}

void print_graph(struct Graph *g) {
    if (g == NULL) return;
    printf("Graph with %d nodes and %d edges:\n", g->num_nodes, g->num_edges);
    for (int i = 0; i < g->num_edges; i++) {
        struct Edge *e = g->edges + i;
        printf("  %d -> %d (weight: %d)\n", e->src, e->dest, e->weight);
    }
}

int find(int parent[], int i) {
    if (parent[i] == -1) return i;
    return find(parent, parent[i]);
}

void union_sets(int parent[], int x, int y) {
    int xset = find(parent, x);
    int yset = find(parent, y);
    if (xset != yset) {
        parent[xset] = yset;
    }
}

int compare_edges(const void *a, const void *b) {
    const struct Edge *e1 = (const struct Edge *)a;
    const struct Edge *e2 = (const struct Edge *)b;
    return e1->weight - e2->weight;
}

void kruskal_mst(struct Graph *g) {
    if (g == NULL || g->num_nodes == 0) return;
    
    struct Edge result[MAX_NODES];
    int e = 0;
    int i = 0;
    
    qsort(g->edges, g->num_edges, sizeof(struct Edge), compare_edges);
    
    int *parent = malloc(g->num_nodes * sizeof(int));
    if (parent == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }
    
    for (int v = 0; v < g->num_nodes; v++) {
        *(parent + v) = -1;
    }
    
    while (e < g->num_nodes - 1 && i < g->num_edges) {
        struct Edge *next_edge = g->edges + i;
        i++;
        
        int x = find(parent, next_edge->src);
        int y = find(parent, next_edge->dest);
        
        if (x != y) {
            *(result + e) = *next_edge;
            e++;
            union_sets(parent, x, y);
        }
    }
    
    printf("Minimum Spanning Tree edges:\n");
    int total_weight = 0;
    for (int j = 0; j < e; j++) {
        struct Edge *edge = result + j;
        printf("  %d -> %d (weight: %d)\n", edge->src, edge->dest, edge->weight);
        total_weight += edge->weight;
    }
    printf("Total weight: %d\n", total_weight);
    
    free(parent);
}

int main() {
    struct Graph g;
    init_graph(&g, 4, 0);
    
    add_edge(&g, 0, 1, 10);
    add_edge(&g, 0, 2, 6);
    add_edge(&g, 0, 3, 5);
    add_edge(&g, 1, 3, 15);
    add_edge(&g, 2, 3, 4);
    
    print_graph(&g);
    kruskal_mst(&g);
    
    return 0;
}