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
    if (validate_node(g, from) && validate_node(g, to)) {
        g->adjacency[from][to] = 1;
        g->adjacency[to][from] = 1;
    }
}

void remove_edge_operation(Graph *g, int from, int to) {
    if (validate_node(g, from) && validate_node(g, to)) {
        g->adjacency[from][to] = 0;
        g->adjacency[to][from] = 0;
    }
}

void print_adjacency_operation(Graph *g, int from, int to) {
    if (g == NULL) {
        return;
    }
    printf("Adjacency matrix:\n");
    for (int i = 0; i < g->node_count; i++) {
        for (int j = 0; j < g->node_count; j++) {
            printf("%d ", g->adjacency[i][j]);
        }
        printf("\n");
    }
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
            if (g->adjacency[current][i] && !visited[i]) {
                stack[top++] = i;
                visited[i] = 1;
            }
        }
    }
    printf("\n");
}

void execute_operation(Graph *g, GraphOperation op, int param1, int param2) {
    if (g != NULL && op != NULL) {
        op(g, param1, param2);
    }
}

int main() {
    Graph graph;
    int node_count;
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &node_count) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (node_count <= 0 || node_count > MAX_NODES) {
        printf("Invalid node count\n");
        return 1;
    }
    
    initialize_graph(&graph, node_count);
    
    GraphOperation operations[] = {
        add_edge_operation,
        remove_edge_operation,
        print_adjacency_operation,
        dfs_traversal_operation
    };
    
    const char* operation_names[] = {
        "Add edge",
        "Remove edge",
        "Print adjacency matrix",
        "DFS traversal"
    };
    
    int operation_count = sizeof(operations) / sizeof(operations[0]);
    
    while (1) {
        printf("\nAvailable operations:\n");
        for (int i = 0; i < operation_count; i++) {
            printf("%d. %s\n", i, operation_names[i]);
        }
        printf("%d. Exit\n", operation_count);
        printf("Select operation: ");
        
        int choice;
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input\n");
            while (getchar() != '\n');
            continue;
        }
        
        if (choice == operation_count) {
            break;
        }
        
        if (choice < 0 || choice >= operation_count) {
            printf("Invalid operation\n");
            continue;
        }
        
        int param1 = 0, param2 = 0;
        if (choice == 0 || choice == 1 || choice == 3) {
            printf("Enter first node: ");
            if (scanf("%d", &param1) != 1) {
                printf("Invalid input\n");
                while (getchar() != '\n');
                continue;
            }
            
            if (choice == 0 || choice == 1) {
                printf("Enter second node: ");
                if (scanf("%d", &param2) != 1) {
                    printf("Invalid input\n");
                    while (getchar() != '\n');
                    continue;
                }
            }
        }
        
        execute_operation(&graph, operations[choice], param1, param2);
    }
    
    return 0;
}