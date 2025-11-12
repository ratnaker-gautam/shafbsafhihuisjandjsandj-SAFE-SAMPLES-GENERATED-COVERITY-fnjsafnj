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
        return;
    }
    g->num_nodes = nodes;
    g->num_edges = edges;
    for (int i = 0; i < edges; i++) {
        (g->edges + i)->src = 0;
        (g->edges + i)->dest = 0;
        (g->edges + i)->weight = 0;
    }
}

int add_edge(struct Graph *g, int src, int dest, int weight) {
    if (g == NULL || src < 0 || src >= g->num_nodes || dest < 0 || dest >= g->num_nodes || g->num_edges >= MAX_EDGES) {
        return 0;
    }
    struct Edge *edge_ptr = g->edges + g->num_edges;
    edge_ptr->src = src;
    edge_ptr->dest = dest;
    edge_ptr->weight = weight;
    g->num_edges++;
    return 1;
}

void print_graph(struct Graph *g) {
    if (g == NULL) {
        return;
    }
    printf("Graph with %d nodes and %d edges:\n", g->num_nodes, g->num_edges);
    for (int i = 0; i < g->num_edges; i++) {
        struct Edge *edge_ptr = g->edges + i;
        printf("Edge %d: %d -> %d (weight %d)\n", i, edge_ptr->src, edge_ptr->dest, edge_ptr->weight);
    }
}

int find_shortest_path(struct Graph *g, int start, int end) {
    if (g == NULL || start < 0 || start >= g->num_nodes || end < 0 || end >= g->num_nodes) {
        return -1;
    }
    int dist[MAX_NODES];
    for (int i = 0; i < g->num_nodes; i++) {
        *(dist + i) = 1000000;
    }
    *(dist + start) = 0;
    for (int i = 0; i < g->num_nodes - 1; i++) {
        for (int j = 0; j < g->num_edges; j++) {
            struct Edge *edge_ptr = g->edges + j;
            int u = edge_ptr->src;
            int v = edge_ptr->dest;
            int w = edge_ptr->weight;
            if (*(dist + u) != 1000000 && *(dist + u) + w < *(dist + v)) {
                *(dist + v) = *(dist + u) + w;
            }
        }
    }
    return *(dist + end);
}

int main() {
    struct Graph graph;
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
    init_graph(&graph, num_nodes, num_edges);
    printf("Enter edges as 'source destination weight' (0-indexed):\n");
    for (int i = 0; i < num_edges; i++) {
        int src, dest, weight;
        if (scanf("%d %d %d", &src, &dest, &weight) != 3) {
            printf("Invalid edge input\n");
            return 1;
        }
        if (!add_edge(&graph, src, dest, weight)) {
            printf("Failed to add edge\n");
            return 1;
        }
    }
    print_graph(&graph);
    int start, end;
    printf("Enter start and end nodes for shortest path: ");
    if (scanf("%d %d", &start, &end) != 2) {
        printf("Invalid node input\n");
        return 1;
    }
    int shortest = find_shortest_path(&graph, start, end);
    if (shortest == 1000000) {
        printf("No path exists between %d and %d\n", start, end);
    } else {
        printf("Shortest path distance from %d to %d: %d\n", start, end, shortest);
    }
    return 0;
}