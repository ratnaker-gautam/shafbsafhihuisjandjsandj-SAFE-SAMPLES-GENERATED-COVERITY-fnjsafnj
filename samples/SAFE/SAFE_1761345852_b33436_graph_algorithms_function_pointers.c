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
        exit(EXIT_FAILURE);
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
        exit(EXIT_FAILURE);
    }
    if (weight < 0) {
        fprintf(stderr, "Invalid edge weight\n");
        exit(EXIT_FAILURE);
    }
    g->adjacency[from][to] = weight;
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
    printf("Neighbors of node %d with weight >= %d:\n", node, threshold);
    int found = 0;
    for (int i = 0; i < g->node_count; i++) {
        if (g->adjacency[node][i] >= threshold && g->adjacency[node][i] != 0) {
            printf("  -> %d (weight: %d)\n", i, g->adjacency[node][i]);
            found = 1;
        }
    }
    if (!found) {
        printf("  No neighbors found\n");
    }
}

void calculate_out_degree(Graph *g, int node, int unused) {
    if (node < 0 || node >= g->node_count) {
        fprintf(stderr, "Invalid node\n");
        return;
    }
    int count = 0;
    for (int i = 0; i < g->node_count; i++) {
        if (g->adjacency[node][i] != 0) {
            count++;
        }
    }
    printf("Out-degree of node %d: %d\n", node, count);
}

void execute_operation(Graph *g, GraphOperation op, int param1, int param2) {
    if (g == NULL || op == NULL) {
        fprintf(stderr, "Invalid operation parameters\n");
        return;
    }
    op(g, param1, param2);
}

int main() {
    Graph g;
    initialize_graph(&g, 6);
    
    add_edge(&g, 0, 1, 5);
    add_edge(&g, 0, 2, 3);
    add_edge(&g, 1, 3, 2);
    add_edge(&g, 2, 3, 7);
    add_edge(&g, 3, 4, 1);
    add_edge(&g, 4, 5, 4);
    add_edge(&g, 1, 4, 6);
    
    GraphOperation operations[] = {
        print_adjacency,
        find_neighbors,
        calculate_out_degree
    };
    
    int operation_count = sizeof(operations) / sizeof(operations[0]);
    
    printf("Graph Operations Demo:\n");
    printf("=====================\n");
    
    for (int i = 0; i < operation_count; i++) {
        for (int node = 0; node < g.node_count; node++) {
            if (i == 0) {
                execute_operation(&g, operations[i], node, 0);
            } else if (i == 1) {
                execute_operation(&g, operations[i], node, 3);
            } else {
                execute_operation(&g, operations[i], node, 0);
            }
        }
        printf("\n");
    }
    
    return 0;
}