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
        fprintf(stderr, "Invalid node indices\n");
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

void find_min_edge(Graph *g, int start, int unused) {
    if (start < 0 || start >= g->node_count) {
        fprintf(stderr, "Invalid start node\n");
        return;
    }
    int min_weight = INT_MAX;
    int min_node = -1;
    for (int i = 0; i < g->node_count; i++) {
        if (g->adjacency[start][i] != 0 && g->adjacency[start][i] < min_weight) {
            min_weight = g->adjacency[start][i];
            min_node = i;
        }
    }
    if (min_node != -1) {
        printf("Minimum edge from node %d: -> %d (weight: %d)\n", start, min_node, min_weight);
    } else {
        printf("No outgoing edges from node %d\n", start);
    }
}

void count_outgoing(Graph *g, int start, int unused) {
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

void execute_operation(Graph *g, GraphOperation op, int node1, int node2) {
    if (g == NULL || op == NULL) {
        fprintf(stderr, "Invalid graph or operation\n");
        return;
    }
    op(g, node1, node2);
}

int main() {
    Graph g;
    int node_count, edge_count;
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &node_count) != 1) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    
    initialize_graph(&g, node_count);
    
    printf("Enter number of edges: ");
    if (scanf("%d", &edge_count) != 1) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    
    if (edge_count < 0 || edge_count > node_count * node_count) {
        fprintf(stderr, "Invalid edge count\n");
        return EXIT_FAILURE;
    }
    
    for (int i = 0; i < edge_count; i++) {
        int from, to, weight;
        printf("Enter edge %d (from to weight): ", i + 1);
        if (scanf("%d %d %d", &from, &to, &weight) != 3) {
            fprintf(stderr, "Invalid edge data\n");
            return EXIT_FAILURE;
        }
        add_edge(&g, from, to, weight);
    }
    
    GraphOperation operations[] = {print_adjacency, find_min_edge, count_outgoing};
    int op_count = sizeof(operations) / sizeof(operations[0]);
    
    int choice;
    printf("\nChoose operation (0=print adjacency, 1=find min edge, 2=count outgoing): ");
    if (scanf("%d", &choice) != 1) {
        fprintf(stderr, "Invalid choice\n");
        return EXIT_FAILURE;
    }
    
    if (choice < 0 || choice >= op_count) {
        fprintf(stderr, "Invalid operation choice\n");
        return EXIT_FAILURE;
    }
    
    int target_node;
    printf("Enter target