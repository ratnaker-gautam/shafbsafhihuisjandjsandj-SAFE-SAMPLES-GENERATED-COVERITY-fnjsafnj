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
        return;
    }
    g->num_nodes = nodes;
    g->num_edges = edges;
    for (int i = 0; i < edges; i++) {
        (g->edges + i)->src = -1;
        (g->edges + i)->dest = -1;
        (g->edges + i)->weight = 0;
    }
}

int add_edge(struct Graph *g, int src, int dest, int weight) {
    if (g == NULL || src < 0 || src >= g->num_nodes || dest < 0 || dest >= g->num_nodes || weight < 0) {
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
    if (g == NULL) {
        return;
    }
    printf("Graph with %d nodes and %d edges:\n", g->num_nodes, g->num_edges);
    for (int i = 0; i < g->num_edges; i++) {
        struct Edge *e = g->edges + i;
        printf("Edge %d: %d -> %d (weight %d)\n", i, e->src, e->dest, e->weight);
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
    const struct Edge *e1 = (const struct Edge *)a;
    const struct Edge *e2 = (const struct Edge *)b;
    if (e1->weight < e2->weight) return -1;
    if (e1->weight > e2->weight) return 1;
    return 0;
}

void kruskal_mst(struct Graph *g) {
    if (g == NULL || g->num_nodes <= 0 || g->num_edges <= 0) {
        return;
    }
    struct Edge result[MAX_NODES];
    int e = 0;
    int i = 0;
    qsort(g->edges, (size_t)g->num_edges, sizeof(struct Edge), compare_edges);
    int parent[MAX_NODES];
    int rank[MAX_NODES];
    for (int v = 0; v < g->num_nodes; v++) {
        parent[v] = v;
        rank[v] = 0;
    }
    while (e < g->num_nodes - 1 && i < g->num_edges) {
        struct Edge *next_edge = g->edges + i;
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
        printf("%d -- %d (weight %d)\n", result[j].src, result[j].dest, result[j].weight);
        total_weight += result[j].weight;
    }
    printf("Total weight: %d\n", total_weight);
}

int main() {
    struct Graph g;
    int nodes, edges;
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &nodes) != 1 || nodes < 1 || nodes > MAX_NODES) {
        printf("Invalid number of nodes\n");
        return 1;
    }
    printf("Enter number of edges (0-%d): ", MAX_EDGES);
    if (scanf("%d", &edges) != 1 || edges < 0 || edges > MAX_EDGES) {
        printf("Invalid number of edges\n");
        return 1;
    }
    init_graph(&g, nodes, edges);
    printf("Enter edges as: source destination weight\n");
    for (int i = 0; i < edges; i++) {
        int src, dest, weight;
        printf("Edge %d: