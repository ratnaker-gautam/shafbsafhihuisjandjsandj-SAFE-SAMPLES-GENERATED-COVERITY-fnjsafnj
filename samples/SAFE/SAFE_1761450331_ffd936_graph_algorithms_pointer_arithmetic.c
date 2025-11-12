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

void dijkstra(struct Graph *g, int start) {
    if (g == NULL || !validate_node(g, start)) {
        fprintf(stderr, "Invalid Dijkstra parameters\n");
        return;
    }

    int dist[MAX_NODES];
    int visited[MAX_NODES];
    
    for (int i = 0; i < g->num_nodes; i++) {
        *(dist + i) = INT_MAX;
        *(visited + i) = 0;
    }
    *(dist + start) = 0;

    for (int count = 0; count < g->num_nodes - 1; count++) {
        int min_dist = INT_MAX;
        int min_node = -1;

        for (int v = 0; v < g->num_nodes; v++) {
            if (!*(visited + v) && *(dist + v) <= min_dist) {
                min_dist = *(dist + v);
                min_node = v;
            }
        }

        if (min_node == -1) break;
        *(visited + min_node) = 1;

        for (int i = 0; i < g->num_edges; i++) {
            struct Edge *e = g->edges + i;
            if (e->src == min_node && !*(visited + e->dest)) {
                int new_dist = *(dist + min_node) + e->weight;
                if (*(dist + min_node) != INT_MAX && new_dist < *(dist + e->dest)) {
                    *(dist + e->dest) = new_dist;
                }
            }
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
    int num_nodes, num_edges;
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &num_nodes) != 1 || num_nodes < 1 || num_nodes > MAX_NODES) {
        fprintf(stderr, "Invalid number of nodes\n");
        return EXIT_FAILURE;
    }
    
    printf("Enter number of edges (0-%d): ", MAX_EDGES);
    if (scanf("%d", &num_edges) != 1 || num_edges < 0 || num_edges > MAX_EDGES) {
        fprintf(stderr, "Invalid number of edges\n");
        return EXIT_FAILURE;
    }
    
    init_graph(&g, num_nodes, num_edges);
    
    printf("Enter edges (source destination weight):\n");
    for (int i = 0; i < num_edges; i++) {
        int src, dest, weight;
        if (scanf("%d %d %d", &src, &dest, &weight) != 3) {
            fprintf(stderr, "Invalid edge input\n");
            return EXIT_FAILURE;
        }
        add_edge(&g, i, src, dest, weight);
    }
    
    int start_node;
    printf("Enter starting node for Dijkstra: ");
    if (scanf("%d", &start_node) != 1) {
        fprintf(stderr, "Invalid start node\n");
        return EXIT_FAILURE;
    }
    
    dijkstra(&g, start_node);
    
    return EXIT_SUCCESS;
}