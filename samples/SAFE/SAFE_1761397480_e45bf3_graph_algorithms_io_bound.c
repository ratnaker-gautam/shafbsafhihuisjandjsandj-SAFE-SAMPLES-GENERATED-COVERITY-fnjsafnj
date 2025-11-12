//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_NODES 100
#define MAX_EDGES 500

struct Graph {
    int nodes;
    int edges;
    int adj[MAX_NODES][MAX_NODES];
};

void init_graph(struct Graph *g, int nodes) {
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

void add_edge(struct Graph *g, int u, int v, int weight) {
    if (u < 0 || u >= g->nodes || v < 0 || v >= g->nodes) {
        printf("Invalid node indices\n");
        return;
    }
    if (weight < 0) {
        printf("Negative weights not allowed\n");
        return;
    }
    if (g->edges >= MAX_EDGES) {
        printf("Too many edges\n");
        return;
    }
    g->adj[u][v] = weight;
    g->adj[v][u] = weight;
    g->edges++;
}

void dijkstra(struct Graph *g, int src) {
    if (src < 0 || src >= g->nodes) {
        printf("Invalid source node\n");
        return;
    }
    
    int dist[MAX_NODES];
    int visited[MAX_NODES];
    
    for (int i = 0; i < g->nodes; i++) {
        dist[i] = INT_MAX;
        visited[i] = 0;
    }
    dist[src] = 0;
    
    for (int count = 0; count < g->nodes - 1; count++) {
        int min_dist = INT_MAX;
        int min_node = -1;
        
        for (int v = 0; v < g->nodes; v++) {
            if (!visited[v] && dist[v] <= min_dist) {
                min_dist = dist[v];
                min_node = v;
            }
        }
        
        if (min_node == -1) break;
        visited[min_node] = 1;
        
        for (int v = 0; v < g->nodes; v++) {
            if (!visited[v] && g->adj[min_node][v] && 
                dist[min_node] != INT_MAX && 
                dist[min_node] + g->adj[min_node][v] < dist[v]) {
                dist[v] = dist[min_node] + g->adj[min_node][v];
            }
        }
    }
    
    printf("Shortest distances from node %d:\n", src);
    for (int i = 0; i < g->nodes; i++) {
        if (dist[i] == INT_MAX) {
            printf("Node %d: unreachable\n", i);
        } else {
            printf("Node %d: %d\n", i, dist[i]);
        }
    }
}

int main() {
    struct Graph g;
    int nodes, edges;
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &nodes) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    init_graph(&g, nodes);
    
    printf("Enter number of edges: ");
    if (scanf("%d", &edges) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (edges < 0 || edges > MAX_EDGES) {
        printf("Invalid number of edges\n");
        return 1;
    }
    
    printf("Enter edges as 'u v weight' (0-indexed):\n");
    for (int i = 0; i < edges; i++) {
        int u, v, weight;
        if (scanf("%d %d %d", &u, &v, &weight) != 3) {
            printf("Invalid edge input\n");
            return 1;
        }
        add_edge(&g, u, v, weight);
    }
    
    int source;
    printf("Enter source node for Dijkstra: ");
    if (scanf("%d", &source) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    dijkstra(&g, source);
    
    return 0;
}