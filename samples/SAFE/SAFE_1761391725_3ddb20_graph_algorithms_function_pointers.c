//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_NODES 100

struct Graph {
    int nodes;
    int adj_matrix[MAX_NODES][MAX_NODES];
};

void initialize_graph(struct Graph *g, int nodes) {
    if (g == NULL || nodes <= 0 || nodes > MAX_NODES) {
        return;
    }
    g->nodes = nodes;
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            g->adj_matrix[i][j] = 0;
        }
    }
}

void add_edge(struct Graph *g, int u, int v, int weight) {
    if (g == NULL || u < 0 || u >= g->nodes || v < 0 || v >= g->nodes) {
        return;
    }
    if (weight < 0 || weight > 1000) {
        return;
    }
    g->adj_matrix[u][v] = weight;
    g->adj_matrix[v][u] = weight;
}

void dfs_util(struct Graph *g, int node, int visited[], void (*visit)(int)) {
    if (g == NULL || visited == NULL || visit == NULL) {
        return;
    }
    if (node < 0 || node >= g->nodes) {
        return;
    }
    visited[node] = 1;
    visit(node);
    for (int i = 0; i < g->nodes; i++) {
        if (g->adj_matrix[node][i] != 0 && !visited[i]) {
            dfs_util(g, i, visited, visit);
        }
    }
}

void dfs(struct Graph *g, int start, void (*visit)(int)) {
    if (g == NULL || visit == NULL) {
        return;
    }
    if (start < 0 || start >= g->nodes) {
        return;
    }
    int visited[MAX_NODES];
    for (int i = 0; i < g->nodes; i++) {
        visited[i] = 0;
    }
    dfs_util(g, start, visited, visit);
}

void bfs(struct Graph *g, int start, void (*visit)(int)) {
    if (g == NULL || visit == NULL) {
        return;
    }
    if (start < 0 || start >= g->nodes) {
        return;
    }
    int visited[MAX_NODES];
    for (int i = 0; i < g->nodes; i++) {
        visited[i] = 0;
    }
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    visited[start] = 1;
    queue[rear++] = start;
    while (front < rear) {
        int current = queue[front++];
        visit(current);
        for (int i = 0; i < g->nodes; i++) {
            if (g->adj_matrix[current][i] != 0 && !visited[i]) {
                visited[i] = 1;
                if (rear < MAX_NODES) {
                    queue[rear++] = i;
                }
            }
        }
    }
}

void print_node(int node) {
    printf("%d ", node);
}

void process_graph(struct Graph *g, void (*traversal)(struct Graph *, int, void (*)(int))) {
    if (g == NULL || traversal == NULL) {
        return;
    }
    printf("Traversal: ");
    traversal(g, 0, print_node);
    printf("\n");
}

int main() {
    struct Graph g;
    initialize_graph(&g, 6);
    add_edge(&g, 0, 1, 2);
    add_edge(&g, 0, 2, 3);
    add_edge(&g, 1, 3, 1);
    add_edge(&g, 2, 3, 4);
    add_edge(&g, 3, 4, 5);
    add_edge(&g, 4, 5, 2);
    printf("DFS ");
    process_graph(&g, dfs);
    printf("BFS ");
    process_graph(&g, bfs);
    return 0;
}