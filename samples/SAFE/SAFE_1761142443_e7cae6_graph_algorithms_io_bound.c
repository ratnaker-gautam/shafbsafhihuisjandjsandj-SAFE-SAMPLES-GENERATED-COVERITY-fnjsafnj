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

void dijkstra(struct Graph *graph, int start_node) {
    if (start_node < 0 || start_node >= graph->num_nodes) return;
    
    int dist[MAX_NODES];
    int visited[MAX_NODES];
    
    for (int i = 0; i < graph->num_nodes; i++) {
        dist[i] = INT_MAX;
        visited[i] = 0;
    }
    dist[start_node] = 0;
    
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
        
        for (int i = 0; i < graph->num_edges; i++) {
            if (graph->edges[i].src == min_node) {
                int neighbor = graph->edges[i].dest;
                int weight = graph->edges[i].weight;
                
                if (!visited[neighbor] && dist[min_node] != INT_MAX && 
                    dist[min_node] + weight < dist[neighbor]) {
                    dist[neighbor] = dist[min_node] + weight;
                }
            }
        }
    }
    
    printf("Shortest distances from node %d:\n", start_node);
    for (int i = 0; i < graph->num_nodes; i++) {
        if (dist[i] == INT_MAX) {
            printf("Node %d: unreachable\n", i);
        } else {
            printf("Node %d: %d\n", i, dist[i]);
        }
    }
}

int read_int(const char *prompt, int min, int max) {
    int value;
    char buffer[100];
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
        }
        
        if (sscanf(buffer, "%d", &value) == 1 && value >= min && value <= max) {
            return value;
        }
        printf("Invalid input. Please enter an integer between %d and %d.\n", min, max);
    }
}

int main() {
    struct Graph graph;
    initialize_graph(&graph);
    
    printf("Graph Shortest Path Calculator\n");
    printf("==============================\n\n");
    
    graph.num_nodes = read_int("Enter number of nodes (1-100): ", 1, MAX_NODES);
    if (graph.num_nodes == -1) return 1;
    
    int num_edges = read_int("Enter number of edges (0-1000): ", 0, MAX_EDGES);
    if (num_edges == -1) return 1;
    
    printf("\nEnter edges (source destination weight):\n");
    for (int i = 0; i < num_edges; i++) {
        printf("Edge %d: ", i + 1);
        
        int src, dest, weight;
        char buffer[100];
        
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) break;
        
        if (sscanf(buffer, "%d %d %d", &src, &dest, &weight) != 3) {
            printf("Invalid format. Please enter: source destination weight\n");
            i--;
            continue;
        }
        
        if (!add_edge(&graph, src, dest, weight)) {
            printf("Invalid edge parameters. Source and destination must be 0-%d, weight 0-1000\n", graph.num_nodes - 1);
            i--;
        }
    }
    
    printf("\n");
    int start_node = read_int("Enter starting node for shortest path: ", 0, graph.num_nodes - 1);
    if (start_node == -1) return 1;
    
    printf("\n");
    dijkstra(&graph, start_node);
    
    return 0;
}