//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 100
#define MAX_EDGES 1000

struct Graph {
    int nodes;
    int edges;
    int adj[MAX_NODES][MAX_NODES];
};

void initializeGraph(struct Graph* g, int nodes) {
    if (nodes <= 0 || nodes > MAX_NODES) {
        printf("Invalid number of nodes\n");
        exit(1);
    }
    g->nodes = nodes;
    g->edges = 0;
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            g->adj[i][j] = 0;
        }
    }
}

void addEdge(struct Graph* g, int u, int v) {
    if (u < 0 || u >= g->nodes || v < 0 || v >= g->nodes) {
        printf("Invalid node indices\n");
        return;
    }
    if (g->edges >= MAX_EDGES) {
        printf("Maximum edges exceeded\n");
        return;
    }
    if (g->adj[u][v] == 0) {
        g->adj[u][v] = 1;
        g->adj[v][u] = 1;
        g->edges++;
    }
}

void dfs(struct Graph* g, int start, int visited[]) {
    if (start < 0 || start >= g->nodes) {
        return;
    }
    visited[start] = 1;
    printf("%d ", start);
    for (int i = 0; i < g->nodes; i++) {
        if (g->adj[start][i] == 1 && visited[i] == 0) {
            dfs(g, i, visited);
        }
    }
}

void bfs(struct Graph* g, int start) {
    if (start < 0 || start >= g->nodes) {
        printf("Invalid start node\n");
        return;
    }
    int visited[MAX_NODES] = {0};
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    visited[start] = 1;
    queue[rear++] = start;
    printf("BFS traversal: ");
    while (front < rear) {
        int current = queue[front++];
        printf("%d ", current);
        for (int i = 0; i < g->nodes; i++) {
            if (g->adj[current][i] == 1 && visited[i] == 0) {
                visited[i] = 1;
                if (rear < MAX_NODES) {
                    queue[rear++] = i;
                } else {
                    printf("Queue overflow\n");
                    return;
                }
            }
        }
    }
    printf("\n");
}

int main() {
    struct Graph g;
    int nodes, edges;
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &nodes) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    initializeGraph(&g, nodes);
    printf("Enter number of edges: ");
    if (scanf("%d", &edges) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    if (edges < 0 || edges > MAX_EDGES) {
        printf("Invalid number of edges\n");
        return 1;
    }
    printf("Enter edges (u v pairs):\n");
    for (int i = 0; i < edges; i++) {
        int u, v;
        if (scanf("%d %d", &u, &v) != 2) {
            printf("Invalid edge input\n");
            return 1;
        }
        addEdge(&g, u, v);
    }
    int start;
    printf("Enter start node for traversals: ");
    if (scanf("%d", &start) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    if (start < 0 || start >= nodes) {
        printf("Invalid start node\n");
        return 1;
    }
    printf("DFS traversal: ");
    int visited[MAX_NODES] = {0};
    dfs(&g, start, visited);
    printf("\n");
    bfs(&g, start);
    return 0;
}