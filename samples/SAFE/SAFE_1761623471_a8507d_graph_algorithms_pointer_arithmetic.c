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
    struct Edge *edges;
};

void init_graph(struct Graph *g, int nodes, int edges) {
    if (nodes <= 0 || nodes > MAX_NODES || edges <= 0 || edges > MAX_EDGES) {
        fprintf(stderr, "Invalid graph parameters\n");
        exit(1);
    }
    g->num_nodes = nodes;
    g->num_edges = edges;
    g->edges = malloc(edges * sizeof(struct Edge));
    if (!g->edges) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
}

void free_graph(struct Graph *g) {
    if (g->edges) {
        free(g->edges);
        g->edges = NULL;
    }
}

int validate_node(struct Graph *g, int node) {
    return node >= 0 && node < g->num_nodes;
}

void add_edge(struct Graph *g, int idx, int src, int dest, int weight) {
    if (idx < 0 || idx >= g->num_edges) {
        fprintf(stderr, "Invalid edge index\n");
        return;
    }
    if (!validate_node(g, src) || !validate_node(g, dest)) {
        fprintf(stderr, "Invalid node index\n");
        return;
    }
    (g->edges + idx)->src = src;
    (g->edges + idx)->dest = dest;
    (g->edges + idx)->weight = weight;
}

void print_graph(struct Graph *g) {
    printf("Graph with %d nodes and %d edges:\n", g->num_nodes, g->num_edges);
    for (int i = 0; i < g->num_edges; i++) {
        struct Edge *e = g->edges + i;
        printf("Edge %d: %d -> %d (weight %d)\n", i, e->src, e->dest, e->weight);
    }
}

void bellman_ford(struct Graph *g, int start) {
    if (!validate_node(g, start)) {
        fprintf(stderr, "Invalid start node\n");
        return;
    }

    int *dist = malloc(g->num_nodes * sizeof(int));
    if (!dist) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }

    for (int i = 0; i < g->num_nodes; i++) {
        *(dist + i) = 1000000;
    }
    *(dist + start) = 0;

    for (int i = 0; i < g->num_nodes - 1; i++) {
        for (int j = 0; j < g->num_edges; j++) {
            struct Edge *e = g->edges + j;
            if (*(dist + e->src) != 1000000 && *(dist + e->src) + e->weight < *(dist + e->dest)) {
                *(dist + e->dest) = *(dist + e->src) + e->weight;
            }
        }
    }

    for (int j = 0; j < g->num_edges; j++) {
        struct Edge *e = g->edges + j;
        if (*(dist + e->src) != 1000000 && *(dist + e->src) + e->weight < *(dist + e->dest)) {
            printf("Graph contains negative weight cycle\n");
            free(dist);
            return;
        }
    }

    printf("Shortest distances from node %d:\n", start);
    for (int i = 0; i < g->num_nodes; i++) {
        printf("Node %d: %d\n", i, *(dist + i));
    }

    free(dist);
}

int main() {
    struct Graph g;
    int nodes, edges;

    printf("Enter number of nodes (1-100): ");
    if (scanf("%d", &nodes) != 1 || nodes < 1 || nodes > MAX_NODES) {
        fprintf(stderr, "Invalid number of nodes\n");
        return 1;
    }

    printf("Enter number of edges (1-1000): ");
    if (scanf("%d", &edges) != 1 || edges < 1 || edges > MAX_EDGES) {
        fprintf(stderr, "Invalid number of edges\n");
        return 1;
    }

    init_graph(&g, nodes, edges);

    printf("Enter edges (source destination weight):\n");
    for (int i = 0; i < edges; i++) {
        int src, dest, weight;
        if (scanf("%d %d %d", &src, &dest, &weight) != 3) {
            fprintf(stderr, "Invalid edge input\n");
            free_graph(&g);
            return 1;
        }
        add_edge(&g, i, src, dest, weight);
    }

    print_graph(&g);

    int start;
    printf("Enter start node for Bellman-Ford: ");
    if (scanf("%d", &start) != 1) {
        fprintf(stderr, "Invalid start node\n");
        free_graph(&g);
        return 1;
    }

    bellman_ford(&g, start);

    free_graph(&g);
    return 0;
}