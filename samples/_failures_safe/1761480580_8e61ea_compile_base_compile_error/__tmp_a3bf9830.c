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

void find_neighbors(Graph *g, int node, int threshold) {
    if (node < 0 || node >= g->node_count) {
        fprintf(stderr, "Invalid node\n");
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

void calculate_out_degree(Graph *g, int node, int unused) {
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
    printf("Out-degree of node %d: %d\n", node, degree);
}

void execute_operation(Graph *g, GraphOperation op, int param1, int param2) {
    if (g == NULL || op == NULL) {
        fprintf(stderr, "Invalid graph or operation\n");
        return;
    }
    op(g, param1, param2);
}

int main() {
    Graph g;
    int nodes, edges;
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &nodes) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    initialize_graph(&g, nodes);
    
    printf("Enter number of edges: ");
    if (scanf("%d", &edges) != 1 || edges < 0) {
        fprintf(stderr, "Invalid edge count\n");
        return 1;
    }
    
    printf("Enter edges (from to weight):\n");
    for (int i = 0; i < edges; i++) {
        int from, to, weight;
        if (scanf("%d %d %d", &from, &to, &weight) != 3) {
            fprintf(stderr, "Invalid edge data\n");
            return 1;
        }
        add_edge(&g, from, to, weight);
    }
    
    GraphOperation operations[] = {
        print_adjacency,
        find_neighbors,
        calculate_out_degree
    };
    const char* operation_names[] = {
        "Print adjacency list",
        "Find neighbors with minimum weight",
        "Calculate out-degree"
    };
    int op_count = sizeof(operations) / sizeof(operations[0]);
    
    while (1) {
        printf("\nAvailable operations:\n");
        for (int i = 0; i < op_count; i++) {
            printf("%d. %s\n", i, operation_names[i]);
        }
        printf("%d. Exit\n", op_count);
        
        int choice;
        printf("Select operation: ");
        if (scanf("%d", &choice) != 1) {
            fprintf(stderr, "Invalid input\n");
            return 1;
        }
        
        if (choice == op_count) {
            break;
        }
        
        if (choice < 0 || choice >= op_count) {
            fprintf(stderr, "Invalid operation\n");
            continue;
        }
        
        int param1, param2 = 0;
        printf("Enter node index: ");