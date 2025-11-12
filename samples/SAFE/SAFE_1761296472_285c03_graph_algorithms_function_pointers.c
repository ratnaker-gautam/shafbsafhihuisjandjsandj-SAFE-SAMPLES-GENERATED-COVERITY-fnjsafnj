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

void initialize_graph(Graph *g, int node_count) {
    if (node_count <= 0 || node_count > MAX_NODES) {
        fprintf(stderr, "Invalid node count\n");
        exit(EXIT_FAILURE);
    }
    g->node_count = node_count;
    for (int i = 0; i < node_count; i++) {
        for (int j = 0; j < node_count; j++) {
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
        fprintf(stderr, "Invalid edge weight\n");
        return;
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

void find_shortest_edge(Graph *g, int start, int unused) {
    if (start < 0 || start >= g->node_count) {
        fprintf(stderr, "Invalid start node\n");
        return;
    }
    int min_weight = INT_MAX;
    int min_neighbor = -1;
    for (int i = 0; i < g->node_count; i++) {
        if (g->adjacency[start][i] != 0 && g->adjacency[start][i] < min_weight) {
            min_weight = g->adjacency[start][i];
            min_neighbor = i;
        }
    }
    if (min_neighbor != -1) {
        printf("Shortest edge from node %d: to node %d (weight: %d)\n", start, min_neighbor, min_weight);
    } else {
        printf("No outgoing edges from node %d\n", start);
    }
}

void count_outgoing_edges(Graph *g, int start, int unused) {
    if (start < 0 || start >= g->node_count) {
        fprintf(stderr, "Invalid start node\n");
        return;
    }
    int count = 0;
    for (int i = 0; i < g->node_count; i++) {
        if (g->adjacency[start][i] != 0) {
            count++;
        }
    }
    printf("Node %d has %d outgoing edges\n", start, count);
}

void execute_operation(Graph *g, GraphOperation op, int node, int param) {
    if (g == NULL || op == NULL) {
        fprintf(stderr, "Invalid graph or operation\n");
        return;
    }
    op(g, node, param);
}

int main() {
    Graph g;
    initialize_graph(&g, 6);
    
    add_edge(&g, 0, 1, 4);
    add_edge(&g, 0, 2, 2);
    add_edge(&g, 1, 2, 5);
    add_edge(&g, 1, 3, 10);
    add_edge(&g, 2, 4, 3);
    add_edge(&g, 3, 5, 1);
    add_edge(&g, 4, 3, 2);
    add_edge(&g, 4, 5, 8);
    
    GraphOperation operations[] = {
        print_adjacency,
        find_shortest_edge,
        count_outgoing_edges
    };
    
    int operation_count = sizeof(operations) / sizeof(operations[0]);
    
    printf("Graph Operations Demo:\n");
    printf("======================\n");
    
    for (int i = 0; i < g.node_count; i++) {
        printf("\nProcessing node %d:\n", i);
        for (int j = 0; j < operation_count; j++) {
            execute_operation(&g, operations[j], i, 0);
        }
    }
    
    return 0;
}