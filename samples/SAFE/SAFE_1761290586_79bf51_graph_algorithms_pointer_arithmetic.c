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

void initialize_graph(struct Graph *graph, int nodes, int edges) {
    if (graph == NULL || nodes <= 0 || nodes > MAX_NODES || edges < 0 || edges > MAX_EDGES) {
        return;
    }
    graph->num_nodes = nodes;
    graph->num_edges = edges;
    for (int i = 0; i < edges; i++) {
        (graph->edges + i)->src = 0;
        (graph->edges + i)->dest = 0;
        (graph->edges + i)->weight = 0;
    }
}

int add_edge(struct Graph *graph, int src, int dest, int weight) {
    if (graph == NULL || src < 0 || src >= graph->num_nodes || dest < 0 || dest >= graph->num_nodes || weight < 0) {
        return 0;
    }
    if (graph->num_edges >= MAX_EDGES) {
        return 0;
    }
    struct Edge *edge_ptr = graph->edges + graph->num_edges;
    edge_ptr->src = src;
    edge_ptr->dest = dest;
    edge_ptr->weight = weight;
    graph->num_edges++;
    return 1;
}

void print_graph(struct Graph *graph) {
    if (graph == NULL) {
        return;
    }
    printf("Graph with %d nodes and %d edges:\n", graph->num_nodes, graph->num_edges);
    for (int i = 0; i < graph->num_edges; i++) {
        struct Edge *edge_ptr = graph->edges + i;
        printf("Edge %d: %d -> %d (weight %d)\n", i, edge_ptr->src, edge_ptr->dest, edge_ptr->weight);
    }
}

int find_shortest_path(struct Graph *graph, int start, int end) {
    if (graph == NULL || start < 0 || start >= graph->num_nodes || end < 0 || end >= graph->num_nodes) {
        return -1;
    }
    if (start == end) {
        return 0;
    }
    int distances[MAX_NODES];
    for (int i = 0; i < graph->num_nodes; i++) {
        *(distances + i) = -1;
    }
    *(distances + start) = 0;
    int updated = 1;
    for (int i = 0; i < graph->num_nodes - 1 && updated; i++) {
        updated = 0;
        for (int j = 0; j < graph->num_edges; j++) {
            struct Edge *edge_ptr = graph->edges + j;
            int u = edge_ptr->src;
            int v = edge_ptr->dest;
            int w = edge_ptr->weight;
            if (*(distances + u) != -1) {
                int new_dist = *(distances + u) + w;
                if (new_dist >= 0 && (*(distances + v) == -1 || new_dist < *(distances + v))) {
                    *(distances + v) = new_dist;
                    updated = 1;
                }
            }
        }
    }
    return *(distances + end);
}

int main() {
    struct Graph graph;
    int num_nodes, num_edges;
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &num_nodes) != 1 || num_nodes < 1 || num_nodes > MAX_NODES) {
        printf("Invalid number of nodes\n");
        return 1;
    }
    printf("Enter number of edges (0-%d): ", MAX_EDGES);
    if (scanf("%d", &num_edges) != 1 || num_edges < 0 || num_edges > MAX_EDGES) {
        printf("Invalid number of edges\n");
        return 1;
    }
    initialize_graph(&graph, num_nodes, num_edges);
    printf("Enter edges as 'source destination weight' (0-indexed nodes):\n");
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
    int distance = find_shortest_path(&graph, start, end);
    if (distance == -1) {
        printf("No path exists from %d to %d\n", start, end);
    } else {
        printf("Shortest path distance from %d to %d: %d\n", start, end, distance);
    }
    return 0;
}