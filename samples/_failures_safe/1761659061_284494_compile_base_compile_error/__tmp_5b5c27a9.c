//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: graph_algorithms
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
        nodes = MAX_NODES;
    }
    g->nodes = nodes;
    g->edges = 0;
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            g->adj[i][j] = (i == j) ? 0 : INF;
        }
    }
}

int addEdge(struct Graph* g, int u, int v, int weight) {
    if (g == NULL || u < 0 || u >= g->nodes || v < 0 || v >= g->nodes) {
        return 0;
    }
    if (weight < 0) {
        return 0;
    }
    if (g->adj[u][v] == INF) {
        g->edges++;
    }
    g->adj[u][v] = weight;
    g->adj[v][u] = weight;
    return 1;
}

void dijkstra(struct Graph* g, int src, int dist[]) {
    if (g == NULL || dist == NULL || src < 0 || src >= g->nodes) {
        return;
    }
    
    int visited[MAX_NODES] = {0};
    
    for (int i = 0; i < g->nodes; i++) {
        dist[i] = INF;
    }
    dist[src] = 0;
    
    for (int count = 0; count < g->nodes - 1; count++) {
        int min_dist = INF;
        int min_node = -1;
        
        for (int v = 0; v < g->nodes; v++) {
            if (!visited[v] && dist[v] < min_dist) {
                min_dist = dist[v];
                min_node = v;
            }
        }
        
        if (min_node == -1) {
            break;
        }
        
        visited[min_node] = 1;
        
        for (int v = 0; v < g->nodes; v++) {
            if (!visited[v] && g->adj[min_node][v] != INF && dist[min_node] != INF) {
                long long new_dist = (long long)dist[min_node] + g->adj[min_node][v];
                if (new_dist < dist[v] && new_dist <= INT_MAX) {
                    dist[v] = (int)new_dist;
                }
            }
        }
    }
}

void printGraph(struct Graph* g) {
    if (g == NULL) {
        return;
    }
    printf("Graph with %d nodes and %d edges:\n", g->nodes, g->edges);
    for (int i = 0; i < g->nodes; i++) {
        printf("Node %d: ", i);
        int has_neighbors = 0;
        for (int j = 0; j < g->nodes; j++) {
            if (i != j && g->adj[i][j] != INF) {
                if (has_neighbors) {
                    printf(", ");
                }
                printf("%d(%d)", j, g->adj[i][j]);
                has_neighbors = 1;
            }
        }
        if (!has_neighbors) {
            printf("no connections");
        }
        printf("\n");
    }
}

int main() {
    struct Graph g;
    int nodes, edges;
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &nodes) != 1 || nodes < 1 || nodes > MAX_NODES) {
        printf("Invalid number of nodes. Using default: 5\n");
        nodes = 5;
    }
    
    initializeGraph(&g, nodes);
    
    printf("Enter number of edges: ");
    if (scanf("%d", &edges) != 1 || edges < 0) {
        printf("Invalid number of edges. Using default: 0\n");
        edges = 0;
    }
    
    printf("Enter edges as 'u v weight' (0-indexed):\n");
    for (int i = 0; i < edges; i++) {
        int u, v, weight;
        if (scanf("%d %d %d", &u, &v, &weight) != 3) {
            printf("Invalid edge format. Skipping.\n");
            while (getchar() != '\n');
            continue;
        }
        if (!addEdge(&g, u, v, weight)) {
            printf("Invalid edge (%d, %d, %d). Skipping.\n", u, v, weight);
        }
    }
    
    printf("\n");
    printGraph(&g);
    printf("\n");
    
    int src;
    printf("Enter source node for Dijkstra's algorithm: ");
    if (scanf("%d", &src) != 1 || src < 0 || src >= g.nodes) {
        printf("Invalid source node. Using node 0.\n");
        src = 0;
    }
    
    int dist[MAX_NODES];
    dijkstra(&g, src, dist);
    
    printf("\nShortest distances from node %d:\n", src);
    for (int i = 0; i < g.nodes; i