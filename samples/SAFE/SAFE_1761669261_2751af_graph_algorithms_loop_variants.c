//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: graph_algorithms
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
    if (graph == NULL || nodes <= 0 || nodes > MAX_NODES || edges < 0 || edges > MAX_EDGES) {
        return;
    }
    graph->num_nodes = nodes;
    graph->num_edges = edges;
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

int add_edge(struct Graph* graph, int index, int src, int dest, int weight) {
    if (graph == NULL || index < 0 || index >= graph->num_edges) {
        return 0;
    }
    if (!validate_edge(src, dest, weight, graph->num_nodes)) {
        return 0;
    }
    graph->edges[index].src = src;
    graph->edges[index].dest = dest;
    graph->edges[index].weight = weight;
    return 1;
}

void bellman_ford(struct Graph* graph, int source) {
    if (graph == NULL || source < 0 || source >= graph->num_nodes) {
        return;
    }
    
    int distances[MAX_NODES];
    int i, j, u, v, weight;
    
    for (i = 0; i < graph->num_nodes; i++) {
        distances[i] = INT_MAX;
    }
    distances[source] = 0;
    
    for (i = 1; i <= graph->num_nodes - 1; i++) {
        for (j = 0; j < graph->num_edges; j++) {
            u = graph->edges[j].src;
            v = graph->edges[j].dest;
            weight = graph->edges[j].weight;
            
            if (distances[u] != INT_MAX && distances[u] + weight < distances[v]) {
                distances[v] = distances[u] + weight;
            }
        }
    }
    
    for (j = 0; j < graph->num_edges; j++) {
        u = graph->edges[j].src;
        v = graph->edges[j].dest;
        weight = graph->edges[j].weight;
        
        if (distances[u] != INT_MAX && distances[u] + weight < distances[v]) {
            printf("Graph contains negative weight cycle\n");
            return;
        }
    }
    
    printf("Shortest distances from node %d:\n", source);
    for (i = 0; i < graph->num_nodes; i++) {
        if (distances[i] == INT_MAX) {
            printf("Node %d: INFINITY\n", i);
        } else {
            printf("Node %d: %d\n", i, distances[i]);
        }
    }
}

int main() {
    struct Graph graph;
    int num_nodes, num_edges;
    int i, src, dest, weight;
    
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
    
    initialize_graph(&graph, num_nodes, num_edges);
    
    printf("Enter edges (source destination weight):\n");
    for (i = 0; i < num_edges; i++) {
        if (scanf("%d %d %d", &src, &dest, &weight) != 3) {
            printf("Invalid edge input\n");
            return 1;
        }
        if (!add_edge(&graph, i, src, dest, weight)) {
            printf("Invalid edge parameters\n");
            return 1;
        }
    }
    
    int source;
    printf("Enter source node (0-%d): ", num_nodes - 1);
    if (scanf("%d", &source) != 1 || source < 0 || source >= num_nodes) {
        printf("Invalid source node\n");
        return 1;
    }
    
    bellman_ford(&graph, source);
    
    return 0;
}