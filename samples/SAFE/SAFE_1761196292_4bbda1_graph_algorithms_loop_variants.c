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

int find_min_distance(int dist[], int visited[], int n) {
    int min = INT_MAX;
    int min_index = -1;
    for (int i = 0; i < n; i++) {
        if (!visited[i] && dist[i] <= min) {
            min = dist[i];
            min_index = i;
        }
    }
    return min_index;
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
        int u = find_min_distance(dist, visited, g->num_nodes);
        if (u == -1) break;
        visited[u] = 1;
        
        int v = 0;
        while (v < g->num_nodes) {
            if (!visited[v] && g->adj_matrix[u][v] && dist[u] != INT_MAX) {
                int new_dist = dist[u] + g->adj_matrix[u][v];
                if (new_dist < dist[v]) {
                    dist[v] = new_dist;
                }
            }
            v++;
        }
    }
    
    printf("Vertex Distance from Source %d\n", src);
    for (int i = 0; i < g->num_nodes; i++) {
        if (dist[i] == INT_MAX) {
            printf("%d \t INF\n", i);
        } else {
            printf("%d \t %d\n", i, dist[i]);
        }
    }
}

int main() {
    struct Graph g;
    int num_nodes, num_edges;
    
    printf("Enter number of nodes (1-100): ");
    if (scanf("%d", &num_nodes) != 1 || num_nodes < 1 || num_nodes > MAX_NODES) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    initialize_graph(&g, num_nodes);
    
    printf("Enter number of edges: ");
    if (scanf("%d", &num_edges) != 1 || num_edges < 0 || num_edges > num_nodes * (num_nodes - 1) / 2) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    for (int i = 0; i < num_edges; i++) {
        int u, v, w;
        printf("Enter edge %d (u v weight): ", i + 1);
        if (scanf("%d %d %d", &u, &v, &w) != 3 || u < 0 || u >= num_nodes || v < 0 || v >= num_nodes || w < 0) {
            fprintf(stderr, "Invalid edge\n");
            return 1;
        }
        add_edge(&g, u, v, w);
    }
    
    int source;
    printf("Enter source vertex: ");
    if (scanf("%d", &source) != 1 || source < 0 || source >= num_nodes) {
        fprintf(stderr, "Invalid source vertex\n");
        return 1;
    }
    
    dijkstra(&g, source);
    
    return 0;
}