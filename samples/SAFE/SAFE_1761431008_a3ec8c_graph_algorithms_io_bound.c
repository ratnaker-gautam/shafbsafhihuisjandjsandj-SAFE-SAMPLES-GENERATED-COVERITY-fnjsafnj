//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_NODES 100
#define MAX_EDGES 1000

struct Graph {
    int num_nodes;
    int num_edges;
    int adjacency[MAX_NODES][MAX_NODES];
    int edge_weights[MAX_NODES][MAX_NODES];
};

struct Graph* create_graph(int nodes) {
    if (nodes <= 0 || nodes > MAX_NODES) {
        return NULL;
    }
    
    struct Graph* graph = malloc(sizeof(struct Graph));
    if (graph == NULL) {
        return NULL;
    }
    
    graph->num_nodes = nodes;
    graph->num_edges = 0;
    
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            graph->adjacency[i][j] = 0;
            graph->edge_weights[i][j] = 0;
        }
    }
    
    return graph;
}

int add_edge(struct Graph* graph, int from, int to, int weight) {
    if (graph == NULL) return 0;
    if (from < 0 || from >= graph->num_nodes) return 0;
    if (to < 0 || to >= graph->num_nodes) return 0;
    if (from == to) return 0;
    if (weight < 0) return 0;
    
    if (graph->adjacency[from][to] == 0) {
        graph->num_edges++;
    }
    
    graph->adjacency[from][to] = 1;
    graph->edge_weights[from][to] = weight;
    return 1;
}

void dijkstra(struct Graph* graph, int source, int distances[]) {
    if (graph == NULL || distances == NULL) return;
    if (source < 0 || source >= graph->num_nodes) return;
    
    int visited[MAX_NODES];
    
    for (int i = 0; i < graph->num_nodes; i++) {
        distances[i] = INT_MAX;
        visited[i] = 0;
    }
    distances[source] = 0;
    
    for (int count = 0; count < graph->num_nodes - 1; count++) {
        int min_distance = INT_MAX;
        int min_node = -1;
        
        for (int v = 0; v < graph->num_nodes; v++) {
            if (!visited[v] && distances[v] <= min_distance) {
                min_distance = distances[v];
                min_node = v;
            }
        }
        
        if (min_node == -1) break;
        visited[min_node] = 1;
        
        for (int v = 0; v < graph->num_nodes; v++) {
            if (!visited[v] && graph->adjacency[min_node][v] && 
                distances[min_node] != INT_MAX) {
                int new_distance = distances[min_node] + graph->edge_weights[min_node][v];
                if (new_distance < distances[v]) {
                    distances[v] = new_distance;
                }
            }
        }
    }
}

void print_shortest_paths(struct Graph* graph, int source) {
    if (graph == NULL) return;
    if (source < 0 || source >= graph->num_nodes) return;
    
    int distances[MAX_NODES];
    dijkstra(graph, source, distances);
    
    printf("Shortest paths from node %d:\n", source);
    for (int i = 0; i < graph->num_nodes; i++) {
        if (distances[i] == INT_MAX) {
            printf("Node %d: unreachable\n", i);
        } else {
            printf("Node %d: distance %d\n", i, distances[i]);
        }
    }
}

int main() {
    struct Graph* graph = create_graph(6);
    if (graph == NULL) {
        printf("Failed to create graph\n");
        return 1;
    }
    
    add_edge(graph, 0, 1, 4);
    add_edge(graph, 0, 2, 2);
    add_edge(graph, 1, 2, 1);
    add_edge(graph, 1, 3, 5);
    add_edge(graph, 2, 3, 8);
    add_edge(graph, 2, 4, 10);
    add_edge(graph, 3, 4, 2);
    add_edge(graph, 3, 5, 6);
    add_edge(graph, 4, 5, 3);
    
    printf("Graph created with %d nodes and %d edges\n", graph->num_nodes, graph->num_edges);
    
    int source_node;
    printf("Enter source node (0-%d): ", graph->num_nodes - 1);
    
    if (scanf("%d", &source_node) != 1) {
        printf("Invalid input\n");
        free(graph);
        return 1;
    }
    
    if (source_node < 0 || source_node >= graph->num_nodes) {
        printf("Invalid node number\n");
        free(graph);
        return 1;
    }
    
    print_shortest_paths(graph, source_node);
    
    free(graph);
    return 0;
}