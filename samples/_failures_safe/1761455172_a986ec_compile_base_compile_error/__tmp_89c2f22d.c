//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_NODES 100
#define MAX_EDGES 1000

struct Graph {
    int num_nodes;
    int num_edges;
    int adjacency[MAX_NODES][MAX_NODES];
    int edge_weights[MAX_NODES][MAX_NODES];
};

struct Graph* create_graph(int nodes, int edges) {
    if (nodes <= 0 || nodes > MAX_NODES || edges < 0 || edges > MAX_EDGES) {
        return NULL;
    }
    
    struct Graph* graph = malloc(sizeof(struct Graph));
    if (graph == NULL) {
        return NULL;
    }
    
    graph->num_nodes = nodes;
    graph->num_edges = edges;
    
    for (int i = 0; i < MAX_NODES; i++) {
        for (int j = 0; j < MAX_NODES; j++) {
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
    
    graph->adjacency[from][to] = 1;
    graph->edge_weights[from][to] = weight;
    return 1;
}

void dijkstra(struct Graph* graph, int source, int* distances) {
    if (graph == NULL || distances == NULL) return;
    if (source < 0 || source >= graph->num_nodes) return;
    
    int visited[MAX_NODES] = {0};
    
    for (int i = 0; i < graph->num_nodes; i++) {
        distances[i] = INT_MAX;
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
                long new_dist = (long)distances[min_node] + graph->edge_weights[min_node][v];
                if (new_dist < distances[v] && new_dist <= INT_MAX) {
                    distances[v] = (int)new_dist;
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

int validate_input_int(const char* prompt, int min_val, int max_val) {
    if (prompt == NULL) return -1;
    
    int value;
    char buffer[100];
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
        }
        
        if (sscanf(buffer, "%d", &value) == 1) {
            if (value >= min_val && value <= max_val) {
                return value;
            }
        }
        printf("Invalid input. Please enter a number between %d and %d.\n", min_val, max_val);
    }
}

int main() {
    printf("Graph Shortest Path Calculator\n");
    printf("==============================\n\n");
    
    int num_nodes = validate_input_int("Enter number of nodes (1-100): ", 1, MAX_NODES);
    if (num_nodes == -1) {
        printf("Input error.\n");
        return 1;
    }
    
    int num_edges = validate_input_int("Enter number of edges (0-1000): ", 0, MAX_EDGES);
    if (num_edges == -1) {
        printf("Input error.\n");
        return 1;
    }
    
    struct Graph* graph = create_graph(num_nodes, num_edges);
    if (graph == NULL) {
        printf("Failed to create graph.\n");
        return 1;
    }
    
    printf("\nEnter edges (from to weight):\n");
    for (int i = 0; i < num_edges; i++) {
        printf("Edge %d: ", i + 1);
        
        int