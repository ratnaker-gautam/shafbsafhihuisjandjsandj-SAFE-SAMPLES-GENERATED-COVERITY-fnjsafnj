//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
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

void init_graph(struct Graph* graph, int nodes, int edges) {
    if (graph == NULL || nodes <= 0 || nodes > MAX_NODES || edges < 0 || edges > MAX_EDGES) {
        return;
    }
    graph->num_nodes = nodes;
    graph->num_edges = edges;
}

int add_edge(struct Graph* graph, int idx, int src, int dest, int weight) {
    if (graph == NULL || idx < 0 || idx >= graph->num_edges) {
        return 0;
    }
    if (src < 0 || src >= graph->num_nodes || dest < 0 || dest >= graph->num_nodes) {
        return 0;
    }
    if (weight < 0 || weight > 1000) {
        return 0;
    }
    graph->edges[idx].src = src;
    graph->edges[idx].dest = dest;
    graph->edges[idx].weight = weight;
    return 1;
}

void dijkstra(struct Graph* graph, int start_node, int* distances) {
    if (graph == NULL || distances == NULL) {
        return;
    }
    if (start_node < 0 || start_node >= graph->num_nodes) {
        return;
    }
    
    int visited[MAX_NODES];
    for (int i = 0; i < graph->num_nodes; i++) {
        distances[i] = INT_MAX;
        visited[i] = 0;
    }
    distances[start_node] = 0;
    
    for (int count = 0; count < graph->num_nodes - 1; count++) {
        int min_dist = INT_MAX;
        int min_node = -1;
        
        for (int v = 0; v < graph->num_nodes; v++) {
            if (!visited[v] && distances[v] <= min_dist) {
                min_dist = distances[v];
                min_node = v;
            }
        }
        
        if (min_node == -1) {
            break;
        }
        
        visited[min_node] = 1;
        
        for (int i = 0; i < graph->num_edges; i++) {
            if (graph->edges[i].src == min_node) {
                int neighbor = graph->edges[i].dest;
                int new_dist;
                if (distances[min_node] < INT_MAX - graph->edges[i].weight) {
                    new_dist = distances[min_node] + graph->edges[i].weight;
                } else {
                    new_dist = INT_MAX;
                }
                if (!visited[neighbor] && new_dist < distances[neighbor]) {
                    distances[neighbor] = new_dist;
                }
            }
        }
    }
}

void print_distances(int* distances, int num_nodes, int start_node) {
    if (distances == NULL || num_nodes <= 0 || num_nodes > MAX_NODES) {
        return;
    }
    if (start_node < 0 || start_node >= num_nodes) {
        return;
    }
    
    printf("Shortest distances from node %d:\n", start_node);
    for (int i = 0; i < num_nodes; i++) {
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
            return min_val;
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
    struct Graph graph;
    int distances[MAX_NODES];
    
    printf("Graph Shortest Path Calculator\n");
    printf("==============================\n");
    
    int num_nodes = get_valid_int("Enter number of nodes (1-100): ", 1, MAX_NODES);
    int num_edges = get_valid_int("Enter number of edges (0-1000): ", 0, MAX_EDGES);
    
    init_graph(&graph, num_nodes, num_edges);
    
    printf("\nEnter edge information (source destination weight):\n");
    for (int i = 0; i < num_edges; i++) {
        printf("Edge %d: ", i);
        
        int src, dest, weight;
        if (scanf("%d %d %d", &src, &dest, &weight) != 3) {
            printf("Invalid input. Using default values.\n");
            src = 0;
            dest = 0;
            weight = 1;
            while (getchar() != '\n');
        } else {
            getchar();
        }
        
        if (!add_edge(&graph, i, src, dest, weight)) {