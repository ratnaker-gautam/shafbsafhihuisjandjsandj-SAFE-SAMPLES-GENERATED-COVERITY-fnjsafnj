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
    if (nodes <= 0 || nodes > MAX_NODES) {
        nodes = MAX_NODES;
    }
    g->nodes = nodes;
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            if (i == j) {
                g->adj[i][j] = 0;
            } else {
                g->adj[i][j] = INF;
            }
        }
    }
}

void add_edge(struct Graph *g, int u, int v, int weight) {
    if (u < 0 || u >= g->nodes || v < 0 || v >= g->nodes) {
        return;
    }
    if (weight < 0) {
        return;
    }
    g->adj[u][v] = weight;
    g->adj[v][u] = weight;
}

void dijkstra(struct Graph *g, int src) {
    int dist[MAX_NODES];
    int visited[MAX_NODES];
    
    for (int i = 0; i < g->nodes; i++) {
        dist[i] = INF;
        visited[i] = 0;
    }
    dist[src] = 0;
    
    int count = 0;
    while (count < g->nodes) {
        int min_dist = INF;
        int u = -1;
        
        int i = 0;
        while (i < g->nodes) {
            if (!visited[i] && dist[i] < min_dist) {
                min_dist = dist[i];
                u = i;
            }
            i++;
        }
        
        if (u == -1) {
            break;
        }
        
        visited[u] = 1;
        count++;
        
        int v = 0;
        do {
            if (!visited[v] && g->adj[u][v] != INF && dist[u] != INF) {
                long new_dist = (long)dist[u] + g->adj[u][v];
                if (new_dist < dist[v] && new_dist < INF) {
                    dist[v] = (int)new_dist;
                }
            }
            v++;
        } while (v < g->nodes);
    }
    
    printf("Shortest distances from node %d:\n", src);
    for (int i = 0; i < g->nodes; i++) {
        if (dist[i] == INF) {
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
        printf("Invalid number of nodes\n");
        return 1;
    }
    
    init_graph(&g, nodes);
    
    printf("Enter number of edges: ");
    if (scanf("%d", &edges) != 1 || edges < 0) {
        printf("Invalid number of edges\n");
        return 1;
    }
    
    printf("Enter edges (u v weight):\n");
    for (int i = 0; i < edges; i++) {
        int u, v, weight;
        if (scanf("%d %d %d", &u, &v, &weight) != 3) {
            printf("Invalid edge input\n");
            return 1;
        }
        if (u < 0 || u >= nodes || v < 0 || v >= nodes || weight < 0) {
            printf("Invalid edge parameters\n");
            return 1;
        }
        add_edge(&g, u, v, weight);
    }
    
    int src;
    printf("Enter source node: ");
    if (scanf("%d", &src) != 1 || src < 0 || src >= nodes) {
        printf("Invalid source node\n");
        return 1;
    }
    
    dijkstra(&g, src);
    
    return 0;
}