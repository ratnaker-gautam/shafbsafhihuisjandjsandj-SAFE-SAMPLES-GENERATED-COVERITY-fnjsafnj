//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 100
#define MAX_EDGES 1000

struct Edge {
    int src;
    int dest;
    int weight;
};

struct Graph {
    int num_nodes;
    int num_edges;
    struct Edge edges[MAX_EDGES];
};

void init_graph(struct Graph *g, int nodes, int edges) {
    if (g == NULL || nodes <= 0 || nodes > MAX_NODES || edges < 0 || edges > MAX_EDGES) {
        exit(1);
    }
    g->num_nodes = nodes;
    g->num_edges = edges;
}

int add_edge(struct Graph *g, int idx, int src, int dest, int weight) {
    if (g == NULL || idx < 0 || idx >= MAX_EDGES) {
        return 0;
    }
    if (src < 0 || src >= g->num_nodes || dest < 0 || dest >= g->num_nodes) {
        return 0;
    }
    if (weight < 0) {
        return 0;
    }
    (g->edges + idx)->src = src;
    (g->edges + idx)->dest = dest;
    (g->edges + idx)->weight = weight;
    return 1;
}

void bellman_ford(struct Graph *g, int start) {
    if (g == NULL || start < 0 || start >= g->num_nodes) {
        return;
    }
    
    int dist[MAX_NODES];
    for (int i = 0; i < g->num_nodes; i++) {
        *(dist + i) = 1000000;
    }
    *(dist + start) = 0;
    
    for (int i = 0; i < g->num_nodes - 1; i++) {
        for (int j = 0; j < g->num_edges; j++) {
            int u = (g->edges + j)->src;
            int v = (g->edges + j)->dest;
            int w = (g->edges + j)->weight;
            if (*(dist + u) != 1000000 && *(dist + u) + w < *(dist + v)) {
                *(dist + v) = *(dist + u) + w;
            }
        }
    }
    
    for (int j = 0; j < g->num_edges; j++) {
        int u = (g->edges + j)->src;
        int v = (g->edges + j)->dest;
        int w = (g->edges + j)->weight;
        if (*(dist + u) != 1000000 && *(dist + u) + w < *(dist + v)) {
            printf("Graph contains negative weight cycle\n");
            return;
        }
    }
    
    printf("Vertex distances from source %d:\n", start);
    for (int i = 0; i < g->num_nodes; i++) {
        printf("Node %d: %d\n", i, *(dist + i));
    }
}

int main() {
    struct Graph g;
    int nodes, edges;
    
    printf("Enter number of nodes (1-100): ");
    if (scanf("%d", &nodes) != 1 || nodes < 1 || nodes > MAX_NODES) {
        printf("Invalid number of nodes\n");
        return 1;
    }
    
    printf("Enter number of edges (0-1000): ");
    if (scanf("%d", &edges) != 1 || edges < 0 || edges > MAX_EDGES) {
        printf("Invalid number of edges\n");
        return 1;
    }
    
    init_graph(&g, nodes, edges);
    
    printf("Enter edges as: source destination weight\n");
    for (int i = 0; i < edges; i++) {
        int src, dest, weight;
        printf("Edge %d: ", i);
        if (scanf("%d %d %d", &src, &dest, &weight) != 3) {
            printf("Invalid edge input\n");
            return 1;
        }
        if (!add_edge(&g, i, src, dest, weight)) {
            printf("Invalid edge parameters\n");
            return 1;
        }
    }
    
    int start;
    printf("Enter starting node: ");
    if (scanf("%d", &start) != 1 || start < 0 || start >= nodes) {
        printf("Invalid starting node\n");
        return 1;
    }
    
    bellman_ford(&g, start);
    
    return 0;
}