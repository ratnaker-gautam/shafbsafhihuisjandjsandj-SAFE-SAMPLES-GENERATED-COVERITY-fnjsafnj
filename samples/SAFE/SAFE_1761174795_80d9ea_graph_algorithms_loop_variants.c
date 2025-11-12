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

void init_graph(struct Graph *g, int nodes) {
    g->nodes = nodes;
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            g->adj[i][j] = (i == j) ? 0 : INF;
        }
    }
}

void add_edge(struct Graph *g, int u, int v, int w) {
    if (u < 0 || u >= g->nodes || v < 0 || v >= g->nodes) return;
    if (w < 0) return;
    g->adj[u][v] = w;
    g->adj[v][u] = w;
}

void floyd_warshall(struct Graph *g) {
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
                if (dist[k][j] == INF) continue;
                if (dist[i][j] > dist[i][k] + dist[k][j]) {
                    if (dist[i][k] < INF - dist[k][j]) {
                        dist[i][j] = dist[i][k] + dist[k][j];
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

void dfs_util(struct Graph *g, int v, int visited[]) {
    visited[v] = 1;
    printf("%d ", v);
    for (int i = 0; i < g->nodes; i++) {
        if (g->adj[v][i] != INF && g->adj[v][i] != 0 && !visited[i]) {
            dfs_util(g, i, visited);
        }
    }
}

void dfs(struct Graph *g, int start) {
    if (start < 0 || start >= g->nodes) return;
    int visited[MAX_NODES] = {0};
    printf("DFS from node %d: ", start);
    dfs_util(g, start, visited);
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
    if (scanf("%d", &edges) != 1 || edges < 0 || edges > nodes * nodes) {
        printf("Invalid number of edges\n");
        return 1;
    }
    
    printf("Enter edges (u v weight):\n");
    for (int i = 0; i < edges; i++) {
        int u, v, w;
        if (scanf("%d %d %d", &u, &v, &w) != 3) {
            printf("Invalid edge input\n");
            return 1;
        }
        if (u < 0 || u >= nodes || v < 0 || v >= nodes || w < 0) {
            printf("Invalid edge parameters\n");
            return 1;
        }
        add_edge(&g, u, v, w);
    }
    
    printf("\nGraph algorithms:\n");
    printf("1. Floyd-Warshall (all pairs shortest path)\n");
    printf("2. DFS traversal\n");
    
    int choice;
    printf("Enter choice (1 or 2): ");
    if (scanf("%d", &choice) != 1) {
        printf("Invalid choice\n");
        return 1;
    }
    
    switch (choice) {
        case 1:
            floyd_warshall(&g);
            break;
        case 2:
            printf("Enter start node for DFS: ");
            int start;
            if (scanf("%d", &start) != 1 || start < 0 || start >= nodes) {
                printf("Invalid start node\n");
                return 1;
            }
            dfs(&g, start);
            break;
        default:
            printf("Invalid choice\n");
            return 1;
    }
    
    return 0;
}