//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

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
        return;
    }
    g->num_nodes = nodes;
    g->num_edges = edges;
    for (int i = 0; i < edges; i++) {
        (g->edges + i)->src = -1;
        (g->edges + i)->dest = -1;
        (g->edges + i)->weight = 0;
    }
}

int add_edge(struct Graph *g, int src, int dest, int weight) {
    if (g == NULL || src < 0 || src >= g->num_nodes || dest < 0 || dest >= g->num_nodes || weight < 0) {
        return -1;
    }
    if (g->num_edges >= MAX_EDGES) {
        return -1;
    }
    struct Edge *e = g->edges + g->num_edges;
    e->src = src;
    e->dest = dest;
    e->weight = weight;
    g->num_edges++;
    return 0;
}

void bellman_ford(struct Graph *g, int start, int *dist) {
    if (g == NULL || dist == NULL || start < 0 || start >= g->num_nodes) {
        return;
    }
    
    for (int i = 0; i < g->num_nodes; i++) {
        *(dist + i) = INT_MAX;
    }
    *(dist + start) = 0;
    
    for (int i = 0; i < g->num_nodes - 1; i++) {
        for (int j = 0; j < g->num_edges; j++) {
            struct Edge *e = g->edges + j;
            if (*(dist + e->src) != INT_MAX) {
                int new_dist = *(dist + e->src) + e->weight;
                if (new_dist < *(dist + e->dest)) {
                    *(dist + e->dest) = new_dist;
                }
            }
        }
    }
    
    for (int j = 0; j < g->num_edges; j++) {
        struct Edge *e = g->edges + j;
        if (*(dist + e->src) != INT_MAX && *(dist + e->src) + e->weight < *(dist + e->dest)) {
            printf("Graph contains negative weight cycle\n");
            return;
        }
    }
}

void print_distances(int *dist, int n, int start) {
    if (dist == NULL || n <= 0 || start < 0 || start >= n) {
        return;
    }
    printf("Shortest distances from node %d:\n", start);
    for (int i = 0; i < n; i++) {
        if (*(dist + i) == INT_MAX) {
            printf("Node %d: unreachable\n", i);
        } else {
            printf("Node %d: %d\n", i, *(dist + i));
        }
    }
}

int main() {
    struct Graph g;
    int distances[MAX_NODES];
    
    int num_nodes, num_edges;
    printf("Enter number of nodes (1-100): ");
    if (scanf("%d", &num_nodes) != 1 || num_nodes < 1 || num_nodes > MAX_NODES) {
        printf("Invalid number of nodes\n");
        return 1;
    }
    
    printf("Enter number of edges (0-1000): ");
    if (scanf("%d", &num_edges) != 1 || num_edges < 0 || num_edges > MAX_EDGES) {
        printf("Invalid number of edges\n");
        return 1;
    }
    
    init_graph(&g, num_nodes, 0);
    
    printf("Enter edges as 'source destination weight':\n");
    for (int i = 0; i < num_edges; i++) {
        int src, dest, weight;
        if (scanf("%d %d %d", &src, &dest, &weight) != 3) {
            printf("Invalid edge input\n");
            return 1;
        }
        if (add_edge(&g, src, dest, weight) != 0) {
            printf("Failed to add edge\n");
            return 1;
        }
    }
    
    int start_node;
    printf("Enter starting node: ");
    if (scanf("%d", &start_node) != 1 || start_node < 0 || start_node >= num_nodes) {
        printf("Invalid starting node\n");
        return 1;
    }
    
    bellman_ford(&g, start_node, distances);
    print_distances(distances, num_nodes, start_node);
    
    return 0;
}