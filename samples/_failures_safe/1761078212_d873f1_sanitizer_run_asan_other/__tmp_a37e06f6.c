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

void add_edge(Graph *g, int from, int to) {
    if (g == NULL || from < 0 || from >= g->node_count || to < 0 || to >= g->node_count) {
        return;
    }
    g->adjacency[from][to] = 1;
    g->adjacency[to][from] = 1;
}

void dfs_traversal(Graph *g, int start, int visited[]) {
    if (g == NULL || start < 0 || start >= g->node_count || visited == NULL) {
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
    if (g == NULL || start < 0 || start >= g->node_count || visited == NULL) {
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
                queue[rear++] = i;
            }
        }
    }
}

void perform_operation(Graph *g, GraphOperation op, int start, int visited[]) {
    if (g == NULL || op == NULL || start < 0 || start >= g->node_count || visited == NULL) {
        return;
    }
    op(g, start, visited);
}

void reset_visited(int visited[], int size) {
    if (visited == NULL || size <= 0 || size > MAX_NODES) {
        return;
    }
    for (int i = 0; i < size; i++) {
        visited[i] = 0;
    }
}

int main() {
    Graph g;
    int visited[MAX_NODES];
    initialize_graph(&g, 6);
    add_edge(&g, 0, 1);
    add_edge(&g, 0, 2);
    add_edge(&g, 1, 3);
    add_edge(&g, 2, 4);
    add_edge(&g, 3, 5);
    printf("DFS traversal: ");
    reset_visited(visited, g.node_count);
    perform_operation(&g, dfs_traversal, 0, visited);
    printf("\nBFS traversal: ");
    reset_visited(visited, g.node_count);
    perform_operation(&g, bfs_traversal, 0, visited);
    printf("\n");
    return 0;
}