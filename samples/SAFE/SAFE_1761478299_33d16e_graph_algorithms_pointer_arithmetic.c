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
    if (nodes <= 0 || nodes > MAX_NODES || edges < 0 || edges > MAX_EDGES) {
        fprintf(stderr, "Invalid graph parameters\n");
        exit(1);
    }
    g->num_nodes = nodes;
    g->num_edges = edges;
}

int add_edge(struct Graph *g, int src, int dest, int weight, int edge_idx) {
    if (edge_idx < 0 || edge_idx >= g->num_edges) {
        return 0;
    }
    if (src < 0 || src >= g->num_nodes || dest < 0 || dest >= g->num_nodes) {
        return 0;
    }
    (g->edges + edge_idx)->src = src;
    (g->edges + edge_idx)->dest = dest;
    (g->edges + edge_idx)->weight = weight;
    return 1;
}

void print_graph(struct Graph *g) {
    printf("Graph with %d nodes and %d edges:\n", g->num_nodes, g->num_edges);
    for (int i = 0; i < g->num_edges; i++) {
        struct Edge *e = g->edges + i;
        printf("Edge %d: %d -> %d (weight %d)\n", i, e->src, e->dest, e->weight);
    }
}

int find(int parent[], int i) {
    if (*(parent + i) == -1) {
        return i;
    }
    return find(parent, *(parent + i));
}

void union_sets(int parent[], int x, int y) {
    int xset = find(parent, x);
    int yset = find(parent, y);
    if (xset != yset) {
        *(parent + xset) = yset;
    }
}

int compare_edges(const void *a, const void *b) {
    struct Edge *e1 = (struct Edge *)a;
    struct Edge *e2 = (struct Edge *)b;
    return e1->weight - e2->weight;
}

void kruskal_mst(struct Graph *g) {
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
        printf("%d -- %d (weight %d)\n", edge->src, edge->dest, edge->weight);
        total_weight += edge->weight;
    }
    printf("Total weight: %d\n", total_weight);
    
    free(parent);
}

int main() {
    struct Graph g;
    int num_nodes, num_edges;
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &num_nodes) != 1 || num_nodes < 1 || num_nodes > MAX_NODES) {
        fprintf(stderr, "Invalid number of nodes\n");
        return 1;
    }
    
    printf("Enter number of edges (0-%d): ", MAX_EDGES);
    if (scanf("%d", &num_edges) != 1 || num_edges < 0 || num_edges > MAX_EDGES) {
        fprintf(stderr, "Invalid number of edges\n");
        return 1;
    }
    
    init_graph(&g, num_nodes, num_edges);
    
    printf("Enter edges as: source destination weight\n");
    for (int i = 0; i < num_edges; i++) {
        int src, dest, weight;
        printf("Edge %d: ", i);
        if (scanf("%d %d %d", &src, &dest, &weight) != 3) {
            fprintf(stderr, "Invalid edge input\n");
            return 1;
        }
        if (!add_edge(&g, src, dest, weight, i)) {
            fprintf(stderr, "Failed to add edge\n");
            return 1;
        }
    }
    
    print_graph(&g);
    kruskal_mst(&g);
    
    return 0;
}