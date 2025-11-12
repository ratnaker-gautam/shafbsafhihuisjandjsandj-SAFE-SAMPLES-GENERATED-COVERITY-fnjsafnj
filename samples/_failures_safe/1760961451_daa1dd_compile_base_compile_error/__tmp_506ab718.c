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
        fprintf(stderr, "Invalid edge nodes\n");
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
    int min_node = -1;
    for (int i = 0; i < g->node_count; i++) {
        if (g->adjacency[start][i] != 0 && g->adjacency[start][i] < min_weight) {
            min_weight = g->adjacency[start][i];
            min_node = i;
        }
    }
    if (min_node != -1) {
        printf("Shortest edge from node %d: to node %d (weight: %d)\n", start, min_node, min_weight);
    } else {
        printf("No edges from node %d\n", start);
    }
}

void count_edges(Graph *g, int start, int unused) {
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
    int nodes, edges;
    
    printf("Enter number of nodes (1-100): ");
    if (scanf("%d", &nodes) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    initialize_graph(&g, nodes);
    
    printf("Enter number of edges: ");
    if (scanf("%d", &edges) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    if (edges < 0 || edges > nodes * nodes) {
        fprintf(stderr, "Invalid edge count\n");
        return 1;
    }
    
    for (int i = 0; i < edges; i++) {
        int from, to, weight;
        printf("Enter edge %d (from to weight): ", i + 1);
        if (scanf("%d %d %d", &from, &to, &weight) != 3) {
            fprintf(stderr, "Invalid edge input\n");
            return 1;
        }
        add_edge(&g, from, to, weight);
    }
    
    GraphOperation operations[] = {print_adjacency, find_shortest_edge, count_edges};
    int op_count = sizeof(operations) / sizeof(operations[0]);
    
    int choice, node;
    printf("\nChoose operation (0=print adjacency, 1=find shortest edge, 2=count edges): ");
    if (scanf("%d", &choice) != 1) {
        fprintf(stderr, "Invalid choice\n");
        return 1;
    }
    
    if (choice < 0 || choice >= op_count) {
        fprintf(stderr, "Invalid operation choice\n");
        return 1;
    }
    
    printf("Enter node to operate on: ");
    if (scanf("%d", &node) != 1) {
        fprintf(stderr, "Invalid node\n");
        return 1