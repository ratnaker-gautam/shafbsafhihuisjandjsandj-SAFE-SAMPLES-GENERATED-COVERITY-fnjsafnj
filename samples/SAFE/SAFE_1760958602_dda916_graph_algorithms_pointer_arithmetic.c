//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: graph_algorithms
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
        fprintf(stderr, "Invalid graph parameters\n");
        exit(EXIT_FAILURE);
    }
    g->num_nodes = nodes;
    g->num_edges = edges;
    memset(g->edges, 0, sizeof(g->edges));
}

int validate_node(struct Graph *g, int node) {
    return (g != NULL && node >= 0 && node < g->num_nodes);
}

void add_edge(struct Graph *g, int idx, int src, int dest, int weight) {
    if (g == NULL || idx < 0 || idx >= g->num_edges || !validate_node(g, src) || 
        !validate_node(g, dest) || weight < 0) {
        fprintf(stderr, "Invalid edge parameters\n");
        exit(EXIT_FAILURE);
    }
    (g->edges + idx)->src = src;
    (g->edges + idx)->dest = dest;
    (g->edges + idx)->weight = weight;
}

void bellman_ford(struct Graph *g, int start) {
    if (g == NULL || !validate_node(g, start)) {
        fprintf(stderr, "Invalid graph or start node\n");
        return;
    }

    int dist[MAX_NODES];
    for (int i = 0; i < g->num_nodes; i++) {
        *(dist + i) = INT_MAX;
    }
    *(dist + start) = 0;

    for (int i = 0; i < g->num_nodes - 1; i++) {
        for (int j = 0; j < g->num_edges; j++) {
            struct Edge *e = g->edges + j;
            if (*(dist + e->src) != INT_MAX && 
                *(dist + e->src) + e->weight < *(dist + e->dest)) {
                *(dist + e->dest) = *(dist + e->src) + e->weight;
            }
        }
    }

    for (int j = 0; j < g->num_edges; j++) {
        struct Edge *e = g->edges + j;
        if (*(dist + e->src) != INT_MAX && 
            *(dist + e->src) + e->weight < *(dist + e->dest)) {
            fprintf(stderr, "Graph contains negative weight cycle\n");
            return;
        }
    }

    printf("Shortest distances from node %d:\n", start);
    for (int i = 0; i < g->num_nodes; i++) {
        if (*(dist + i) == INT_MAX) {
            printf("Node %d: unreachable\n", i);
        } else {
            printf("Node %d: %d\n", i, *(dist + i));
        }
    }
}

int main() {
    struct Graph g;
    int nodes, edges, start;

    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &nodes) != 1 || nodes < 1 || nodes > MAX_NODES) {
        fprintf(stderr, "Invalid number of nodes\n");
        return EXIT_FAILURE;
    }

    printf("Enter number of edges (0-%d): ", MAX_EDGES);
    if (scanf("%d", &edges) != 1 || edges < 0 || edges > MAX_EDGES) {
        fprintf(stderr, "Invalid number of edges\n");
        return EXIT_FAILURE;
    }

    init_graph(&g, nodes, edges);

    printf("Enter edges (source destination weight):\n");
    for (int i = 0; i < edges; i++) {
        int src, dest, weight;
        if (scanf("%d %d %d", &src, &dest, &weight) != 3) {
            fprintf(stderr, "Invalid edge input\n");
            return EXIT_FAILURE;
        }
        add_edge(&g, i, src, dest, weight);
    }

    printf("Enter start node (0-%d): ", nodes - 1);
    if (scanf("%d", &start) != 1 || start < 0 || start >= nodes) {
        fprintf(stderr, "Invalid start node\n");
        return EXIT_FAILURE;
    }

    bellman_ford(&g, start);

    return EXIT_SUCCESS;
}