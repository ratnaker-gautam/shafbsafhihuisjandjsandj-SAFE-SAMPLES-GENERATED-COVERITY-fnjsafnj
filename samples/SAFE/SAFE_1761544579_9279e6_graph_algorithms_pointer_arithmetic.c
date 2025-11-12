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
    if (g->num_edges >= MAX_EDGES) {
        return 0;
    }
    if (src < 0 || src >= g->num_nodes || dest < 0 || dest >= g->num_nodes) {
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
    printf("Graph with %d nodes and %d edges:\n", g->num_nodes, g->num_edges);
    struct Edge *end = g->edges + g->num_edges;
    for (struct Edge *e = g->edges; e != end; e++) {
        printf("  %d -> %d (weight %d)\n", e->src, e->dest, e->weight);
    }
}

int find(int parent[], int i) {
    if (parent[i] == -1) {
        return i;
    }
    return find(parent, parent[i]);
}

void union_sets(int parent[], int x, int y) {
    int xset = find(parent, x);
    int yset = find(parent, y);
    parent[xset] = yset;
}

int compare_edges(const void *a, const void *b) {
    const struct Edge *e1 = (const struct Edge *)a;
    const struct Edge *e2 = (const struct Edge *)b;
    return e1->weight - e2->weight;
}

void kruskal_mst(struct Graph *g) {
    if (g->num_nodes == 0) {
        printf("Graph is empty\n");
        return;
    }
    
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
    
    struct Edge *end = g->edges + g->num_edges;
    for (struct Edge *next_edge = g->edges; next_edge != end; next_edge++) {
        if (e >= g->num_nodes - 1) {
            break;
        }
        
        int x = find(parent, next_edge->src);
        int y = find(parent, next_edge->dest);
        
        if (x != y) {
            *(result + e) = *next_edge;
            e++;
            union_sets(parent, x, y);
        }
        i++;
    }
    
    printf("Minimum Spanning Tree edges:\n");
    for (i = 0; i < e; i++) {
        printf("  %d -> %d (weight %d)\n", (result + i)->src, (result + i)->dest, (result + i)->weight);
    }
    
    free(parent);
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
    add_edge(&g, 5, 2, 2);
    add_edge(&g, 5, 4, 3);
    
    print_graph(&g);
    kruskal_mst(&g);
    
    return 0;
}