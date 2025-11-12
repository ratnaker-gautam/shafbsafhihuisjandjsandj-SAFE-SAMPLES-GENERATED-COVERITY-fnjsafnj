//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_NODES 100
#define MAX_EDGES 1000

typedef struct {
    int from;
    int to;
    int weight;
} Edge;

typedef struct {
    int node_count;
    int edge_count;
    Edge edges[MAX_EDGES];
} Graph;

int read_graph(Graph *graph) {
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &graph->node_count) != 1) {
        return 0;
    }
    if (graph->node_count < 1 || graph->node_count > MAX_NODES) {
        return 0;
    }

    printf("Enter number of edges (0-%d): ", MAX_EDGES);
    if (scanf("%d", &graph->edge_count) != 1) {
        return 0;
    }
    if (graph->edge_count < 0 || graph->edge_count > MAX_EDGES) {
        return 0;
    }

    printf("Enter edges as 'from to weight' (nodes 0-%d):\n", graph->node_count - 1);
    for (int i = 0; i < graph->edge_count; i++) {
        if (scanf("%d %d %d", &graph->edges[i].from, &graph->edges[i].to, &graph->edges[i].weight) != 3) {
            return 0;
        }
        if (graph->edges[i].from < 0 || graph->edges[i].from >= graph->node_count ||
            graph->edges[i].to < 0 || graph->edges[i].to >= graph->node_count) {
            return 0;
        }
    }
    return 1;
}

void dijkstra(Graph *graph, int source, int distances[]) {
    int visited[MAX_NODES] = {0};
    
    for (int i = 0; i < graph->node_count; i++) {
        distances[i] = INT_MAX;
    }
    distances[source] = 0;
    
    for (int i = 0; i < graph->node_count; i++) {
        int min_dist = INT_MAX;
        int min_node = -1;
        
        for (int j = 0; j < graph->node_count; j++) {
            if (!visited[j] && distances[j] < min_dist) {
                min_dist = distances[j];
                min_node = j;
            }
        }
        
        if (min_node == -1) break;
        visited[min_node] = 1;
        
        for (int j = 0; j < graph->edge_count; j++) {
            if (graph->edges[j].from == min_node) {
                int neighbor = graph->edges[j].to;
                int new_dist;
                if (distances[min_node] == INT_MAX) {
                    new_dist = INT_MAX;
                } else {
                    new_dist = distances[min_node] + graph->edges[j].weight;
                    if (new_dist < distances[min_node]) {
                        new_dist = INT_MAX;
                    }
                }
                if (new_dist < distances[neighbor]) {
                    distances[neighbor] = new_dist;
                }
            }
        }
    }
}

void print_distances(int distances[], int node_count, int source) {
    printf("Shortest distances from node %d:\n", source);
    for (int i = 0; i < node_count; i++) {
        if (distances[i] == INT_MAX) {
            printf("Node %d: unreachable\n", i);
        } else {
            printf("Node %d: %d\n", i, distances[i]);
        }
    }
}

int main() {
    Graph graph;
    int distances[MAX_NODES];
    int source;
    
    printf("Graph Shortest Path Calculator\n");
    printf("==============================\n");
    
    if (!read_graph(&graph)) {
        printf("Error: Invalid input\n");
        return 1;
    }
    
    printf("Enter source node (0-%d): ", graph.node_count - 1);
    if (scanf("%d", &source) != 1) {
        printf("Error: Invalid source node\n");
        return 1;
    }
    if (source < 0 || source >= graph.node_count) {
        printf("Error: Invalid source node\n");
        return 1;
    }
    
    dijkstra(&graph, source, distances);
    print_distances(distances, graph.node_count, source);
    
    return 0;
}