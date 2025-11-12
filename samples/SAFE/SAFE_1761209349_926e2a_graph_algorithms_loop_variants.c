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

void floyd_warshall(struct Graph *g, int dist[MAX_NODES][MAX_NODES]) {
    for (int i = 0; i < g->nodes; i++) {
        for (int j = 0; j < g->nodes; j++) {
            dist[i][j] = g->adj[i][j];
        }
    }
    
    for (int k = 0; k < g->nodes; k++) {
        for (int i = 0; i < g->nodes; i++) {
            if (dist[i][k] == INF) continue;
            for (int j = 0; j < g->nodes; j++) {
                if (dist[k][j] != INF && dist[i][k] != INF) {
                    if (dist[i][j] > dist[i][k] + dist[k][j]) {
                        if (dist[i][k] > INT_MAX - dist[k][j]) {
                            continue;
                        }
                        dist[i][j] = dist[i][k] + dist[k][j];
                    }
                }
            }
        }
    }
}

void print_shortest_paths(struct Graph *g, int dist[MAX_NODES][MAX_NODES]) {
    printf("Shortest path distances:\n");
    for (int i = 0; i < g->nodes; i++) {
        for (int j = 0; j < g->nodes; j++) {
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
    struct Graph g;
    int nodes = 5;
    init_graph(&g, nodes);
    
    add_edge(&g, 0, 1, 5);
    add_edge(&g, 0, 3, 10);
    add_edge(&g, 1, 2, 3);
    add_edge(&g, 1, 3, 8);
    add_edge(&g, 2, 4, 2);
    add_edge(&g, 3, 4, 1);
    
    int dist[MAX_NODES][MAX_NODES];
    floyd_warshall(&g, dist);
    print_shortest_paths(&g, dist);
    
    printf("\nFinding all pairs shortest paths using different loop variants:\n");
    
    int count = 0;
    int i = 0;
    while (i < g.nodes) {
        int j = 0;
        do {
            if (dist[i][j] != INF) {
                count++;
            }
            j++;
        } while (j < g.nodes);
        i++;
    }
    printf("Total reachable paths: %d\n", count);
    
    int reachable_pairs = 0;
    for (int k = 0; k < g.nodes; k++) {
        int temp_count = 0;
        for (int m = 0; m < g.nodes; m++) {
            if (dist[k][m] != INF && k != m) {
                temp_count++;
            }
        }
        reachable_pairs += temp_count;
    }
    printf("Total reachable node pairs: %d\n", reachable_pairs);
    
    return 0;
}