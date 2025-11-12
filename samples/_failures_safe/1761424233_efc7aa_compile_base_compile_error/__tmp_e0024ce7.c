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

void print_graph(const struct Graph *g) {
    if (g == NULL) {
        return;
    }
    printf("Graph with %d nodes and %d edges:\n", g->num_nodes, g->num_edges);
    const struct Edge *e = g->edges;
    for (int i = 0; i < g->num_edges; i++, e++) {
        printf("  %d -> %d (weight: %d)\n", e->src, e->dest, e->weight);
    }
}

int find_set(int *parent, int node) {
    if (parent == NULL || node < 0) {
        return -1;
    }
    while (parent[node] != node) {
        parent[node] = parent[parent[node]];
        node = parent[node];
    }
    return node;
}

int union_sets(int *parent, int *rank, int x, int y) {
    if (parent == NULL || rank == NULL || x < 0 || y < 0) {
        return 0;
    }
    int root_x = find_set(parent, x);
    int root_y = find_set(parent, y);
    if (root_x == root_y) {
        return 0;
    }
    if (rank[root_x] < rank[root_y]) {
        *(parent + root_x) = root_y;
    } else if (rank[root_x] > rank[root_y]) {
        *(parent + root_y) = root_x;
    } else {
        *(parent + root_y) = root_x;
        (*(rank + root_x))++;
    }
    return 1;
}

int compare_edges(const void *a, const void *b) {
    const struct Edge *ea = (const struct Edge *)a;
    const struct Edge *eb = (const struct Edge *)b;
    if (ea->weight < eb->weight) return -1;
    if (ea->weight > eb->weight) return 1;
    return 0;
}

void kruskal_mst(const struct Graph *g) {
    if (g == NULL || g->num_nodes <= 0 || g->num_edges < 0) {
        return;
    }
    struct Edge result[MAX_NODES];
    int parent[MAX_NODES];
    int rank[MAX_NODES];
    for (int i = 0; i < g->num_nodes; i++) {
        parent[i] = i;
        rank[i] = 0;
    }
    struct Edge sorted_edges[MAX_EDGES];
    memcpy(sorted_edges, g->edges, g->num_edges * sizeof(struct Edge));
    qsort(sorted_edges, g->num_edges, sizeof(struct Edge), compare_edges);
    int e = 0;
    int i = 0;
    struct Edge *next_edge = sorted_edges;
    while (e < g->num_nodes - 1 && i < g->num_edges) {
        if (next_edge->src < 0 || next_edge->src >= g->num_nodes || next_edge->dest < 0 || next_edge->dest >= g->num_nodes) {
            next_edge++;
            i++;
            continue;
        }
        int x = find_set(parent, next_edge->src);
        int y = find_set(parent, next_edge->dest);
        if (x != y && x != -1 && y != -1) {
            result[e] = *next_edge;
            e++;
            union_sets(parent, rank, x, y);
        }
        next_edge++;
        i++;
    }
    printf("Minimum Spanning Tree edges:\n");
    int total_weight = 0;
    struct Edge *mst_edge = result;
    for (int j = 0; j < e; j++, mst_edge++) {
        printf("  %d -> %d (weight: %d)\n", mst_edge->src, mst_edge->dest, mst_edge->weight);
        total_weight += mst_edge->weight;
    }
    printf("Total MST weight: %d\n", total_weight);
}

int main() {
    struct Graph g;
    init_graph(&g, 5, 0);
    add_edge(&