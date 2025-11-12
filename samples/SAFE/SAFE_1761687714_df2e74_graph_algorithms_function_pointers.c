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
        return;
    }
    if (weight < 0 || weight > 1000) {
        fprintf(stderr, "Invalid weight\n");
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
    printf("Neighbors of node %d with weight >= %d:\n", node, threshold);
    int found = 0;
    for (int i = 0; i < g->node_count; i++) {
        if (g->adjacency[node][i] >= threshold) {
            printf("  Node %d (weight: %d)\n", i, g->adjacency[node][i]);
            found = 1;
        }
    }
    if (!found) {
        printf("  No neighbors found\n");
    }
}

void calculate_degree(Graph *g, int node, int unused) {
    if (node < 0 || node >= g->node_count) {
        fprintf(stderr, "Invalid node\n");
        return;
    }
    int degree = 0;
    for (int i = 0; i < g->node_count; i++) {
        if (g->adjacency[node][i] != 0) {
            degree++;
        }
    }
    printf("Degree of node %d: %d\n", node, degree);
}

void execute_operation(Graph *g, GraphOperation op, int param1, int param2) {
    if (g == NULL || op == NULL) {
        fprintf(stderr, "Invalid parameters\n");
        return;
    }
    op(g, param1, param2);
}

int main() {
    Graph g;
    initialize_graph(&g, 6);
    
    add_edge(&g, 0, 1, 5);
    add_edge(&g, 0, 2, 3);
    add_edge(&g, 1, 2, 2);
    add_edge(&g, 1, 3, 7);
    add_edge(&g, 2, 4, 4);
    add_edge(&g, 3, 4, 1);
    add_edge(&g, 3, 5, 6);
    add_edge(&g, 4, 5, 8);
    
    GraphOperation operations[] = {
        print_adjacency,
        find_neighbors,
        calculate_degree
    };
    
    int operation_count = sizeof(operations) / sizeof(operations[0]);
    
    printf("Graph Operations Demo\n");
    printf("====================\n\n");
    
    for (int i = 0; i < operation_count; i++) {
        switch (i) {
            case 0:
                printf("Operation 1: Print adjacency\n");
                for (int node = 0; node < g.node_count; node++) {
                    execute_operation(&g, operations[i], node, 0);
                }
                break;
            case 1:
                printf("\nOperation 2: Find neighbors with weight >= 3\n");
                for (int node = 0; node < g.node_count; node++) {
                    execute_operation(&g, operations[i], node, 3);
                }
                break;
            case 2:
                printf("\nOperation 3: Calculate node degrees\n");
                for (int node = 0; node < g.node_count; node++) {
                    execute_operation(&g, operations[i], node, 0);
                }
                break;
        }
        printf("\n");
    }
    
    return 0;
}