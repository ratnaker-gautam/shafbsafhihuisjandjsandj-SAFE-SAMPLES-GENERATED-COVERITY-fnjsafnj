//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 100
#define INF INT_MAX

struct Graph {
    int nodes;
    int adj[MAX_NODES][MAX_NODES];
};

struct Graph* create_graph(int n) {
    if (n <= 0 || n > MAX_NODES) {
        return NULL;
    }
    struct Graph* graph = malloc(sizeof(struct Graph));
    if (graph == NULL) {
        return NULL;
    }
    graph->nodes = n;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            graph->adj[i][j] = (i == j) ? 0 : INF;
        }
    }
    return graph;
}

int add_edge(struct Graph* graph, int u, int v, int weight) {
    if (graph == NULL || u < 0 || u >= graph->nodes || v < 0 || v >= graph->nodes) {
        return 0;
    }
    if (weight < 0) {
        return 0;
    }
    graph->adj[u][v] = weight;
    graph->adj[v][u] = weight;
    return 1;
}

void floyd_warshall(struct Graph* graph) {
    if (graph == NULL) {
        return;
    }
    int dist[MAX_NODES][MAX_NODES];
    for (int i = 0; i < graph->nodes; i++) {
        for (int j = 0; j < graph->nodes; j++) {
            dist[i][j] = graph->adj[i][j];
        }
    }
    for (int k = 0; k < graph->nodes; k++) {
        for (int i = 0; i < graph->nodes; i++) {
            for (int j = 0; j < graph->nodes; j++) {
                if (dist[i][k] != INF && dist[k][j] != INF) {
                    if (dist[i][j] > dist[i][k] + dist[k][j]) {
                        dist[i][j] = dist[i][k] + dist[k][j];
                    }
                }
            }
        }
    }
    printf("Shortest path distances:\n");
    for (int i = 0; i < graph->nodes; i++) {
        for (int j = 0; j < graph->nodes; j++) {
            if (dist[i][j] == INF) {
                printf("INF ");
            } else {
                printf("%3d ", dist[i][j]);
            }
        }
        printf("\n");
    }
}

int validate_input(int min, int max) {
    int value;
    while (1) {
        if (scanf("%d", &value) != 1) {
            while (getchar() != '\n');
            printf("Invalid input. Enter a number between %d and %d: ", min, max);
            continue;
        }
        if (value >= min && value <= max) {
            return value;
        }
        printf("Value out of range. Enter a number between %d and %d: ", min, max);
    }
}

int main() {
    printf("Graph Shortest Path Calculator\n");
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    int n = validate_input(1, MAX_NODES);
    
    struct Graph* graph = create_graph(n);
    if (graph == NULL) {
        printf("Failed to create graph.\n");
        return 1;
    }
    
    printf("Enter number of edges: ");
    int edges = validate_input(0, n * (n - 1) / 2);
    
    for (int i = 0; i < edges; i++) {
        printf("Edge %d: enter source, destination, weight: ", i + 1);
        int u = validate_input(0, n - 1);
        int v = validate_input(0, n - 1);
        int w = validate_input(0, 1000);
        
        if (!add_edge(graph, u, v, w)) {
            printf("Failed to add edge.\n");
            i--;
        }
    }
    
    floyd_warshall(graph);
    
    free(graph);
    return 0;
}