//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: graph_algorithms
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
    if (u < 0 || u >= g->nodes || v < 0 || v >= g->nodes) return;
    if (weight < 0) return;
    g->adj[u][v] = weight;
    g->adj[v][u] = weight;
}

int minDistance(int dist[], int visited[], int n) {
    int min = INF, min_index = -1;
    for (int v = 0; v < n; v++) {
        if (visited[v] == 0 && dist[v] <= min) {
            min = dist[v];
            min_index = v;
        }
    }
    return min_index;
}

void dijkstra(struct Graph* g, int src) {
    int dist[MAX_NODES];
    int visited[MAX_NODES];
    
    for (int i = 0; i < g->nodes; i++) {
        dist[i] = INF;
        visited[i] = 0;
    }
    dist[src] = 0;
    
    for (int count = 0; count < g->nodes - 1; count++) {
        int u = minDistance(dist, visited, g->nodes);
        if (u == -1) break;
        visited[u] = 1;
        
        for (int v = 0; v < g->nodes; v++) {
            if (!visited[v] && g->adj[u][v] != INF && dist[u] != INF) {
                long sum = (long)dist[u] + g->adj[u][v];
                if (sum < dist[v] && sum <= INT_MAX) {
                    dist[v] = (int)sum;
                }
            }
        }
    }
    
    printf("Vertex Distance from Source %d:\n", src);
    for (int i = 0; i < g->nodes; i++) {
        if (dist[i] == INF) {
            printf("%d \t INF\n", i);
        } else {
            printf("%d \t %d\n", i, dist[i]);
        }
    }
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
    if (scanf("%d", &edges) != 1 || edges < 0) {
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
        if (u < 0 || u >= nodes || v < 0 || v >= nodes) {
            printf("Invalid node index\n");
            return 1;
        }
        if (w < 0) {
            printf("Negative weights not allowed\n");
            return 1;
        }
        addEdge(&g, u, v, w);
    }
    
    int source;
    printf("Enter source vertex: ");
    if (scanf("%d", &source) != 1 || source < 0 || source >= nodes) {
        printf("Invalid source vertex\n");
        return 1;
    }
    
    dijkstra(&g, source);
    
    return 0;
}