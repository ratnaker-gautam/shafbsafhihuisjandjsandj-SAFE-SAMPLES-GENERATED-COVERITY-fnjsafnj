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

void add_edge(Graph *g, int from, int to) {
    if (!validate_node(g, from) || !validate_node(g, to)) {
        return;
    }
    g->adjacency[from][to] = 1;
    g->adjacency[to][from] = 1;
}

void dfs_traversal(Graph *g, int start, int visited[]) {
    if (g == NULL || visited == NULL || !validate_node(g, start)) {
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

void bfs_traversal(Graph *g, int start, int visited[]) {
    if (g == NULL || visited == NULL || !validate_node(g, start)) {
        return;
    }
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    visited[start] = 1;
    queue[rear++] = start;
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
}

void perform_operation(Graph *g, int start, GraphOperation op) {
    if (g == NULL || op == NULL || !validate_node(g, start)) {
        return;
    }
    int visited[MAX_NODES] = {0};
    op(g, start, visited);
    printf("\n");
}

void print_graph(Graph *g, int unused1, int unused2) {
    if (g == NULL) {
        return;
    }
    printf("Graph adjacency matrix:\n");
    for (int i = 0; i < g->node_count; i++) {
        for (int j = 0; j < g->node_count; j++) {
            printf("%d ", g->adjacency[i][j]);
        }
        printf("\n");
    }
}

int main() {
    Graph g;
    initialize_graph(&g, 6);
    
    add_edge(&g, 0, 1);
    add_edge(&g, 0, 2);
    add_edge(&g, 1, 3);
    add_edge(&g, 2, 3);
    add_edge(&g, 3, 4);
    add_edge(&g, 4, 5);
    
    GraphOperation operations[] = {print_graph, dfs_traversal, bfs_traversal};
    int operation_count = sizeof(operations) / sizeof(operations[0]);
    
    for (int i = 0; i < operation_count; i++) {
        if (i == 0) {
            operations[i](&g, 0, 0);
        } else {
            printf("Starting from node 0: ");
            operations[i](&g, 0, 0);
        }
    }
    
    return 0;
}