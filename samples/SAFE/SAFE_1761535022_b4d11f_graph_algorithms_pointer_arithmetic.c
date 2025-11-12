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

void initialize_graph(struct Graph *graph, int nodes, int edges) {
    if (graph == NULL || nodes <= 0 || nodes > MAX_NODES || edges < 0 || edges > MAX_EDGES) {
        return;
    }
    graph->num_nodes = nodes;
    graph->num_edges = edges;
    for (int i = 0; i < edges; i++) {
        (graph->edges + i)->src = -1;
        (graph->edges + i)->dest = -1;
        (graph->edges + i)->weight = 0;
    }
}

int add_edge(struct Graph *graph, int src, int dest, int weight) {
    if (graph == NULL || src < 0 || src >= graph->num_nodes || 
        dest < 0 || dest >= graph->num_nodes || weight < 0) {
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

void dijkstra(struct Graph *graph, int start_node, int *distances) {
    if (graph == NULL || distances == NULL || start_node < 0 || start_node >= graph->num_nodes) {
        return;
    }
    
    int visited[MAX_NODES];
    for (int i = 0; i < graph->num_nodes; i++) {
        *(distances + i) = INT_MAX;
        *(visited + i) = 0;
    }
    *(distances + start_node) = 0;
    
    for (int count = 0; count < graph->num_nodes - 1; count++) {
        int min_distance = INT_MAX;
        int min_index = -1;
        
        for (int v = 0; v < graph->num_nodes; v++) {
            if (!*(visited + v) && *(distances + v) <= min_distance) {
                min_distance = *(distances + v);
                min_index = v;
            }
        }
        
        if (min_index == -1) break;
        *(visited + min_index) = 1;
        
        for (int i = 0; i < graph->num_edges; i++) {
            struct Edge *edge_ptr = graph->edges + i;
            if (edge_ptr->src == min_index) {
                int v = edge_ptr->dest;
                int weight = edge_ptr->weight;
                if (!*(visited + v) && *(distances + min_index) != INT_MAX && 
                    *(distances + min_index) + weight < *(distances + v)) {
                    *(distances + v) = *(distances + min_index) + weight;
                }
            }
        }
    }
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
    
    initialize_graph(&graph, num_nodes, num_edges);
    
    printf("Enter edges as: source destination weight\n");
    for (int i = 0; i < num_edges; i++) {
        int src, dest, weight;
        printf("Edge %d: ", i + 1);
        if (scanf("%d %d %d", &src, &dest, &weight) != 3) {
            printf("Invalid edge input\n");
            return 1;
        }
        if (!add_edge(&graph, src, dest, weight)) {
            printf("Failed to add edge\n");
            return 1;
        }
    }
    
    int start_node;
    printf("Enter starting node for Dijkstra: ");
    if (scanf("%d", &start_node) != 1 || start_node < 0 || start_node >= num_nodes) {
        printf("Invalid starting node\n");
        return 1;
    }
    
    int distances[MAX_NODES];
    dijkstra(&graph, start_node, distances);
    
    printf("\nShortest distances from node %d:\n", start_node);
    for (int i = 0; i < num_nodes; i++) {
        if (*(distances + i) == INT_MAX) {
            printf("Node %d: unreachable\n", i);
        } else {
            printf("Node %d: %d\n", i, *(distances + i));
        }
    }
    
    return 0;
}