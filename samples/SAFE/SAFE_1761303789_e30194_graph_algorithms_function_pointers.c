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
        return;
    }
    if (weight < 0) {
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
        printf("No edges from node %d\n", start);
    }
}

void count_neighbors(Graph *g, int start, int unused) {
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
    printf("Node %d has %d outgoing neighbors\n", start, count);
}

void execute_graph_operation(Graph *g, GraphOperation op, int node1, int node2) {
    if (g == NULL || op == NULL) {
        fprintf(stderr, "Invalid graph or operation\n");
        return;
    }
    op(g, node1, node2);
}

int main() {
    Graph g;
    initialize_graph(&g, 5);
    
    add_edge(&g, 0, 1, 4);
    add_edge(&g, 0, 2, 1);
    add_edge(&g, 1, 3, 3);
    add_edge(&g, 2, 1, 2);
    add_edge(&g, 2, 3, 5);
    add_edge(&g, 3, 4, 2);
    add_edge(&g, 4, 0, 3);
    
    GraphOperation operations[] = {
        print_adjacency,
        find_shortest_edge,
        count_neighbors
    };
    const char* operation_names[] = {
        "Print adjacency",
        "Find shortest edge",
        "Count neighbors"
    };
    int op_count = sizeof(operations) / sizeof(operations[0]);
    
    printf("Graph operations demonstration:\n");
    for (int i = 0; i < op_count; i++) {
        printf("\nOperation: %s\n", operation_names[i]);
        for (int node = 0; node < g.node_count; node++) {
            execute_graph_operation(&g, operations[i], node, 0);
        }
    }
    
    return 0;
}