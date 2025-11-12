//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: graph_algorithms
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

void initialize_graph(struct Graph *graph) {
    graph->num_nodes = 0;
    graph->num_edges = 0;
}

int add_edge(struct Graph *graph, int src, int dest, int weight) {
    if (graph->num_edges >= MAX_EDGES) {
        return 0;
    }
    if (src < 0 || src >= MAX_NODES || dest < 0 || dest >= MAX_NODES) {
        return 0;
    }
    if (weight < 0 || weight > 1000) {
        return 0;
    }
    
    graph->edges[graph->num_edges].src = src;
    graph->edges[graph->num_edges].dest = dest;
    graph->edges[graph->num_edges].weight = weight;
    graph->num_edges++;
    
    if (src >= graph->num_nodes) graph->num_nodes = src + 1;
    if (dest >= graph->num_nodes) graph->num_nodes = dest + 1;
    
    return 1;
}

void dijkstra(struct Graph *graph, int start_node, int distances[]) {
    int visited[MAX_NODES] = {0};
    
    for (int i = 0; i < graph->num_nodes; i++) {
        distances[i] = INT_MAX;
    }
    distances[start_node] = 0;
    
    for (int count = 0; count < graph->num_nodes - 1; count++) {
        int min_dist = INT_MAX;
        int min_node = -1;
        
        for (int i = 0; i < graph->num_nodes; i++) {
            if (!visited[i] && distances[i] < min_dist) {
                min_dist = distances[i];
                min_node = i;
            }
        }
        
        if (min_node == -1) break;
        visited[min_node] = 1;
        
        for (int i = 0; i < graph->num_edges; i++) {
            if (graph->edges[i].src == min_node) {
                int neighbor = graph->edges[i].dest;
                int new_dist = distances[min_node] + graph->edges[i].weight;
                
                if (new_dist > 0 && new_dist < distances[neighbor]) {
                    distances[neighbor] = new_dist;
                }
            }
        }
    }
}

int get_valid_int(const char *prompt, int min_val, int max_val) {
    char buffer[100];
    int value;
    
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
    struct Graph graph;
    initialize_graph(&graph);
    
    printf("Graph Shortest Path Calculator\n");
    printf("==============================\n\n");
    
    int num_edges = get_valid_int("Enter number of edges (1-100): ", 1, 100);
    if (num_edges == -1) {
        return 1;
    }
    
    printf("\nEnter edges as: source destination weight\n");
    printf("Nodes are numbered 0-99, weights 0-1000\n\n");
    
    for (int i = 0; i < num_edges; i++) {
        printf("Edge %d: ", i + 1);
        
        char buffer[100];
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return 1;
        }
        
        int src, dest, weight;
        if (sscanf(buffer, "%d %d %d", &src, &dest, &weight) != 3) {
            printf("Invalid format. Please try again.\n");
            i--;
            continue;
        }
        
        if (!add_edge(&graph, src, dest, weight)) {
            printf("Invalid edge parameters. Please try again.\n");
            i--;
            continue;
        }
    }
    
    int start_node = get_valid_int("\nEnter starting node: ", 0, graph.num_nodes - 1);
    if (start_node == -1) {
        return 1;
    }
    
    int distances[MAX_NODES];
    dijkstra(&graph, start_node, distances);
    
    printf("\nShortest distances from node %d:\n", start_node);
    printf("Node\tDistance\n");
    printf("----\t--------\n");
    
    for (int i = 0; i < graph.num_nodes; i++) {
        if (distances[i] == INT_MAX) {
            printf("%d\tUnreachable\n", i);
        } else {
            printf("%d\t%d\n", i, distances[i]);
        }
    }
    
    return 0;
}