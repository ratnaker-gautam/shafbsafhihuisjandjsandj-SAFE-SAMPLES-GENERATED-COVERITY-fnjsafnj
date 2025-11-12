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

void init_graph(struct Graph *g, int nodes) {
    if (nodes <= 0 || nodes > MAX_NODES) {
        fprintf(stderr, "Invalid number of nodes\n");
        exit(1);
    }
    g->num_nodes = nodes;
    g->num_edges = 0;
}

int add_edge(struct Graph *g, int src, int dest, int weight) {
    if (g->num_edges >= MAX_EDGES) return 0;
    if (src < 0 || src >= g->num_nodes) return 0;
    if (dest < 0 || dest >= g->num_nodes) return 0;
    
    struct Edge *e = g->edges + g->num_edges;
    e->src = src;
    e->dest = dest;
    e->weight = weight;
    g->num_edges++;
    return 1;
}

void print_graph(struct Graph *g) {
    printf("Graph with %d nodes and %d edges:\n", g->num_nodes, g->num_edges);
    struct Edge *end = g->edges + g->num_edges;
    for (struct Edge *e = g->edges; e != end; e++) {
        printf("  %d -> %d (weight: %d)\n", e->src, e->dest, e->weight);
    }
}

int find(int parent[], int i) {
    if (parent[i] != i) {
        parent[i] = find(parent, parent[i]);
    }
    return parent[i];
}

void union_sets(int parent[], int rank[], int x, int y) {
    int xroot = find(parent, x);
    int yroot = find(parent, y);
    
    if (rank[xroot] < rank[yroot]) {
        parent[xroot] = yroot;
    } else if (rank[xroot] > rank[yroot]) {
        parent[yroot] = xroot;
    } else {
        parent[yroot] = xroot;
        rank[xroot]++;
    }
}

int compare_edges(const void *a, const void *b) {
    const struct Edge *e1 = (const struct Edge*)a;
    const struct Edge *e2 = (const struct Edge*)b;
    if (e1->weight < e2->weight) return -1;
    if (e1->weight > e2->weight) return 1;
    return 0;
}

void kruskal_mst(struct Graph *g) {
    if (g->num_nodes == 0) return;
    
    struct Edge result[MAX_NODES];
    int parent[MAX_NODES];
    int rank[MAX_NODES];
    
    for (int i = 0; i < g->num_nodes; i++) {
        parent[i] = i;
        rank[i] = 0;
    }
    
    struct Edge sorted_edges[MAX_EDGES];
    if (g->num_edges > 0) {
        memcpy(sorted_edges, g->edges, g->num_edges * sizeof(struct Edge));
        qsort(sorted_edges, g->num_edges, sizeof(struct Edge), compare_edges);
    }
    
    int e = 0;
    int i = 0;
    struct Edge *end = sorted_edges + g->num_edges;
    
    while (e < g->num_nodes - 1 && (sorted_edges + i) != end) {
        struct Edge *next_edge = sorted_edges + i;
        i++;
        
        int x = find(parent, next_edge->src);
        int y = find(parent, next_edge->dest);
        
        if (x != y) {
            result[e] = *next_edge;
            e++;
            union_sets(parent, rank, x, y);
        }
    }
    
    printf("Minimum Spanning Tree edges:\n");
    int total_weight = 0;
    for (int j = 0; j < e; j++) {
        printf("  %d -> %d (weight: %d)\n", result[j].src, result[j].dest, result[j].weight);
        total_weight += result[j].weight;
    }
    printf("Total weight: %d\n", total_weight);
}

int main() {
    struct Graph g;
    init_graph(&g, 6);
    
    add_edge(&g, 0, 1, 4);
    add_edge(&g, 0, 2, 4);
    add_edge(&g, 1, 2, 2);
    add_edge(&g, 1, 0, 4);
    add_edge(&g, 2, 0, 4);
    add_edge(&g, 2, 1, 2);
    add_edge(&g, 2, 3, 3);
    add_edge(&g, 2, 5, 2);
    add_edge(&g, 2, 4, 4);
    add_edge(&g, 3, 2, 3);
    add_edge(&g, 3, 4, 3);
    add_edge(&g, 4, 2, 4);
    add_edge(&g, 4, 3, 3);
    add_edge(&g