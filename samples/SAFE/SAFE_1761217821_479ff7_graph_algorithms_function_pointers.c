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
    if (weight < 0) {
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
        if (g->adjacency[start][i] > 0) {
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
    int min_node = -1;
    for (int i = 0; i < g->node_count; i++) {
        if (g->adjacency[start][i] > 0 && g->adjacency[start][i] < min_weight) {
            min_weight = g->adjacency[start][i];
            min_node = i;
        }
    }
    if (min_node != -1) {
        printf("Shortest edge from node %d: to node %d (weight: %d)\n", start, min_node, min_weight);
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
        if (g->adjacency[start][i] > 0) {
            count++;
        }
    }
    printf("Node %d has %d outgoing edges\n", start, count);
}

void perform_operation(Graph *g, GraphOperation op, int node1, int node2) {
    if (g == NULL || op == NULL) {
        fprintf(stderr, "Invalid operation parameters\n");
        return;
    }
    op(g, node1, node2);
}

int main() {
    Graph g;
    initialize_graph(&g, 5);
    
    add_edge(&g, 0, 1, 4);
    add_edge(&g, 0, 2, 1);
    add_edge(&g, 1, 2, 2);
    add_edge(&g, 1, 3, 5);
    add_edge(&g, 2, 3, 8);
    add_edge(&g, 3, 4, 3);
    add_edge(&g, 4, 0, 7);
    
    GraphOperation operations[] = {
        print_adjacency,
        find_shortest_edge,
        count_outgoing_edges
    };
    
    int operation_count = sizeof(operations) / sizeof(operations[0]);
    
    printf("Graph Operations:\n");
    printf("=================\n");
    
    for (int i = 0; i < g.node_count; i++) {
        for (int j = 0; j < operation_count; j++) {
            perform_operation(&g, operations[j], i, 0);
        }
        printf("\n");
    }
    
    return 0;
}