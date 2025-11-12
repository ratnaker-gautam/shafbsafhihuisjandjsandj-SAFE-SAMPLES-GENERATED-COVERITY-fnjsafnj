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

void dfs_traversal(Graph *g, int start, int visited[]) {
    if (g == NULL || !validate_node(g, start) || visited == NULL) {
        return;
    }
    visited[start] = 1;
    printf("%d ", start);
    for (int i = 0; i < g->node_count; i++) {
        if (g->adjacency[start][i] && !visited[i]) {
            dfs_traversal(g, i, visited);
        }
    }
}

void dfs_operation(Graph *g, int start, int unused) {
    if (g == NULL || !validate_node(g, start)) {
        return;
    }
    int visited[MAX_NODES] = {0};
    printf("DFS traversal from node %d: ", start);
    dfs_traversal(g, start, visited);
    printf("\n");
}

void bfs_operation(Graph *g, int start, int unused) {
    if (g == NULL || !validate_node(g, start)) {
        return;
    }
    int visited[MAX_NODES] = {0};
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    
    visited[start] = 1;
    queue[rear++] = start;
    
    printf("BFS traversal from node %d: ", start);
    while (front < rear) {
        int current = queue[front++];
        printf("%d ", current);
        
        for (int i = 0; i < g->node_count; i++) {
            if (g->adjacency[current][i] && !visited[i]) {
                visited[i] = 1;
                if (rear < MAX_NODES) {
                    queue[rear++] = i;
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
    int node_count = 6;
    
    initialize_graph(&graph, node_count);
    
    GraphOperation operations[] = {
        add_edge_operation,
        remove_edge_operation,
        dfs_operation,
        bfs_operation
    };
    
    execute_graph_operation(&graph, operations[0], 0, 1);
    execute_graph_operation(&graph, operations[0], 0, 2);
    execute_graph_operation(&graph, operations[0], 1, 3);
    execute_graph_operation(&graph, operations[0], 2, 4);
    execute_graph_operation(&graph, operations[0], 3, 5);
    execute_graph_operation(&graph, operations[0], 4, 5);
    
    execute_graph_operation(&graph, operations[2], 0, 0);
    execute_graph_operation(&graph, operations[3], 0, 0);
    
    execute_graph_operation(&graph, operations[1], 2, 4);
    
    execute_graph_operation(&graph, operations[2], 0, 0);
    
    return 0;
}