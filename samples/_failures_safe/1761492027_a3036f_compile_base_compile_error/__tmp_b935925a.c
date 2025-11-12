//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 100
#define MAX_EDGES 1000

struct Graph {
    int nodes;
    int edges[MAX_EDGES][3];
    int edge_count;
};

void initialize_graph(struct Graph *g, int nodes) {
    if (nodes <= 0 || nodes > MAX_NODES) {
        printf("Invalid number of nodes. Must be between 1 and %d.\n", MAX_NODES);
        exit(1);
    }
    g->nodes = nodes;
    g->edge_count = 0;
}

void add_edge(struct Graph *g, int u, int v, int weight) {
    if (g->edge_count >= MAX_EDGES) {
        printf("Too many edges. Maximum is %d.\n", MAX_EDGES);
        return;
    }
    if (u < 0 || u >= g->nodes || v < 0 || v >= g->nodes) {
        printf("Invalid node indices. Must be between 0 and %d.\n", g->nodes - 1);
        return;
    }
    g->edges[g->edge_count][0] = u;
    g->edges[g->edge_count][1] = v;
    g->edges[g->edge_count][2] = weight;
    g->edge_count++;
}

void print_graph(struct Graph *g) {
    printf("Graph with %d nodes and %d edges:\n", g->nodes, g->edge_count);
    for (int i = 0; i < g->edge_count; i++) {
        printf("Edge %d: %d -> %d (weight %d)\n", i, g->edges[i][0], g->edges[i][1], g->edges[i][2]);
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
    const int *edge1 = (const int *)a;
    const int *edge2 = (const int *)b;
    if (edge1[2] < edge2[2]) return -1;
    if (edge1[2] > edge2[2]) return 1;
    return 0;
}

void kruskal_mst(struct Graph *g) {
    if (g->edge_count == 0) {
        printf("Graph has no edges.\n");
        return;
    }
    int parent[MAX_NODES];
    int rank[MAX_NODES];
    for (int i = 0; i < g->nodes; i++) {
        parent[i] = i;
        rank[i] = 0;
    }
    int result[MAX_EDGES][3];
    int result_count = 0;
    int total_weight = 0;
    int temp_edges[MAX_EDGES][3];
    for (int i = 0; i < g->edge_count; i++) {
        temp_edges[i][0] = g->edges[i][0];
        temp_edges[i][1] = g->edges[i][1];
        temp_edges[i][2] = g->edges[i][2];
    }
    qsort(temp_edges, g->edge_count, sizeof(temp_edges[0]), compare_edges);
    for (int i = 0; i < g->edge_count && result_count < g->nodes - 1; i++) {
        int u = temp_edges[i][0];
        int v = temp_edges[i][1];
        int weight = temp_edges[i][2];
        int set_u = find(parent, u);
        int set_v = find(parent, v);
        if (set_u != set_v) {
            result[result_count][0] = u;
            result[result_count][1] = v;
            result[result_count][2] = weight;
            result_count++;
            total_weight += weight;
            union_sets(parent, rank, set_u, set_v);
        }
    }
    if (result_count != g->nodes - 1) {
        printf("Graph is not connected. MST may not span all nodes.\n");
    }
    printf("Minimum Spanning Tree (Kruskal's algorithm):\n");
    printf("Total weight: %d\n", total_weight);
    for (int i = 0; i < result_count; i++) {
        printf("Edge: %d - %d (weight %d)\n", result[i][0], result[i][1], result[i][2]);
    }
}

int main() {
    struct Graph g;
    int nodes, edges;
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &nodes) != 1) {
        printf("Invalid input.\n");
        return 1;
    }
    initialize_graph(&g, nodes);
    printf("Enter number of edges (0-%d): ", MAX_EDGES