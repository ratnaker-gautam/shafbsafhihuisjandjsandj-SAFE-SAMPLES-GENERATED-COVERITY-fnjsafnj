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

int find_set(int *parent, int node) {
    if (parent == NULL) return -1;
    while (*(parent + node) != node) {
        node = *(parent + node);
    }
    return node;
}

void union_sets(int *parent, int *rank, int x, int y) {
    if (parent == NULL || rank == NULL) return;
    int root_x = find_set(parent, x);
    int root_y = find_set(parent, y);
    if (root_x == root_y) return;
    if (*(rank + root_x) < *(rank + root_y)) {
        *(parent + root_x) = root_y;
    } else if (*(rank + root_x) > *(rank + root_y)) {
        *(parent + root_y) = root_x;
    } else {
        *(parent + root_y) = root_x;
        *(rank + root_x) += 1;
    }
}

int compare_edges(const void *a, const void *b) {
    const struct Edge *edge_a = (const struct Edge *)a;
    const struct Edge *edge_b = (const struct Edge *)b;
    if (edge_a->weight < edge_b->weight) return -1;
    if (edge_a->weight > edge_b->weight) return 1;
    return 0;
}

void kruskal_mst(struct Graph *g) {
    if (g == NULL) return;
    struct Edge result[MAX_NODES];
    int parent[MAX_NODES];
    int rank[MAX_NODES];
    for (int i = 0; i < g->num_nodes; i++) {
        *(parent + i) = i;
        *(rank + i) = 0;
    }
    qsort(g->edges, g->num_edges, sizeof(struct Edge), compare_edges);
    int e = 0;
    int i = 0;
    while (e < g->num_nodes - 1 && i < g->num_edges) {
        struct Edge *next_edge = g->edges + i;
        i++;
        int x = find_set(parent, next_edge->src);
        int y = find_set(parent, next_edge->dest);
        if (x != y) {
            *(result + e) = *next_edge;
            e++;
            union_sets(parent, rank, x, y);
        }
    }
    printf("Minimum Spanning Tree edges:\n");
    int total_weight = 0;
    for (int j = 0; j < e; j++) {
        struct Edge *edge_ptr = result + j;
        printf("  %d -> %d (weight: %d)\n", edge_ptr->src, edge_ptr->dest, edge_ptr->weight);
        total_weight += edge_ptr->weight;
    }
    printf("Total weight: %d\n", total_weight);
}

int main() {
    struct Graph g;
    int nodes, edges;
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &nodes) != 1 || nodes < 1 || nodes > MAX_NODES) {
        fprintf(stderr, "Invalid number of nodes\n");
        return 1;
    }
    printf("Enter number of edges (0-%d): ", MAX_EDGES);
    if (scanf("%d", &edges) != 1 || edges < 0 || edges > MAX_EDGES) {
        fprintf(stderr, "Invalid number of edges\n");
        return 1;
    }
    init_graph(&g, nodes, edges);
    printf("Enter edges as: source destination weight (0-indexed nodes)\n");
    for (int i