//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 100

typedef struct {
    int edges[MAX_NODES][MAX_NODES];
    int num_nodes;
} Graph;

void init_graph(Graph *g, int n) {
    if (n <= 0 || n > MAX_NODES) n = MAX_NODES;
    g->num_nodes = n;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            g->edges[i][j] = 0;
        }
    }
}

void add_edge(Graph *g, int u, int v, int weight) {
    if (u < 0 || u >= g->num_nodes) return;
    if (v < 0 || v >= g->num_nodes) return;
    if (weight < 0) return;
    g->edges[u][v] = weight;
    g->edges[v][u] = weight;
}

void dijkstra(Graph *g, int src, int dist[]) {
    int visited[MAX_NODES] = {0};
    
    for (int i = 0; i < g->num_nodes; i++) {
        dist[i] = INT_MAX;
    }
    dist[src] = 0;
    
    for (int count = 0; count < g->num_nodes - 1; count++) {
        int min_dist = INT_MAX;
        int min_node = -1;
        
        for (int v = 0; v < g->num_nodes; v++) {
            if (!visited[v] && dist[v] <= min_dist) {
                min_dist = dist[v];
                min_node = v;
            }
        }
        
        if (min_node == -1) break;
        visited[min_node] = 1;
        
        for (int v = 0; v < g->num_nodes; v++) {
            if (!visited[v] && g->edges[min_node][v] != 0) {
                long new_dist = (long)dist[min_node] + g->edges[min_node][v];
                if (new_dist < dist[v] && new_dist <= INT_MAX) {
                    dist[v] = (int)new_dist;
                }
            }
        }
    }
}

void print_paths(Graph *g, int src, int dist[]) {
    printf("Shortest paths from node %d:\n", src);
    for (int i = 0; i < g->num_nodes; i++) {
        if (dist[i] == INT_MAX) {
            printf("Node %d: unreachable\n", i);
        } else {
            printf("Node %d: distance %d\n", i, dist[i]);
        }
    }
}

int main() {
    Graph g;
    int num_nodes, num_edges;
    int src;
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &num_nodes) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    if (num_nodes < 1 || num_nodes > MAX_NODES) {
        printf("Invalid number of nodes\n");
        return 1;
    }
    
    init_graph(&g, num_nodes);
    
    printf("Enter number of edges: ");
    if (scanf("%d", &num_edges) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    if (num_edges < 0 || num_edges > num_nodes * (num_nodes - 1) / 2) {
        printf("Invalid number of edges\n");
        return 1;
    }
    
    for (int i = 0; i < num_edges; i++) {
        int u, v, w;
        printf("Enter edge %d (u v weight): ", i + 1);
        if (scanf("%d %d %d", &u, &v, &w) != 3) {
            printf("Invalid input\n");
            return 1;
        }
        if (u < 0 || u >= num_nodes || v < 0 || v >= num_nodes) {
            printf("Invalid node indices\n");
            return 1;
        }
        if (w < 0) {
            printf("Negative weights not allowed\n");
            return 1;
        }
        add_edge(&g, u, v, w);
    }
    
    printf("Enter source node: ");
    if (scanf("%d", &src) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    if (src < 0 || src >= num_nodes) {
        printf("Invalid source node\n");
        return 1;
    }
    
    int dist[MAX_NODES];
    dijkstra(&g, src, dist);
    print_paths(&g, src, dist);
    
    return 0;
}