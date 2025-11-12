//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: graph_algorithms
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
    if (nodes <= 0 || nodes > MAX_NODES) {
        fprintf(stderr, "Invalid number of nodes\n");
        exit(1);
    }
    g->nodes = nodes;
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            g->adj[i][j] = (i == j) ? 0 : INF;
        }
    }
}

void addEdge(struct Graph* g, int u, int v, int weight) {
    if (u < 0 || u >= g->nodes || v < 0 || v >= g->nodes) {
        fprintf(stderr, "Invalid node index\n");
        exit(1);
    }
    if (weight < 0) {
        fprintf(stderr, "Negative weights not allowed\n");
        exit(1);
    }
    g->adj[u][v] = weight;
    g->adj[v][u] = weight;
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
            if (dist[i][k] == INF) continue;
            for (int j = 0; j < g->nodes; j++) {
                if (dist[k][j] != INF && dist[i][k] + dist[k][j] < dist[i][j]) {
                    if (dist[i][k] > INF - dist[k][j]) {
                        continue;
                    }
                    dist[i][j] = dist[i][k] + dist[k][j];
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

void dfsUtil(struct Graph* g, int node, int visited[]) {
    visited[node] = 1;
    printf("%d ", node);
    for (int i = 0; i < g->nodes; i++) {
        if (g->adj[node][i] != INF && !visited[i]) {
            dfsUtil(g, i, visited);
        }
    }
}

void dfs(struct Graph* g, int start) {
    if (start < 0 || start >= g->nodes) {
        fprintf(stderr, "Invalid start node\n");
        return;
    }
    int visited[MAX_NODES] = {0};
    printf("DFS traversal: ");
    dfsUtil(g, start, visited);
    printf("\n");
}

void bfs(struct Graph* g, int start) {
    if (start < 0 || start >= g->nodes) {
        fprintf(stderr, "Invalid start node\n");
        return;
    }
    int visited[MAX_NODES] = {0};
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    visited[start] = 1;
    queue[rear++] = start;
    printf("BFS traversal: ");
    while (front < rear) {
        int node = queue[front++];
        printf("%d ", node);
        for (int i = 0; i < g->nodes; i++) {
            if (g->adj[node][i] != INF && !visited[i]) {
                visited[i] = 1;
                if (rear >= MAX_NODES) {
                    fprintf(stderr, "Queue overflow\n");
                    return;
                }
                queue[rear++] = i;
            }
        }
    }
    printf("\n");
}

int main() {
    struct Graph g;
    int nodes = 5;
    initializeGraph(&g, nodes);
    addEdge(&g, 0, 1, 10);
    addEdge(&g, 0, 2, 5);
    addEdge(&g, 1, 2, 2);
    addEdge(&g, 1, 3, 1);
    addEdge(&g, 2, 3, 9);
    addEdge(&g, 2, 4, 2);
    addEdge(&g, 3, 4, 4);
    addEdge(&g, 4, 0, 7);

    printf("Graph with %d nodes\n", nodes);
    floydWarshall(&g);
    dfs(&g, 0);
    bfs(&g, 0);

    return 0;
}