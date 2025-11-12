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

void init_graph(struct Graph *g, int nodes) {
    if (nodes <= 0 || nodes > MAX_NODES) {
        nodes = MAX_NODES;
    }
    g->nodes = nodes;
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            g->adj_matrix[i][j] = 0;
        }
    }
}

int add_edge(struct Graph *g, int from, int to, int weight) {
    if (from < 0 || from >= g->nodes || to < 0 || to >= g->nodes) {
        return 0;
    }
    if (weight < 0 || weight > 1000) {
        return 0;
    }
    g->adj_matrix[from][to] = weight;
    g->adj_matrix[to][from] = weight;
    return 1;
}

void dfs_util(struct Graph *g, int node, int visited[], void (*visit)(int)) {
    if (node < 0 || node >= g->nodes || visited[node]) {
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
    if (start < 0 || start >= g->nodes || visit == NULL) {
        return;
    }
    int visited[MAX_NODES] = {0};
    dfs_util(g, start, visited, visit);
}

void bfs(struct Graph *g, int start, void (*visit)(int)) {
    if (start < 0 || start >= g->nodes || visit == NULL) {
        return;
    }
    int visited[MAX_NODES] = {0};
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

void square_node(int node) {
    printf("%d ", node * node);
}

void process_graph(struct Graph *g, int start, void (*traversal)(struct Graph*, int, void (*)(int)), void (*operation)(int)) {
    if (g == NULL || traversal == NULL || operation == NULL) {
        return;
    }
    traversal(g, start, operation);
    printf("\n");
}

int main() {
    struct Graph g;
    init_graph(&g, 6);
    add_edge(&g, 0, 1, 5);
    add_edge(&g, 0, 2, 3);
    add_edge(&g, 1, 3, 2);
    add_edge(&g, 2, 4, 1);
    add_edge(&g, 3, 5, 4);
    add_edge(&g, 4, 5, 6);
    
    printf("DFS traversal: ");
    process_graph(&g, 0, dfs, print_node);
    
    printf("BFS traversal: ");
    process_graph(&g, 0, bfs, print_node);
    
    printf("DFS with squares: ");
    process_graph(&g, 0, dfs, square_node);
    
    printf("BFS with squares: ");
    process_graph(&g, 0, bfs, square_node);
    
    return 0;
}