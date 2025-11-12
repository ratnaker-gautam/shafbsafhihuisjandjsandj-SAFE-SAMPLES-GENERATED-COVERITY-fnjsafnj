//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_NODES 100

struct Graph {
    int nodes;
    int edges;
    int adj[MAX_NODES][MAX_NODES];
};

void init_graph(struct Graph *g, int nodes) {
    if (g == NULL || nodes <= 0 || nodes > MAX_NODES) {
        return;
    }
    g->nodes = nodes;
    g->edges = 0;
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            *(*(g->adj + i) + j) = 0;
        }
    }
}

int add_edge(struct Graph *g, int u, int v) {
    if (g == NULL || u < 0 || u >= g->nodes || v < 0 || v >= g->nodes) {
        return 0;
    }
    if (*(*(g->adj + u) + v) == 0) {
        *(*(g->adj + u) + v) = 1;
        *(*(g->adj + v) + u) = 1;
        g->edges++;
        return 1;
    }
    return 0;
}

void dfs_util(struct Graph *g, int v, int *visited) {
    if (g == NULL || visited == NULL || v < 0 || v >= g->nodes) {
        return;
    }
    *(visited + v) = 1;
    printf("%d ", v);
    for (int i = 0; i < g->nodes; i++) {
        if (*(*(g->adj + v) + i) && !*(visited + i)) {
            dfs_util(g, i, visited);
        }
    }
}

void dfs(struct Graph *g, int start) {
    if (g == NULL || start < 0 || start >= g->nodes) {
        return;
    }
    int visited[MAX_NODES];
    for (int i = 0; i < g->nodes; i++) {
        *(visited + i) = 0;
    }
    printf("DFS traversal: ");
    dfs_util(g, start, visited);
    printf("\n");
}

void bfs(struct Graph *g, int start) {
    if (g == NULL || start < 0 || start >= g->nodes) {
        return;
    }
    int visited[MAX_NODES];
    for (int i = 0; i < g->nodes; i++) {
        *(visited + i) = 0;
    }
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    *(visited + start) = 1;
    *(queue + rear) = start;
    rear = (rear + 1) % MAX_NODES;
    printf("BFS traversal: ");
    while (front != rear) {
        int current = *(queue + front);
        front = (front + 1) % MAX_NODES;
        printf("%d ", current);
        for (int i = 0; i < g->nodes; i++) {
            if (*(*(g->adj + current) + i) && !*(visited + i)) {
                *(visited + i) = 1;
                *(queue + rear) = i;
                rear = (rear + 1) % MAX_NODES;
            }
        }
    }
    printf("\n");
}

int main() {
    struct Graph g;
    int nodes, edges;
    printf("Enter number of nodes (1-100): ");
    if (scanf("%d", &nodes) != 1 || nodes < 1 || nodes > MAX_NODES) {
        printf("Invalid number of nodes\n");
        return 1;
    }
    init_graph(&g, nodes);
    printf("Enter number of edges: ");
    if (scanf("%d", &edges) != 1 || edges < 0 || edges > nodes * (nodes - 1) / 2) {
        printf("Invalid number of edges\n");
        return 1;
    }
    printf("Enter edges (u v):\n");
    for (int i = 0; i < edges; i++) {
        int u, v;
        if (scanf("%d %d", &u, &v) != 2 || u < 0 || u >= nodes || v < 0 || v >= nodes) {
            printf("Invalid edge\n");
            return 1;
        }
        if (!add_edge(&g, u, v)) {
            printf("Failed to add edge\n");
            return 1;
        }
    }
    int start;
    printf("Enter starting node for traversals: ");
    if (scanf("%d", &start) != 1 || start < 0 || start >= nodes) {
        printf("Invalid start node\n");
        return 1;
    }
    dfs(&g, start);
    bfs(&g, start);
    return 0;
}