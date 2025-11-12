//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: function_pointers ; Variation: graph_algorithms
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
        fprintf(stderr, "Invalid node index\n");
        exit(1);
    }
    if (weight < 0 || weight > 1000) {
        fprintf(stderr, "Invalid edge weight\n");
        exit(1);
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
    printf("Neighbors of node %d with weight <= %d:\n", node, threshold);
    int found = 0;
    for (int i = 0; i < g->node_count; i++) {
        if (g->adjacency[node][i] != 0 && g->adjacency[node][i] <= threshold) {
            printf("  Node %d (weight: %d)\n", i, g->adjacency[node][i]);
            found = 1;
        }
    }
    if (!found) {
        printf("  No neighbors found\n");
    }
}

void apply_to_neighbors(Graph *g, int node, int threshold, GraphOperation op) {
    if (node < 0 || node >= g->node_count) {
        fprintf(stderr, "Invalid node\n");
        return;
    }
    if (op == NULL) {
        fprintf(stderr, "Invalid operation\n");
        return;
    }
    for (int i = 0; i < g->node_count; i++) {
        if (g->adjacency[node][i] != 0 && g->adjacency[node][i] <= threshold) {
            op(g, node, i);
        }
    }
}

void print_edge(Graph *g, int from, int to) {
    if (from < 0 || from >= g->node_count || to < 0 || to >= g->node_count) {
        fprintf(stderr, "Invalid node indices\n");
        return;
    }
    printf("Edge %d-%d: weight %d\n", from, to, g->adjacency[from][to]);
}

void process_graph(Graph *g, GraphOperation ops[], int op_count, int node, int threshold) {
    if (g == NULL || ops == NULL) {
        fprintf(stderr, "Invalid graph or operations\n");
        return;
    }
    if (node < 0 || node >= g->node_count) {
        fprintf(stderr, "Invalid node\n");
        return;
    }
    if (op_count <= 0) {
        fprintf(stderr, "Invalid operation count\n");
        return;
    }
    for (int i = 0; i < op_count; i++) {
        if (ops[i] != NULL) {
            ops[i](g, node, threshold);
        }
    }
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
    add_edge(&g, 4, 5, 6);
    
    printf("Graph operations demonstration:\n\n");
    
    GraphOperation single_ops[] = {print_adjacency, find_neighbors};
    process_graph(&g, single_ops, 2, 1, 3);
    
    printf("\nApplying operations to neighbors of node 1:\n");
    apply_to_neighbors(&g, 1, 5, print_edge);
    
    GraphOperation multi_ops[] = {print_adjacency, find_neighbors};
    printf("\nMulti-operation processing for