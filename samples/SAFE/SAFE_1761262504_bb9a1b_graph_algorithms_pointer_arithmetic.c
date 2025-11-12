//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: graph_algorithms
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
    (*(g->edges + idx)).src = src;
    (*(g->edges + idx)).dest = dest;
    (*(g->edges + idx)).weight = weight;
    return 1;
}

void bellman_ford(struct Graph *g, int start) {
    if (g == NULL || start < 0 || start >= g->num_nodes) {
        return;
    }
    
    int dist[MAX_NODES];
    int i, j;
    
    for (i = 0; i < g->num_nodes; i++) {
        *(dist + i) = 999999;
    }
    *(dist + start) = 0;
    
    for (i = 0; i < g->num_nodes - 1; i++) {
        for (j = 0; j < g->num_edges; j++) {
            int u = (*(g->edges + j)).src;
            int v = (*(g->edges + j)).dest;
            int w = (*(g->edges + j)).weight;
            
            if (*(dist + u) != 999999 && *(dist + u) + w < *(dist + v)) {
                *(dist + v) = *(dist + u) + w;
            }
        }
    }
    
    for (j = 0; j < g->num_edges; j++) {
        int u = (*(g->edges + j)).src;
        int v = (*(g->edges + j)).dest;
        int w = (*(g->edges + j)).weight;
        
        if (*(dist + u) != 999999 && *(dist + u) + w < *(dist + v)) {
            printf("Graph contains negative weight cycle\n");
            return;
        }
    }
    
    printf("Shortest distances from node %d:\n", start);
    for (i = 0; i < g->num_nodes; i++) {
        printf("Node %d: %d\n", i, *(dist + i));
    }
}

int main() {
    struct Graph g;
    int num_nodes, num_edges;
    int i, src, dest, weight;
    
    printf("Enter number of nodes (1-100): ");
    if (scanf("%d", &num_nodes) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    printf("Enter number of edges (0-1000): ");
    if (scanf("%d", &num_edges) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (num_nodes < 1 || num_nodes > MAX_NODES || num_edges < 0 || num_edges > MAX_EDGES) {
        printf("Invalid graph parameters\n");
        return 1;
    }
    
    init_graph(&g, num_nodes, num_edges);
    
    for (i = 0; i < num_edges; i++) {
        printf("Enter edge %d (src dest weight): ", i);
        if (scanf("%d %d %d", &src, &dest, &weight) != 3) {
            printf("Invalid edge input\n");
            return 1;
        }
        
        if (!add_edge(&g, i, src, dest, weight)) {
            printf("Invalid edge parameters\n");
            return 1;
        }
    }
    
    int start_node;
    printf("Enter starting node: ");
    if (scanf("%d", &start_node) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (start_node < 0 || start_node >= num_nodes) {
        printf("Invalid starting node\n");
        return 1;
    }
    
    bellman_ford(&g, start_node);
    
    return 0;
}