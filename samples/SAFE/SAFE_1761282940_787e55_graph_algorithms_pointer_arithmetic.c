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

void init_graph(struct Graph *graph, int nodes, int edges) {
    if (nodes <= 0 || nodes > MAX_NODES || edges <= 0 || edges > MAX_EDGES) {
        fprintf(stderr, "Invalid graph parameters\n");
        exit(1);
    }
    graph->num_nodes = nodes;
    graph->num_edges = edges;
    graph->edges = malloc(edges * sizeof(struct Edge));
    if (graph->edges == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
}

void free_graph(struct Graph *graph) {
    free(graph->edges);
}

int validate_node(struct Graph *graph, int node) {
    return node >= 0 && node < graph->num_nodes;
}

void add_edge(struct Graph *graph, int idx, int src, int dest, int weight) {
    if (idx < 0 || idx >= graph->num_edges) {
        fprintf(stderr, "Invalid edge index\n");
        exit(1);
    }
    if (!validate_node(graph, src) || !validate_node(graph, dest)) {
        fprintf(stderr, "Invalid node in edge\n");
        exit(1);
    }
    (graph->edges + idx)->src = src;
    (graph->edges + idx)->dest = dest;
    (graph->edges + idx)->weight = weight;
}

void bellman_ford(struct Graph *graph, int start) {
    if (!validate_node(graph, start)) {
        fprintf(stderr, "Invalid start node\n");
        return;
    }

    int *dist = malloc(graph->num_nodes * sizeof(int));
    if (dist == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }

    for (int i = 0; i < graph->num_nodes; i++) {
        *(dist + i) = 1000000;
    }
    *(dist + start) = 0;

    for (int i = 0; i < graph->num_nodes - 1; i++) {
        for (int j = 0; j < graph->num_edges; j++) {
            struct Edge *edge = graph->edges + j;
            int u = edge->src;
            int v = edge->dest;
            int w = edge->weight;
            if (*(dist + u) != 1000000 && *(dist + u) + w < *(dist + v)) {
                *(dist + v) = *(dist + u) + w;
            }
        }
    }

    for (int j = 0; j < graph->num_edges; j++) {
        struct Edge *edge = graph->edges + j;
        int u = edge->src;
        int v = edge->dest;
        int w = edge->weight;
        if (*(dist + u) != 1000000 && *(dist + u) + w < *(dist + v)) {
            printf("Graph contains negative weight cycle\n");
            free(dist);
            return;
        }
    }

    printf("Shortest distances from node %d:\n", start);
    for (int i = 0; i < graph->num_nodes; i++) {
        printf("Node %d: %d\n", i, *(dist + i));
    }

    free(dist);
}

int main() {
    struct Graph graph;
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

    init_graph(&graph, nodes, edges);

    printf("Enter edges as: source destination weight\n");
    for (int i = 0; i < edges; i++) {
        int src, dest, weight;
        printf("Edge %d: ", i);
        if (scanf("%d %d %d", &src, &dest, &weight) != 3) {
            fprintf(stderr, "Invalid edge input\n");
            free_graph(&graph);
            return 1;
        }
        add_edge(&graph, i, src, dest, weight);
    }

    int start_node;
    printf("Enter start node for Bellman-Ford: ");
    if (scanf("%d", &start_node) != 1) {
        fprintf(stderr, "Invalid start node\n");
        free_graph(&graph);
        return 1;
    }

    bellman_ford(&graph, start_node);
    free_graph(&graph);

    return 0;
}