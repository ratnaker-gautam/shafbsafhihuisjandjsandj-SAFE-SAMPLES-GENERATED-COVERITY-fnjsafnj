//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 100
#define INF INT_MAX

struct Graph {
    int nodes;
    int adj[MAX_NODES][MAX_NODES];
};

void initializeGraph(struct Graph* g, int nodes) {
    g->nodes = nodes;
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            g->adj[i][j] = (i == j) ? 0 : INF;
        }
    }
}

void addEdge(struct Graph* g, int u, int v, int weight) {
    if (u >= 0 && u < g->nodes && v >= 0 && v < g->nodes && weight >= 0) {
        g->adj[u][v] = weight;
        g->adj[v][u] = weight;
    }
}

void floydWarshall(struct Graph* g) {
    int dist[MAX_NODES][MAX_NODES];
    for (int i = 0; i < g->nodes; i++) {
        for (int j = 0; j < g->nodes; j++) {
            dist[i][j] = g->adj[i][j];
        }
    }

    for (int k = 0; k < g->nodes; k++) {
        for (int i = 0; i < g->nodes; i++) {
            if (dist[i][k] != INF) {
                for (int j = 0; j < g->nodes; j++) {
                    if (dist[k][j] != INF && dist[i][k] + dist[k][j] < dist[i][j]) {
                        if (dist[i][k] <= INF - dist[k][j]) {
                            dist[i][j] = dist[i][k] + dist[k][j];
                        }
                    }
                }
            }
        }
    }

    printf("Shortest path distances:\n");
    for (int i = 0; i < g->nodes; i++) {
        for (int j = 0; j < g->nodes; j++) {
            if (dist[i][j] == INF) {
                printf("INF ");
            } else {
                printf("%3d ", dist[i][j]);
            }
        }
        printf("\n");
    }
}

void dfs(struct Graph* g, int start) {
    int visited[MAX_NODES] = {0};
    int stack[MAX_NODES];
    int top = -1;

    stack[++top] = start;
    visited[start] = 1;

    printf("DFS traversal: ");
    while (top >= 0) {
        int current = stack[top--];
        printf("%d ", current);

        for (int i = g->nodes - 1; i >= 0; i--) {
            if (g->adj[current][i] != INF && !visited[i]) {
                visited[i] = 1;
                stack[++top] = i;
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

    initializeGraph(&g, nodes);

    printf("Enter number of edges: ");
    if (scanf("%d", &edges) != 1 || edges < 0 || edges > nodes * nodes) {
        printf("Invalid number of edges\n");
        return 1;
    }

    for (int i = 0; i < edges; i++) {
        int u, v, w;
        printf("Enter edge %d (u v weight): ", i + 1);
        if (scanf("%d %d %d", &u, &v, &w) != 3 || u < 0 || u >= nodes || v < 0 || v >= nodes || w < 0) {
            printf("Invalid edge data\n");
            return 1;
        }
        addEdge(&g, u, v, w);
    }

    int startNode;
    printf("Enter start node for DFS: ");
    if (scanf("%d", &startNode) != 1 || startNode < 0 || startNode >= nodes) {
        printf("Invalid start node\n");
        return 1;
    }

    floydWarshall(&g);
    dfs(&g, startNode);

    return 0;
}