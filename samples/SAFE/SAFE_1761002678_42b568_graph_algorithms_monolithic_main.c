//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: monolithic_main ; Variation: graph_algorithms
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
}

int validate_node(int node, int max_nodes) {
    return node >= 0 && node < max_nodes;
}

void bellman_ford(struct Graph* graph, int source) {
    if (!validate_node(source, graph->num_nodes)) {
        fprintf(stderr, "Invalid source node\n");
        return;
    }

    int distances[MAX_NODES];
    for (int i = 0; i < graph->num_nodes; i++) {
        distances[i] = INT_MAX;
    }
    distances[source] = 0;

    for (int i = 1; i <= graph->num_nodes - 1; i++) {
        for (int j = 0; j < graph->num_edges; j++) {
            int u = graph->edges[j].src;
            int v = graph->edges[j].dest;
            int weight = graph->edges[j].weight;
            
            if (validate_node(u, graph->num_nodes) && validate_node(v, graph->num_nodes)) {
                if (distances[u] != INT_MAX && distances[u] + weight < distances[v]) {
                    distances[v] = distances[u] + weight;
                }
            }
        }
    }

    for (int i = 0; i < graph->num_edges; i++) {
        int u = graph->edges[i].src;
        int v = graph->edges[i].dest;
        int weight = graph->edges[i].weight;
        
        if (validate_node(u, graph->num_nodes) && validate_node(v, graph->num_nodes)) {
            if (distances[u] != INT_MAX && distances[u] + weight < distances[v]) {
                fprintf(stderr, "Graph contains negative weight cycle\n");
                return;
            }
        }
    }

    printf("Shortest distances from node %d:\n", source);
    for (int i = 0; i < graph->num_nodes; i++) {
        if (distances[i] == INT_MAX) {
            printf("Node %d: unreachable\n", i);
        } else {
            printf("Node %d: %d\n", i, distances[i]);
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
        if (!validate_node(src, num_nodes) || !validate_node(dest, num_nodes)) {
            fprintf(stderr, "Invalid node in edge\n");
            return 1;
        }
        graph.edges[i].src = src;
        graph.edges[i].dest = dest;
        graph.edges[i].weight = weight;
    }
    
    int source;
    printf("Enter source node: ");
    if (scanf("%d", &source) != 1 || !validate_node(source, num_nodes)) {
        fprintf(stderr, "Invalid source node\n");
        return 1;
    }
    
    bellman_ford(&graph, source);
    
    return 0;
}