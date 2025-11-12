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
    add_edge(&g, 1, 3, 2);
    add_edge(&g, 2, 4, 1);
    add_edge(&g, 3, 4, 4);
    
    int dist[MAX_NODES][MAX_NODES];
    floyd_warshall(&g, dist);
    print_shortest_paths(&g, dist);
    
    printf("\nFinding nodes reachable within distance 6:\n");
    int reachable_count = 0;
    int current_node = 0;
    
    while (current_node < nodes) {
        int count_from_current = 0;
        for (int target = 0; target < nodes; target++) {
            if (dist[current_node][target] <= 6 && dist[current_node][target] != INF) {
                count_from_current++;
            }
        }
        printf("Node %d: %d nodes reachable within distance 6\n", current_node, count_from_current);
        reachable_count += count_from_current;
        current_node++;
    }
    
    printf("Total reachable pairs within distance 6: %d\n", reachable_count);
    
    int path_sum = 0;
    int path_count = 0;
    int i = 0;
    do {
        int j = 0;
        while (j < nodes) {
            if (dist[i][j] != INF && i != j) {
                path_sum += dist[i][j];
                path_count++;
            }
            j++;
        }
        i++;
    } while (i < nodes);
    
    if (path_count > 0) {
        printf("Average shortest path length: %.2f\n", (double)path_sum / path_count);
    } else {
        printf("No paths found\n");
    }
    
    return 0;
}