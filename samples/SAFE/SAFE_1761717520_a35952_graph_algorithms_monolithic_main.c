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

void init_graph(struct Graph *g, int nodes) {
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

void add_edge(struct Graph *g, int u, int v, int weight) {
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

void dijkstra(struct Graph *g, int src, int dist[]) {
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
        
        if (min_node == -1) break;
        visited[min_node] = 1;
        
        for (int v = 0; v < g->nodes; v++) {
            if (!visited[v] && g->adj[min_node][v] != INF && dist[min_node] != INF) {
                long new_dist = (long)dist[min_node] + g->adj[min_node][v];
                if (new_dist < dist[v] && new_dist <= INT_MAX) {
                    dist[v] = (int)new_dist;
                }
            }
        }
    }
}

void print_graph(struct Graph *g) {
    printf("Graph adjacency matrix:\n");
    for (int i = 0; i < g->nodes; i++) {
        for (int j = 0; j < g->nodes; j++) {
            if (g->adj[i][j] == INF) {
                printf("INF ");
            } else {
                printf("%3d ", g->adj[i][j]);
            }
        }
        printf("\n");
    }
}

int main() {
    struct Graph g;
    int nodes, edges;
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &nodes) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    init_graph(&g, nodes);
    
    printf("Enter number of edges: ");
    if (scanf("%d", &edges) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    printf("Enter edges (u v weight):\n");
    for (int i = 0; i < edges; i++) {
        int u, v, weight;
        if (scanf("%d %d %d", &u, &v, &weight) != 3) {
            fprintf(stderr, "Invalid edge input\n");
            return 1;
        }
        add_edge(&g, u, v, weight);
    }
    
    print_graph(&g);
    
    int src;
    printf("Enter source node for Dijkstra: ");
    if (scanf("%d", &src) != 1) {
        fprintf(stderr, "Invalid source node\n");
        return 1;
    }
    
    if (src < 0 || src >= g.nodes) {
        fprintf(stderr, "Invalid source node\n");
        return 1;
    }
    
    int dist[MAX_NODES];
    dijkstra(&g, src, dist);
    
    printf("Shortest distances from node %d:\n", src);
    for (int i = 0; i < g.nodes; i++) {
        if (dist[i] == INF) {
            printf("Node %d: unreachable\n", i);
        } else {
            printf("Node %d: %d\n", i, dist[i]);
        }
    }
    
    return 0;
}