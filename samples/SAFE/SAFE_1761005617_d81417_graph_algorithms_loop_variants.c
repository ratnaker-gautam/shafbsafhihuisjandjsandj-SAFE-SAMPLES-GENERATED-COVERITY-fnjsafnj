//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 100

struct Graph {
    int nodes;
    int adj[MAX_NODES][MAX_NODES];
};

void init_graph(struct Graph *g, int nodes) {
    if (nodes <= 0 || nodes > MAX_NODES) {
        fprintf(stderr, "Invalid number of nodes\n");
        exit(1);
    }
    g->nodes = nodes;
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            g->adj[i][j] = 0;
        }
    }
}

void add_edge(struct Graph *g, int u, int v, int weight) {
    if (u < 0 || u >= g->nodes || v < 0 || v >= g->nodes) {
        fprintf(stderr, "Invalid edge\n");
        exit(1);
    }
    g->adj[u][v] = weight;
    g->adj[v][u] = weight;
}

void dijkstra(struct Graph *g, int src) {
    int dist[MAX_NODES];
    int visited[MAX_NODES];
    
    for (int i = 0; i < g->nodes; i++) {
        dist[i] = INT_MAX;
        visited[i] = 0;
    }
    dist[src] = 0;
    
    int count = 0;
    while (count < g->nodes) {
        int min_dist = INT_MAX;
        int u = -1;
        for (int i = 0; i < g->nodes; i++) {
            if (!visited[i] && dist[i] < min_dist) {
                min_dist = dist[i];
                u = i;
            }
        }
        
        if (u == -1) break;
        
        visited[u] = 1;
        count++;
        
        for (int v = 0; v < g->nodes; v++) {
            if (!visited[v] && g->adj[u][v] != 0) {
                int new_dist = dist[u] + g->adj[u][v];
                if (new_dist > 0 && new_dist < dist[v]) {
                    dist[v] = new_dist;
                }
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
    if (scanf("%d", &nodes) != 1 || nodes < 1 || nodes > MAX_NODES) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    init_graph(&g, nodes);
    
    printf("Enter number of edges: ");
    if (scanf("%d", &edges) != 1 || edges < 0) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    printf("Enter edges (u v weight):\n");
    for (int i = 0; i < edges; i++) {
        int u, v, w;
        if (scanf("%d %d %d", &u, &v, &w) != 3) {
            fprintf(stderr, "Invalid edge input\n");
            return 1;
        }
        if (u < 0 || u >= nodes || v < 0 || v >= nodes || w <= 0) {
            fprintf(stderr, "Invalid edge parameters\n");
            return 1;
        }
        add_edge(&g, u, v, w);
    }
    
    int src;
    printf("Enter source node: ");
    if (scanf("%d", &src) != 1 || src < 0 || src >= nodes) {
        fprintf(stderr, "Invalid source node\n");
        return 1;
    }
    
    dijkstra(&g, src);
    
    return 0;
}