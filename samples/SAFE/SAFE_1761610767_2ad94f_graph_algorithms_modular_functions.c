//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_NODES 100
#define INF INT_MAX

struct Graph {
    int nodes;
    int edges;
    int adj[MAX_NODES][MAX_NODES];
};

void initialize_graph(struct Graph *g, int nodes) {
    if (g == NULL || nodes <= 0 || nodes > MAX_NODES) {
        return;
    }
    g->nodes = nodes;
    g->edges = 0;
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            g->adj[i][j] = INF;
        }
        g->adj[i][i] = 0;
    }
}

int add_edge(struct Graph *g, int u, int v, int weight) {
    if (g == NULL || u < 0 || u >= g->nodes || v < 0 || v >= g->nodes) {
        return 0;
    }
    if (weight < 0) {
        return 0;
    }
    g->adj[u][v] = weight;
    g->edges++;
    return 1;
}

void floyd_warshall(struct Graph *g, int dist[MAX_NODES][MAX_NODES]) {
    if (g == NULL || dist == NULL) {
        return;
    }
    for (int i = 0; i < g->nodes; i++) {
        for (int j = 0; j < g->nodes; j++) {
            dist[i][j] = g->adj[i][j];
        }
    }
    for (int k = 0; k < g->nodes; k++) {
        for (int i = 0; i < g->nodes; i++) {
            for (int j = 0; j < g->nodes; j++) {
                if (dist[i][k] != INF && dist[k][j] != INF) {
                    if (dist[i][j] > dist[i][k] + dist[k][j]) {
                        dist[i][j] = dist[i][k] + dist[k][j];
                    }
                }
            }
        }
    }
}

void print_shortest_paths(struct Graph *g, int dist[MAX_NODES][MAX_NODES]) {
    if (g == NULL || dist == NULL) {
        return;
    }
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

int read_int(const char *prompt, int min, int max) {
    int value;
    char buffer[100];
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
        }
        if (sscanf(buffer, "%d", &value) == 1) {
            if (value >= min && value <= max) {
                return value;
            }
        }
        printf("Invalid input. Enter a number between %d and %d.\n", min, max);
    }
}

int main() {
    struct Graph g;
    int nodes, edges;
    int dist[MAX_NODES][MAX_NODES];
    
    printf("Graph Shortest Path Calculator\n");
    printf("==============================\n");
    
    nodes = read_int("Enter number of nodes (1-100): ", 1, MAX_NODES);
    if (nodes == -1) {
        return 1;
    }
    
    initialize_graph(&g, nodes);
    
    edges = read_int("Enter number of edges: ", 0, nodes * nodes);
    if (edges == -1) {
        return 1;
    }
    
    printf("Enter edges as: source destination weight\n");
    for (int i = 0; i < edges; i++) {
        int u, v, w;
        printf("Edge %d: ", i + 1);
        if (scanf("%d %d %d", &u, &v, &w) != 3) {
            printf("Invalid edge format.\n");
            while (getchar() != '\n');
            i--;
            continue;
        }
        if (!add_edge(&g, u, v, w)) {
            printf("Invalid edge parameters.\n");
            i--;
        }
    }
    
    floyd_warshall(&g, dist);
    print_shortest_paths(&g, dist);
    
    return 0;
}