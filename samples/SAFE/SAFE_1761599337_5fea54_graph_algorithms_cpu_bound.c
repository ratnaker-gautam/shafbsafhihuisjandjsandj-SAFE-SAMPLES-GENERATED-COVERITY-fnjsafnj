//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 100
#define INF INT_MAX

typedef struct {
    int adjacency[MAX_NODES][MAX_NODES];
    int num_nodes;
} Graph;

void initialize_graph(Graph *g, int n) {
    if (n <= 0 || n > MAX_NODES) {
        fprintf(stderr, "Invalid number of nodes\n");
        exit(1);
    }
    g->num_nodes = n;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            g->adjacency[i][j] = (i == j) ? 0 : INF;
        }
    }
}

void add_edge(Graph *g, int u, int v, int weight) {
    if (u < 0 || u >= g->num_nodes || v < 0 || v >= g->num_nodes) {
        fprintf(stderr, "Invalid node index\n");
        exit(1);
    }
    if (weight < 0) {
        fprintf(stderr, "Negative weights not allowed\n");
        exit(1);
    }
    g->adjacency[u][v] = weight;
    g->adjacency[v][u] = weight;
}

void floyd_warshall(Graph *g, int dist[MAX_NODES][MAX_NODES]) {
    for (int i = 0; i < g->num_nodes; i++) {
        for (int j = 0; j < g->num_nodes; j++) {
            dist[i][j] = g->adjacency[i][j];
        }
    }
    
    for (int k = 0; k < g->num_nodes; k++) {
        for (int i = 0; i < g->num_nodes; i++) {
            for (int j = 0; j < g->num_nodes; j++) {
                if (dist[i][k] != INF && dist[k][j] != INF) {
                    long long sum = (long long)dist[i][k] + dist[k][j];
                    if (sum > INT_MAX) continue;
                    if (dist[i][j] > sum) {
                        dist[i][j] = (int)sum;
                    }
                }
            }
        }
    }
}

void print_shortest_paths(Graph *g, int dist[MAX_NODES][MAX_NODES]) {
    printf("Shortest path distances:\n");
    for (int i = 0; i < g->num_nodes; i++) {
        for (int j = 0; j < g->num_nodes; j++) {
            if (dist[i][j] == INF) {
                printf("INF ");
            } else {
                printf("%3d ", dist[i][j]);
            }
        }
        printf("\n");
    }
}

int main() {
    Graph g;
    int dist[MAX_NODES][MAX_NODES];
    
    printf("Enter number of nodes (1-100): ");
    int n;
    if (scanf("%d", &n) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    initialize_graph(&g, n);
    
    printf("Enter number of edges: ");
    int m;
    if (scanf("%d", &m) != 1 || m < 0) {
        fprintf(stderr, "Invalid number of edges\n");
        return 1;
    }
    
    printf("Enter edges (u v weight):\n");
    for (int i = 0; i < m; i++) {
        int u, v, w;
        if (scanf("%d %d %d", &u, &v, &w) != 3) {
            fprintf(stderr, "Invalid edge input\n");
            return 1;
        }
        add_edge(&g, u, v, w);
    }
    
    floyd_warshall(&g, dist);
    print_shortest_paths(&g, dist);
    
    return 0;
}