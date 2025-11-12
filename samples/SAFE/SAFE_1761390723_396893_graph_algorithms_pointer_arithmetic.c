//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_NODES 100
#define MAX_EDGES 500

struct Graph {
    int num_nodes;
    int num_edges;
    int *adj_matrix;
};

struct Graph* create_graph(int nodes) {
    if (nodes <= 0 || nodes > MAX_NODES) return NULL;
    
    struct Graph *graph = malloc(sizeof(struct Graph));
    if (!graph) return NULL;
    
    graph->num_nodes = nodes;
    graph->num_edges = 0;
    
    size_t matrix_size = (size_t)nodes * (size_t)nodes * sizeof(int);
    if (matrix_size > SIZE_MAX / sizeof(int)) {
        free(graph);
        return NULL;
    }
    
    graph->adj_matrix = calloc((size_t)nodes * (size_t)nodes, sizeof(int));
    if (!graph->adj_matrix) {
        free(graph);
        return NULL;
    }
    
    return graph;
}

void destroy_graph(struct Graph *graph) {
    if (graph) {
        free(graph->adj_matrix);
        free(graph);
    }
}

int add_edge(struct Graph *graph, int from, int to, int weight) {
    if (!graph || from < 0 || to < 0 || from >= graph->num_nodes || to >= graph->num_nodes) return 0;
    if (weight < 0 || weight > 1000) return 0;
    
    int *matrix = graph->adj_matrix;
    *(matrix + from * graph->num_nodes + to) = weight;
    *(matrix + to * graph->num_nodes + from) = weight;
    
    graph->num_edges++;
    return 1;
}

void dijkstra(struct Graph *graph, int start) {
    if (!graph || start < 0 || start >= graph->num_nodes) return;
    
    int dist[MAX_NODES];
    int visited[MAX_NODES];
    
    for (int i = 0; i < graph->num_nodes; i++) {
        dist[i] = INT_MAX;
        visited[i] = 0;
    }
    dist[start] = 0;
    
    for (int count = 0; count < graph->num_nodes - 1; count++) {
        int min_dist = INT_MAX;
        int min_node = -1;
        
        for (int v = 0; v < graph->num_nodes; v++) {
            if (!visited[v] && dist[v] <= min_dist) {
                min_dist = dist[v];
                min_node = v;
            }
        }
        
        if (min_node == -1) break;
        visited[min_node] = 1;
        
        int *matrix = graph->adj_matrix;
        for (int v = 0; v < graph->num_nodes; v++) {
            if (!visited[v] && *(matrix + min_node * graph->num_nodes + v) != 0) {
                int edge_weight = *(matrix + min_node * graph->num_nodes + v);
                if (dist[min_node] != INT_MAX && dist[min_node] + edge_weight < dist[v]) {
                    dist[v] = dist[min_node] + edge_weight;
                }
            }
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
    printf("Graph Algorithm Demo - Dijkstra's Shortest Path\n");
    printf("Creating graph with 6 nodes...\n");
    
    struct Graph *graph = create_graph(6);
    if (!graph) {
        printf("Failed to create graph\n");
        return 1;
    }
    
    int edges[][3] = {
        {0, 1, 4}, {0, 2, 2}, {1, 2, 1}, {1, 3, 5},
        {2, 3, 8}, {2, 4, 10}, {3, 4, 2}, {3, 5, 6},
        {4, 5, 3}
    };
    
    int num_edges = sizeof(edges) / sizeof(edges[0]);
    
    for (int i = 0; i < num_edges; i++) {
        if (!add_edge(graph, edges[i][0], edges[i][1], edges[i][2])) {
            printf("Failed to add edge %d-%d\n", edges[i][0], edges[i][1]);
        }
    }
    
    printf("Graph created with %d nodes and %d edges\n", graph->num_nodes, graph->num_edges);
    
    printf("\nRunning Dijkstra's algorithm from node 0:\n");
    dijkstra(graph, 0);
    
    printf("\nRunning Dijkstra's algorithm from node 3:\n");
    dijkstra(graph, 3);
    
    destroy_graph(graph);
    return 0;
}