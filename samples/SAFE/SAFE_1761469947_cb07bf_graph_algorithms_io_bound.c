//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

void initialize_graph(struct Graph *graph) {
    graph->num_nodes = 0;
    graph->num_edges = 0;
}

int add_edge(struct Graph *graph, int src, int dest, int weight) {
    if (graph->num_edges >= MAX_EDGES) return 0;
    if (src < 0 || src >= MAX_NODES) return 0;
    if (dest < 0 || dest >= MAX_NODES) return 0;
    if (weight < 0 || weight > 1000) return 0;
    
    graph->edges[graph->num_edges].src = src;
    graph->edges[graph->num_edges].dest = dest;
    graph->edges[graph->num_edges].weight = weight;
    graph->num_edges++;
    return 1;
}

void bellman_ford(struct Graph *graph, int start) {
    int dist[MAX_NODES];
    int i, j;
    
    for (i = 0; i < MAX_NODES; i++) {
        dist[i] = INT_MAX;
    }
    dist[start] = 0;
    
    for (i = 0; i < graph->num_nodes - 1; i++) {
        for (j = 0; j < graph->num_edges; j++) {
            int u = graph->edges[j].src;
            int v = graph->edges[j].dest;
            int w = graph->edges[j].weight;
            
            if (dist[u] != INT_MAX && dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
            }
        }
    }
    
    for (j = 0; j < graph->num_edges; j++) {
        int u = graph->edges[j].src;
        int v = graph->edges[j].dest;
        int w = graph->edges[j].weight;
        
        if (dist[u] != INT_MAX && dist[u] + w < dist[v]) {
            printf("Graph contains negative weight cycle\n");
            return;
        }
    }
    
    printf("Shortest distances from node %d:\n", start);
    for (i = 0; i < graph->num_nodes; i++) {
        if (dist[i] == INT_MAX) {
            printf("Node %d: unreachable\n", i);
        } else {
            printf("Node %d: %d\n", i, dist[i]);
        }
    }
}

int main() {
    struct Graph graph;
    initialize_graph(&graph);
    
    printf("Enter number of nodes (1-100): ");
    if (scanf("%d", &graph.num_nodes) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    if (graph.num_nodes < 1 || graph.num_nodes > MAX_NODES) {
        printf("Invalid number of nodes\n");
        return 1;
    }
    
    printf("Enter number of edges: ");
    if (scanf("%d", &graph.num_edges) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    if (graph.num_edges < 0 || graph.num_edges > MAX_EDGES) {
        printf("Invalid number of edges\n");
        return 1;
    }
    
    printf("Enter edges (source destination weight):\n");
    for (int i = 0; i < graph.num_edges; i++) {
        int src, dest, weight;
        if (scanf("%d %d %d", &src, &dest, &weight) != 3) {
            printf("Invalid edge input\n");
            return 1;
        }
        if (!add_edge(&graph, src, dest, weight)) {
            printf("Invalid edge parameters\n");
            return 1;
        }
    }
    
    int start_node;
    printf("Enter starting node: ");
    if (scanf("%d", &start_node) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    if (start_node < 0 || start_node >= graph.num_nodes) {
        printf("Invalid starting node\n");
        return 1;
    }
    
    bellman_ford(&graph, start_node);
    
    return 0;
}