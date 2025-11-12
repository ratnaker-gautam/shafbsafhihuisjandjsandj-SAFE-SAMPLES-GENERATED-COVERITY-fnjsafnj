//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_NODES 100

struct Graph {
    int nodes;
    int edges;
    int adjacency[MAX_NODES][MAX_NODES];
};

void initialize_graph(struct Graph *g, int nodes) {
    if (g == NULL || nodes <= 0 || nodes > MAX_NODES) return;
    g->nodes = nodes;
    g->edges = 0;
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            g->adjacency[i][j] = 0;
        }
    }
}

int add_edge(struct Graph *g, int from, int to, int weight) {
    if (g == NULL || from < 0 || from >= g->nodes || to < 0 || to >= g->nodes) return 0;
    if (weight < 0 || weight > 1000) return 0;
    g->adjacency[from][to] = weight;
    g->edges++;
    return 1;
}

void print_graph(struct Graph *g) {
    if (g == NULL) return;
    printf("Graph with %d nodes and %d edges:\n", g->nodes, g->edges);
    for (int i = 0; i < g->nodes; i++) {
        printf("Node %d: ", i);
        for (int j = 0; j < g->nodes; j++) {
            if (g->adjacency[i][j] != 0) {
                printf("%d(%d) ", j, g->adjacency[i][j]);
            }
        }
        printf("\n");
    }
}

void dfs_util(struct Graph *g, int node, int visited[], void (*visit)(int)) {
    if (g == NULL || visited == NULL || visit == NULL) return;
    if (node < 0 || node >= g->nodes) return;
    if (visited[node]) return;
    visited[node] = 1;
    visit(node);
    for (int i = 0; i < g->nodes; i++) {
        if (g->adjacency[node][i] != 0 && !visited[i]) {
            dfs_util(g, i, visited, visit);
        }
    }
}

void dfs(struct Graph *g, int start, void (*visit)(int)) {
    if (g == NULL || visit == NULL) return;
    if (start < 0 || start >= g->nodes) return;
    int visited[MAX_NODES];
    for (int i = 0; i < g->nodes; i++) visited[i] = 0;
    dfs_util(g, start, visited, visit);
}

void bfs(struct Graph *g, int start, void (*visit)(int)) {
    if (g == NULL || visit == NULL) return;
    if (start < 0 || start >= g->nodes) return;
    int visited[MAX_NODES];
    for (int i = 0; i < g->nodes; i++) visited[i] = 0;
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    visited[start] = 1;
    queue[rear++] = start;
    while (front < rear) {
        int current = queue[front++];
        visit(current);
        for (int i = 0; i < g->nodes; i++) {
            if (g->adjacency[current][i] != 0 && !visited[i]) {
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

void process_graph(struct Graph *g, void (*traversal)(struct Graph*, int, void (*)(int)), void (*operation)(int)) {
    if (g == NULL || traversal == NULL || operation == NULL) return;
    printf("Traversal result: ");
    traversal(g, 0, operation);
    printf("\n");
}

int main() {
    struct Graph g;
    initialize_graph(&g, 6);
    add_edge(&g, 0, 1, 5);
    add_edge(&g, 0, 2, 3);
    add_edge(&g, 1, 3, 2);
    add_edge(&g, 2, 3, 7);
    add_edge(&g, 3, 4, 1);
    add_edge(&g, 4, 5, 4);
    add_edge(&g, 1, 4, 6);
    
    print_graph(&g);
    
    printf("DFS with print: ");
    process_graph(&g, dfs, print_node);
    
    printf("BFS with print: ");
    process_graph(&g, bfs, print_node);
    
    printf("DFS with square: ");
    process_graph(&g, dfs, square_node);
    
    printf("BFS with square: ");
    process_graph(&g, bfs, square_node);
    
    return 0;
}