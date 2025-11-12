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

void dfsUtil(struct Graph* g, int v, int visited[]) {
    visited[v] = 1;
    printf("%d ", v);
    
    for (int i = 0; i < g->nodes; i++) {
        if (g->adj[v][i] != INF && g->adj[v][i] != 0 && !visited[i]) {
            dfsUtil(g, i, visited);
        }
    }
}

void dfs(struct Graph* g, int start) {
    if (start < 0 || start >= g->nodes) return;
    
    int visited[MAX_NODES] = {0};
    printf("DFS traversal from node %d: ", start);
    dfsUtil(g, start, visited);
    printf("\n");
}

void bfs(struct Graph* g, int start) {
    if (start < 0 || start >= g->nodes) return;
    
    int visited[MAX_NODES] = {0};
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    
    visited[start] = 1;
    queue[rear++] = start;
    
    printf("BFS traversal from node %d: ", start);
    
    while (front < rear) {
        int current = queue[front++];
        printf("%d ", current);
        
        for (int i = 0; i < g->nodes; i++) {
            if (g->adj[current][i] != INF && g->adj[current][i] != 0 && !visited[i]) {
                visited[i] = 1;
                if (rear < MAX_NODES) {
                    queue[rear++] = i;
                }
            }
        }
    }
    printf("\n");
}

int main() {
    struct Graph g;
    int nodes = 6;
    
    initializeGraph(&g, nodes);
    
    addEdge(&g, 0, 1, 4);
    addEdge(&g, 0, 2, 2);
    addEdge(&g, 1, 2, 1);
    addEdge(&g, 1, 3, 5);
    addEdge(&g, 2, 3, 8);
    addEdge(&g, 2, 4, 10);
    addEdge(&g, 3, 4, 2);
    addEdge(&g, 3, 5, 6);
    addEdge(&g, 4, 5, 3);
    
    printf("Graph algorithms demonstration:\n\n");
    
    dfs(&g, 0);
    bfs(&g, 0);
    printf("\n");
    floydWarshall(&g);
    
    return 0;
}