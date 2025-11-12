//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 100

struct Graph {
    int num_nodes;
    int adj_matrix[MAX_NODES][MAX_NODES];
};

void initialize_graph(struct Graph *g, int n) {
    if (n <= 0 || n > MAX_NODES) {
        fprintf(stderr, "Invalid number of nodes\n");
        exit(1);
    }
    g->num_nodes = n;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            g->adj_matrix[i][j] = 0;
        }
    }
}

void add_edge(struct Graph *g, int u, int v, int weight) {
    if (u < 0 || u >= g->num_nodes || v < 0 || v >= g->num_nodes) {
        fprintf(stderr, "Invalid edge\n");
        exit(1);
    }
    g->adj_matrix[u][v] = weight;
    g->adj_matrix[v][u] = weight;
}

void dijkstra(struct Graph *g, int src) {
    int dist[MAX_NODES];
    int visited[MAX_NODES];
    
    for (int i = 0; i < g->num_nodes; i++) {
        dist[i] = INT_MAX;
        visited[i] = 0;
    }
    dist[src] = 0;

    for (int count = 0; count < g->num_nodes - 1; count++) {
        int min_dist = INT_MAX;
        int min_index = -1;
        
        int i = 0;
        while (i < g->num_nodes) {
            if (!visited[i] && dist[i] <= min_dist) {
                min_dist = dist[i];
                min_index = i;
            }
            i++;
        }
        
        if (min_index == -1) break;
        
        visited[min_index] = 1;
        
        int v = 0;
        do {
            if (!visited[v] && g->adj_matrix[min_index][v] != 0 && 
                dist[min_index] != INT_MAX && 
                dist[min_index] + g->adj_matrix[min_index][v] < dist[v]) {
                dist[v] = dist[min_index] + g->adj_matrix[min_index][v];
            }
            v++;
        } while (v < g->num_nodes);
    }

    printf("Shortest distances from node %d:\n", src);
    for (int i = 0; i < g->num_nodes; i++) {
        if (dist[i] == INT_MAX) {
            printf("Node %d: unreachable\n", i);
        } else {
            printf("Node %d: %d\n", i, dist[i]);
        }
    }
}

int main() {
    struct Graph g;
    int num_nodes, num_edges;
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &num_nodes) != 1 || num_nodes < 1 || num_nodes > MAX_NODES) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    initialize_graph(&g, num_nodes);
    
    printf("Enter number of edges: ");
    if (scanf("%d", &num_edges) != 1 || num_edges < 0) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    printf("Enter edges as (u v weight):\n");
    for (int i = 0; i < num_edges; i++) {
        int u, v, w;
        if (scanf("%d %d %d", &u, &v, &w) != 3) {
            fprintf(stderr, "Invalid edge input\n");
            return 1;
        }
        if (u < 0 || u >= num_nodes || v < 0 || v >= num_nodes || w < 0) {
            fprintf(stderr, "Invalid edge parameters\n");
            return 1;
        }
        add_edge(&g, u, v, w);
    }
    
    int source;
    printf("Enter source node: ");
    if (scanf("%d", &source) != 1 || source < 0 || source >= num_nodes) {
        fprintf(stderr, "Invalid source node\n");
        return 1;
    }
    
    dijkstra(&g, source);
    
    return 0;
}