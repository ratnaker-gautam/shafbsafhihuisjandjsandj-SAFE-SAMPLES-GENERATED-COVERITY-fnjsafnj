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

int validate_node(Graph *g, int node) {
    return (node >= 0 && node < g->node_count);
}

void add_edge_operation(Graph *g, int from, int to) {
    if (!validate_node(g, from) || !validate_node(g, to)) {
        fprintf(stderr, "Invalid node indices\n");
        return;
    }
    
    if (from == to) {
        fprintf(stderr, "Self-loops not allowed\n");
        return;
    }
    
    g->adjacency[from][to] = 1;
    g->adjacency[to][from] = 1;
}

void remove_edge_operation(Graph *g, int from, int to) {
    if (!validate_node(g, from) || !validate_node(g, to)) {
        fprintf(stderr, "Invalid node indices\n");
        return;
    }
    
    g->adjacency[from][to] = 0;
    g->adjacency[to][from] = 0;
}

void print_adjacency_operation(Graph *g, int from, int to) {
    if (!validate_node(g, from)) {
        fprintf(stderr, "Invalid node index\n");
        return;
    }
    
    printf("Node %d neighbors: ", from);
    for (int i = 0; i < g->node_count; i++) {
        if (g->adjacency[from][i] == 1) {
            printf("%d ", i);
        }
    }
    printf("\n");
}

void dfs_traversal_operation(Graph *g, int start, int unused) {
    if (!validate_node(g, start)) {
        fprintf(stderr, "Invalid start node\n");
        return;
    }
    
    int visited[MAX_NODES] = {0};
    int stack[MAX_NODES];
    int top = -1;
    
    stack[++top] = start;
    visited[start] = 1;
    
    printf("DFS traversal from node %d: ", start);
    
    while (top >= 0) {
        int current = stack[top--];
        printf("%d ", current);
        
        for (int i = g->node_count - 1; i >= 0; i--) {
            if (g->adjacency[current][i] == 1 && !visited[i]) {
                if (top + 1 >= MAX_NODES) {
                    fprintf(stderr, "Stack overflow\n");
                    return;
                }
                stack[++top] = i;
                visited[i] = 1;
            }
        }
    }
    printf("\n");
}

void execute_graph_operation(Graph *g, GraphOperation op, int param1, int param2) {
    if (g == NULL || op == NULL) {
        fprintf(stderr, "Invalid operation parameters\n");
        return;
    }
    op(g, param1, param2);
}

int main() {
    Graph graph;
    initialize_graph(&graph, 6);
    
    GraphOperation operations[] = {
        add_edge_operation,
        remove_edge_operation,
        print_adjacency_operation,
        dfs_traversal_operation
    };
    
    int operation_count = sizeof(operations) / sizeof(operations[0]);
    
    execute_graph_operation(&graph, operations[0], 0, 1);
    execute_graph_operation(&graph, operations[0], 0, 2);
    execute_graph_operation(&graph, operations[0], 1, 3);
    execute_graph_operation(&graph, operations[0], 2, 3);
    execute_graph_operation(&graph, operations[0], 3, 4);
    execute_graph_operation(&graph, operations[0], 4, 5);
    
    execute_graph_operation(&graph, operations[2], 0, 0);
    execute_graph_operation(&graph, operations[2], 3, 0);
    
    execute_graph_operation(&graph, operations[3], 0, 0);
    
    execute_graph_operation(&graph, operations[1], 2, 3);
    
    execute_graph_operation(&graph, operations[2], 2, 0);
    execute_graph_operation(&graph, operations[3], 0, 0);
    
    execute_graph_operation(&graph, operations[0], 10, 0);
    execute_graph_operation(&graph, operations[0], 0, 0);
    
    return EXIT_SUCCESS;
}