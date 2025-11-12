//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
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

void initialize_graph(struct Graph* graph, int nodes, int edges) {
    if (nodes <= 0 || nodes > MAX_NODES || edges < 0 || edges > MAX_EDGES) {
        fprintf(stderr, "Invalid graph parameters\n");
        exit(1);
    }
    graph->num_nodes = nodes;
    graph->num_edges = edges;
    for (int i = 0; i < edges; i++) {
        graph->edges[i].src = -1;
        graph->edges[i].dest = -1;
        graph->edges[i].weight = 0;
    }
}

int validate_edge(int src, int dest, int weight, int num_nodes) {
    if (src < 0 || src >= num_nodes || dest < 0 || dest >= num_nodes) {
        return 0;
    }
    if (weight < -1000 || weight > 1000) {
        return 0;
    }
    return 1;
}

int add_edge(struct Graph* graph, int idx, int src, int dest, int weight) {
    if (idx < 0 || idx >= graph->num_edges) {
        return 0;
    }
    if (!validate_edge(src, dest, weight, graph->num_nodes)) {
        return 0;
    }
    graph->edges[idx].src = src;
    graph->edges[idx].dest = dest;
    graph->edges[idx].weight = weight;
    return 1;
}

void bellman_ford(struct Graph* graph, int start) {
    if (start < 0 || start >= graph->num_nodes) {
        fprintf(stderr, "Invalid start node\n");
        return;
    }

    int dist[MAX_NODES];
    for (int i = 0; i < graph->num_nodes; i++) {
        dist[i] = INT_MAX;
    }
    dist[start] = 0;

    for (int i = 0; i < graph->num_nodes - 1; i++) {
        for (int j = 0; j < graph->num_edges; j++) {
            int u = graph->edges[j].src;
            int v = graph->edges[j].dest;
            int w = graph->edges[j].weight;
            if (dist[u] != INT_MAX && dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
            }
        }
    }

    for (int j = 0; j < graph->num_edges; j++) {
        int u = graph->edges[j].src;
        int v = graph->edges[j].dest;
        int w = graph->edges[j].weight;
        if (dist[u] != INT_MAX && dist[u] + w < dist[v]) {
            fprintf(stderr, "Graph contains negative weight cycle\n");
            return;
        }
    }

    printf("Shortest distances from node %d:\n", start);
    for (int i = 0; i < graph->num_nodes; i++) {
        if (dist[i] == INT_MAX) {
            printf("Node %d: unreachable\n", i);
        } else {
            printf("Node %d: %d\n", i, dist[i]);
        }
    }
}

int main() {
    struct Graph graph;
    int num_nodes, num_edges;

    printf("Enter number of nodes (1-100): ");
    if (scanf("%d", &num_nodes) != 1 || num_nodes < 1 || num_nodes > MAX_NODES) {
        fprintf(stderr, "Invalid number of nodes\n");
        return 1;
    }

    printf("Enter number of edges (0-1000): ");
    if (scanf("%d", &num_edges) != 1 || num_edges < 0 || num_edges > MAX_EDGES) {
        fprintf(stderr, "Invalid number of edges\n");
        return 1;
    }

    initialize_graph(&graph, num_nodes, num_edges);

    printf("Enter edges as 'source destination weight':\n");
    for (int i = 0; i < num_edges; i++) {
        int src, dest, weight;
        if (scanf("%d %d %d", &src, &dest, &weight) != 3) {
            fprintf(stderr, "Invalid edge input\n");
            return 1;
        }
        if (!add_edge(&graph, i, src, dest, weight)) {
            fprintf(stderr, "Invalid edge parameters\n");
            return 1;
        }
    }

    int start_node;
    printf("Enter start node for Bellman-Ford: ");
    if (scanf("%d", &start_node) != 1 || start_node < 0 || start_node >= num_nodes) {
        fprintf(stderr, "Invalid start node\n");
        return 1;
    }

    bellman_ford(&graph, start_node);

    return 0;
}