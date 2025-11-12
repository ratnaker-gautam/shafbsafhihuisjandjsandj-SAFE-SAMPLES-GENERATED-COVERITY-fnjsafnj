//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: modular_functions ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_NODES 100
#define MAX_EDGES 1000

struct Graph {
    int nodes;
    int edges[MAX_EDGES][3];
    int edge_count;
};

struct Graph* create_graph(int nodes) {
    if (nodes <= 0 || nodes > MAX_NODES) {
        return NULL;
    }
    struct Graph* graph = malloc(sizeof(struct Graph));
    if (graph == NULL) {
        return NULL;
    }
    graph->nodes = nodes;
    graph->edge_count = 0;
    return graph;
}

int add_edge(struct Graph* graph, int from, int to, int weight) {
    if (graph == NULL || from < 0 || from >= graph->nodes || 
        to < 0 || to >= graph->nodes || graph->edge_count >= MAX_EDGES) {
        return 0;
    }
    graph->edges[graph->edge_count][0] = from;
    graph->edges[graph->edge_count][1] = to;
    graph->edges[graph->edge_count][2] = weight;
    graph->edge_count++;
    return 1;
}

void dijkstra(struct Graph* graph, int source, int* distances) {
    if (graph == NULL || distances == NULL || 
        source < 0 || source >= graph->nodes) {
        return;
    }
    
    int visited[MAX_NODES] = {0};
    
    for (int i = 0; i < graph->nodes; i++) {
        distances[i] = INT_MAX;
    }
    distances[source] = 0;
    
    for (int count = 0; count < graph->nodes - 1; count++) {
        int min_dist = INT_MAX;
        int min_node = -1;
        
        for (int v = 0; v < graph->nodes; v++) {
            if (!visited[v] && distances[v] <= min_dist) {
                min_dist = distances[v];
                min_node = v;
            }
        }
        
        if (min_node == -1) break;
        visited[min_node] = 1;
        
        for (int i = 0; i < graph->edge_count; i++) {
            if (graph->edges[i][0] == min_node) {
                int neighbor = graph->edges[i][1];
                int weight = graph->edges[i][2];
                if (!visited[neighbor] && distances[min_node] != INT_MAX && 
                    distances[min_node] + weight < distances[neighbor]) {
                    distances[neighbor] = distances[min_node] + weight;
                }
            }
        }
    }
}

void print_distances(int* distances, int nodes, int source) {
    if (distances == NULL || nodes <= 0 || source < 0 || source >= nodes) {
        return;
    }
    printf("Shortest distances from node %d:\n", source);
    for (int i = 0; i < nodes; i++) {
        if (distances[i] == INT_MAX) {
            printf("Node %d: unreachable\n", i);
        } else {
            printf("Node %d: %d\n", i, distances[i]);
        }
    }
}

int get_valid_int(const char* prompt, int min_val, int max_val) {
    int value;
    char buffer[100];
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            continue;
        }
        
        if (sscanf(buffer, "%d", &value) != 1) {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        if (value < min_val || value > max_val) {
            printf("Value must be between %d and %d.\n", min_val, max_val);
            continue;
        }
        
        return value;
    }
}

int main() {
    printf("Graph Shortest Path Calculator\n");
    printf("==============================\n\n");
    
    int nodes = get_valid_int("Enter number of nodes (1-100): ", 1, MAX_NODES);
    
    struct Graph* graph = create_graph(nodes);
    if (graph == NULL) {
        printf("Failed to create graph.\n");
        return 1;
    }
    
    printf("\nEnter edges (from to weight). Enter -1 -1 -1 to stop.\n");
    
    while (1) {
        int from = get_valid_int("From node (0 to %d, -1 to stop): ", -1, nodes-1);
        if (from == -1) break;
        
        int to = get_valid_int("To node (0 to %d): ", 0, nodes-1);
        int weight = get_valid_int("Weight (1-1000): ", 1, 1000);
        
        if (!add_edge(graph, from, to, weight)) {
            printf("Failed to add edge. Maximum edges reached or invalid nodes.\n");
            break;
        }
        
        printf("Edge added: %d -> %d (weight %d)\n", from, to, weight);
        
        if (graph->edge_count >= MAX_EDGES) {
            printf("Maximum number of edges reached.\n");
            break;
        }
    }
    
    printf("\n");
    int source = get_valid_int("Enter source node for shortest paths (0 to %d): ", 0, nodes-1);
    
    int distances[MAX_N