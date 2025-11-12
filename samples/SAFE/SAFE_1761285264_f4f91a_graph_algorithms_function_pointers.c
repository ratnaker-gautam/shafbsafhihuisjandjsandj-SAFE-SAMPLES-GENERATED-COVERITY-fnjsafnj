//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_NODES 100

typedef struct {
    int adjacency[MAX_NODES][MAX_NODES];
    int node_count;
} Graph;

typedef void (*GraphOperation)(Graph*, int, int);

void initialize_graph(Graph *g, int nodes) {
    if (nodes <= 0 || nodes > MAX_NODES) {
        fprintf(stderr, "Invalid node count\n");
        exit(1);
    }
    g->node_count = nodes;
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            g->adjacency[i][j] = 0;
        }
    }
}

void add_edge(Graph *g, int from, int to, int weight) {
    if (from < 0 || from >= g->node_count || to < 0 || to >= g->node_count) {
        fprintf(stderr, "Invalid edge nodes\n");
        return;
    }
    if (weight < 0 || weight > 1000) {
        fprintf(stderr, "Invalid edge weight\n");
        return;
    }
    g->adjacency[from][to] = weight;
    g->adjacency[to][from] = weight;
}

void print_adjacency(Graph *g, int start, int unused) {
    if (start < 0 || start >= g->node_count) {
        fprintf(stderr, "Invalid start node\n");
        return;
    }
    printf("Adjacency list for node %d:\n", start);
    for (int i = 0; i < g->node_count; i++) {
        if (g->adjacency[start][i] != 0) {
            printf("  -> %d (weight: %d)\n", i, g->adjacency[start][i]);
        }
    }
}

void find_neighbors(Graph *g, int node, int threshold) {
    if (node < 0 || node >= g->node_count) {
        fprintf(stderr, "Invalid node\n");
        return;
    }
    if (threshold < 0) {
        fprintf(stderr, "Invalid threshold\n");
        return;
    }
    printf("Neighbors of node %d with weight <= %d:\n", node, threshold);
    int found = 0;
    for (int i = 0; i < g->node_count; i++) {
        if (g->adjacency[node][i] != 0 && g->adjacency[node][i] <= threshold) {
            printf("  %d (weight: %d)\n", i, g->adjacency[node][i]);
            found = 1;
        }
    }
    if (!found) {
        printf("  No neighbors found\n");
    }
}

void process_graph(Graph *g, GraphOperation op, int param1, int param2) {
    if (g == NULL || op == NULL) {
        fprintf(stderr, "Invalid graph or operation\n");
        return;
    }
    op(g, param1, param2);
}

int main() {
    Graph g;
    initialize_graph(&g, 6);
    
    add_edge(&g, 0, 1, 4);
    add_edge(&g, 0, 2, 2);
    add_edge(&g, 1, 2, 1);
    add_edge(&g, 1, 3, 5);
    add_edge(&g, 2, 3, 8);
    add_edge(&g, 3, 4, 3);
    add_edge(&g, 4, 5, 7);
    
    printf("Graph operations:\n\n");
    
    process_graph(&g, print_adjacency, 0, 0);
    printf("\n");
    
    process_graph(&g, print_adjacency, 1, 0);
    printf("\n");
    
    process_graph(&g, find_neighbors, 0, 3);
    printf("\n");
    
    process_graph(&g, find_neighbors, 1, 2);
    printf("\n");
    
    process_graph(&g, find_neighbors, 3, 5);
    
    return 0;
}