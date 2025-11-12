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
    if (g == NULL || node_count <= 0 || node_count > MAX_NODES) {
        return;
    }
    g->node_count = node_count;
    for (int i = 0; i < node_count; i++) {
        for (int j = 0; j < node_count; j++) {
            g->adjacency[i][j] = 0;
        }
    }
}

int validate_node(Graph *g, int node) {
    return (g != NULL && node >= 0 && node < g->node_count);
}

void add_edge_operation(Graph *g, int from, int to) {
    if (!validate_node(g, from) || !validate_node(g, to)) {
        return;
    }
    g->adjacency[from][to] = 1;
    g->adjacency[to][from] = 1;
}

void remove_edge_operation(Graph *g, int from, int to) {
    if (!validate_node(g, from) || !validate_node(g, to)) {
        return;
    }
    g->adjacency[from][to] = 0;
    g->adjacency[to][from] = 0;
}

void print_adjacency_operation(Graph *g, int from, int to) {
    if (!validate_node(g, from)) {
        return;
    }
    printf("Node %d adjacency: ", from);
    for (int i = 0; i < g->node_count; i++) {
        if (g->adjacency[from][i] == 1) {
            printf("%d ", i);
        }
    }
    printf("\n");
}

void dfs_traversal_operation(Graph *g, int start, int unused) {
    if (!validate_node(g, start)) {
        return;
    }
    int visited[MAX_NODES] = {0};
    int stack[MAX_NODES];
    int top = 0;
    stack[top++] = start;
    visited[start] = 1;
    
    printf("DFS traversal: ");
    while (top > 0) {
        int current = stack[--top];
        printf("%d ", current);
        
        for (int i = g->node_count - 1; i >= 0; i--) {
            if (g->adjacency[current][i] == 1 && !visited[i]) {
                if (top < MAX_NODES - 1) {
                    stack[top++] = i;
                    visited[i] = 1;
                }
            }
        }
    }
    printf("\n");
}

void execute_graph_operation(Graph *g, GraphOperation op, int param1, int param2) {
    if (g == NULL || op == NULL) {
        return;
    }
    op(g, param1, param2);
}

int main() {
    Graph graph;
    initialize_graph(&graph, 6);
    
    GraphOperation operations[] = {
        add_edge_operation,
        add_edge_operation,
        add_edge_operation,
        add_edge_operation,
        add_edge_operation,
        print_adjacency_operation,
        dfs_traversal_operation,
        remove_edge_operation,
        print_adjacency_operation
    };
    
    int operation_params[][2] = {
        {0, 1}, {0, 2}, {1, 3}, {2, 4}, {3, 5}, {0, 0}, {0, 0}, {1, 3}, {1, 0}
    };
    
    int operation_count = sizeof(operations) / sizeof(operations[0]);
    
    for (int i = 0; i < operation_count; i++) {
        execute_graph_operation(&graph, operations[i], 
                              operation_params[i][0], 
                              operation_params[i][1]);
    }
    
    return 0;
}