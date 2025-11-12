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
        fprintf(stderr, "Invalid node index\n");
        exit(1);
    }
    if (weight < 0 || weight > 100) {
        fprintf(stderr, "Invalid edge weight\n");
        exit(1);
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
    if (threshold < 0) {
        fprintf(stderr, "Invalid threshold\n");
        return;
    }
    printf("Neighbors of node %d with weight >= %d:\n", node, threshold);
    int found = 0;
    for (int i = 0; i < g->node_count; i++) {
        if (g->adjacency[node][i] >= threshold) {
            printf("  -> %d (weight: %d)\n", i, g->adjacency[node][i]);
            found = 1;
        }
    }
    if (!found) {
        printf("  No neighbors found\n");
    }
}

void perform_operation(Graph *g, GraphOperation op, int param1, int param2) {
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
    
    printf("Graph operations using function pointers:\n\n");
    
    GraphOperation operations[] = {print_adjacency, find_neighbors};
    int operation_count = sizeof(operations) / sizeof(operations[0]);
    
    int test_cases[][3] = {
        {0, 0, 0},
        {1, 0, 0},
        {0, 0, 3},
        {3, 0, 2},
        {4, 0, 0}
    };
    int test_count = sizeof(test_cases) / sizeof(test_cases[0]);
    
    for (int i = 0; i < test_count; i++) {
        int node = test_cases[i][0];
        int op_index = test_cases[i][1];
        int param = test_cases[i][2];
        
        if (op_index < 0 || op_index >= operation_count) {
            fprintf(stderr, "Invalid operation index\n");
            continue;
        }
        
        printf("Operation %d: ", i + 1);
        perform_operation(&g, operations[op_index], node, param);
        printf("\n");
    }
    
    return 0;
}