//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 100
#define INF INT_MAX

struct Graph {
    int nodes;
    int edges;
    int adj[MAX_NODES][MAX_NODES];
};

void initializeGraph(struct Graph* g, int nodes) {
    if (nodes <= 0 || nodes > MAX_NODES) {
        fprintf(stderr, "Invalid number of nodes\n");
        exit(1);
    }
    
    g->nodes = nodes;
    g->edges = 0;
    
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            g->adj[i][j] = (i == j) ? 0 : INF;
        }
    }
}

void addEdge(struct Graph* g, int u, int v, int weight) {
    if (u < 0 || u >= g->nodes || v < 0 || v >= g->nodes) {
        fprintf(stderr, "Invalid node indices\n");
        exit(1);
    }
    
    if (weight < 0) {
        fprintf(stderr, "Negative weights not allowed\n");
        exit(1);
    }
    
    g->adj[u][v] = weight;
    g->edges++;
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

void dfs(struct Graph* g, int start) {
    if (start < 0 || start >= g->nodes) {
        fprintf(stderr, "Invalid start node\n");
        return;
    }
    
    int visited[MAX_NODES] = {0};
    int stack[MAX_NODES];
    int top = -1;
    
    stack[++top] = start;
    visited[start] = 1;
    
    printf("DFS traversal: ");
    
    while (top >= 0) {
        int current = stack[top--];
        printf("%d ", current);
        
        for (int neighbor = g->nodes - 1; neighbor >= 0; neighbor--) {
            if (g->adj[current][neighbor] != INF && g->adj[current][neighbor] != 0 && !visited[neighbor]) {
                stack[++top] = neighbor;
                visited[neighbor] = 1;
            }
        }
    }
    printf("\n");
}

int main() {
    struct Graph g;
    int nodes = 5;
    
    initializeGraph(&g, nodes);
    
    addEdge(&g, 0, 1, 2);
    addEdge(&g, 0, 3, 6);
    addEdge(&g, 1, 2, 3);
    addEdge(&g, 1, 3, 8);
    addEdge(&g, 1, 4, 5);
    addEdge(&g, 2, 1, 1);
    addEdge(&g, 3, 2, 2);
    addEdge(&g, 4, 3, 1);
    
    printf("Graph with %d nodes and %d edges\n", g.nodes, g.edges);
    
    floydWarshall(&g);
    dfs(&g, 0);
    
    return 0;
}