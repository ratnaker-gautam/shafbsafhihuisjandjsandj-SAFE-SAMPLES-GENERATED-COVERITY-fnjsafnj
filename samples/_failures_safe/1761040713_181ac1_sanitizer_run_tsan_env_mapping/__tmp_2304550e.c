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

void init_graph(struct Graph* g, int nodes) {
    g->nodes = nodes;
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            g->adj[i][j] = (i == j) ? 0 : INF;
        }
    }
}

void add_edge(struct Graph* g, int u, int v, int weight) {
    if (u >= 0 && u < g->nodes && v >= 0 && v < g->nodes && weight >= 0) {
        g->adj[u][v] = weight;
        g->adj[v][u] = weight;
    }
}

void dijkstra(struct Graph* g, int src, int dist[]) {
    int visited[MAX_NODES] = {0};
    
    for (int i = 0; i < g->nodes; i++) {
        dist[i] = INF;
    }
    dist[src] = 0;
    
    for (int count = 0; count < g->nodes; count++) {
        int min_dist = INF;
        int min_node = -1;
        
        for (int v = 0; v < g->nodes; v++) {
            if (!visited[v] && dist[v] < min_dist) {
                min_dist = dist[v];
                min_node = v;
            }
        }
        
        if (min_node == -1) break;
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

void print_paths(struct Graph* g, int src) {
    int dist[MAX_NODES];
    dijkstra(g, src, dist);
    
    printf("Shortest paths from node %d:\n", src);
    for (int i = 0; i < g->nodes; i++) {
        if (dist[i] == INF) {
            printf("Node %d: unreachable\n", i);
        } else {
            printf("Node %d: distance %d\n", i, dist[i]);
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
    if (scanf("%d", &edges) != 1 || edges < 0 || edges > nodes * (nodes - 1) / 2) {
        printf("Invalid number of edges\n");
        return 1;
    }
    
    for (int i = 0; i < edges; i++) {
        int u, v, w;
        printf("Enter edge %d (u v weight): ", i + 1);
        if (scanf("%d %d %d", &u, &v, &w) != 3 || u < 0 || u >= nodes || v < 0 || v >= nodes || w < 0) {
            printf("Invalid edge\n");
            return 1;
        }
        add_edge(&g, u, v, w);
    }
    
    int src;
    printf("Enter source node: ");
    if (scanf("%d", &src) != 1 || src < 0 || src >= nodes) {
        printf("Invalid source node\n");
        return 1;
    }
    
    print_paths(&g, src);
    
    return 0;
}