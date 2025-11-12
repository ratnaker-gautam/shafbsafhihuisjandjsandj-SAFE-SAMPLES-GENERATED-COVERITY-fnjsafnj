//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 100

struct Graph {
    int num_nodes;
    int adj_matrix[MAX_NODES][MAX_NODES];
};

void init_graph(struct Graph *g, int n) {
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
        fprintf(stderr, "Invalid node index\n");
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
    
    int count = 0;
    while (count < g->num_nodes) {
        int min_dist = INT_MAX;
        int u = -1;
        
        for (int i = 0; i < g->num_nodes; i++) {
            if (!visited[i] && dist[i] < min_dist) {
                min_dist = dist[i];
                u = i;
            }
        }
        
        if (u == -1) break;
        visited[u] = 1;
        count++;
        
        for (int v = 0; v < g->num_nodes; v++) {
            if (!visited[v] && g->adj_matrix[u][v] != 0) {
                int new_dist = dist[u] + g->adj_matrix[u][v];
                if (new_dist > 0 && new_dist < dist[v]) {
                    dist[v] = new_dist;
                }
            }
        }
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
    
    init_graph(&g, num_nodes);
    
    printf("Enter number of edges: ");
    if (scanf("%d", &num_edges) != 1 || num_edges < 0) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    for (int i = 0; i < num_edges; i++) {
        int u, v, w;
        printf("Enter edge %d (u v weight): ", i + 1);
        if (scanf("%d %d %d", &u, &v, &w) != 3) {
            fprintf(stderr, "Invalid input\n");
            return 1;
        }
        if (u < 0 || u >= num_nodes || v < 0 || v >= num_nodes) {
            fprintf(stderr, "Invalid node index\n");
            return 1;
        }
        if (w <= 0) {
            fprintf(stderr, "Weight must be positive\n");
            return 1;
        }
        add_edge(&g, u, v, w);
    }
    
    int start_node;
    printf("Enter starting node for Dijkstra: ");
    if (scanf("%d", &start_node) != 1 || start_node < 0 || start_node >= num_nodes) {
        fprintf(stderr, "Invalid starting node\n");
        return 1;
    }
    
    dijkstra(&g, start_node);
    
    return 0;
}