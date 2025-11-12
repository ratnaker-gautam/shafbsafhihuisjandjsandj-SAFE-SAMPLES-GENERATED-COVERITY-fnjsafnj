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

void bellman_ford(struct Graph *graph, int start) {
    if (graph == NULL || start < 0 || start >= graph->num_nodes) {
        return;
    }
    int distances[MAX_NODES];
    for (int i = 0; i < graph->num_nodes; i++) {
        *(distances + i) = 999999;
    }
    *(distances + start) = 0;

    for (int i = 0; i < graph->num_nodes - 1; i++) {
        for (int j = 0; j < graph->num_edges; j++) {
            struct Edge *edge_ptr = graph->edges + j;
            int u = edge_ptr->src;
            int v = edge_ptr->dest;
            int w = edge_ptr->weight;
            if (*(distances + u) != 999999 && *(distances + u) + w < *(distances + v)) {
                *(distances + v) = *(distances + u) + w;
            }
        }
    }

    for (int j = 0; j < graph->num_edges; j++) {
        struct Edge *edge_ptr = graph->edges + j;
        int u = edge_ptr->src;
        int v = edge_ptr->dest;
        int w = edge_ptr->weight;
        if (*(distances + u) != 999999 && *(distances + u) + w < *(distances + v)) {
            printf("Graph contains negative weight cycle\n");
            return;
        }
    }

    printf("Shortest distances from node %d:\n", start);
    for (int i = 0; i < graph->num_nodes; i++) {
        printf("Node %d: %d\n", i, *(distances + i));
    }
}

int main() {
    struct Graph graph;
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
    
    initialize_graph(&graph, nodes, edges);
    
    printf("Enter edges as 'source destination weight' (0-indexed):\n");
    for (int i = 0; i < edges; i++) {
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
    
    int start_node;
    printf("Enter starting node for Bellman-Ford: ");
    if (scanf("%d", &start_node) != 1 || start_node < 0 || start_node >= nodes) {
        printf("Invalid starting node\n");
        return 1;
    }
    
    bellman_ford(&graph, start_node);
    
    return 0;
}