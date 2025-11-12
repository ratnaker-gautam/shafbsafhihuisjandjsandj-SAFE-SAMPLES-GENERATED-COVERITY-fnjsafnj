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

int add_node(struct Graph *graph) {
    if (graph->num_nodes >= MAX_NODES) {
        return -1;
    }
    return graph->num_nodes++;
}

int add_edge(struct Graph *graph, int src, int dest, int weight) {
    if (graph->num_edges >= MAX_EDGES) {
        return 0;
    }
    if (src < 0 || src >= graph->num_nodes || dest < 0 || dest >= graph->num_nodes) {
        return 0;
    }
    if (weight < 0) {
        return 0;
    }
    graph->edges[graph->num_edges].src = src;
    graph->edges[graph->num_edges].dest = dest;
    graph->edges[graph->num_edges].weight = weight;
    graph->num_edges++;
    return 1;
}

void dijkstra(struct Graph *graph, int start_node) {
    if (start_node < 0 || start_node >= graph->num_nodes) {
        printf("Invalid start node\n");
        return;
    }

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

        if (min_node == -1) {
            break;
        }

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

int read_int(const char *prompt, int min_val, int max_val) {
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
        printf("Invalid input. Please enter an integer between %d and %d.\n", min_val, max_val);
    }
}

int main() {
    struct Graph graph;
    initialize_graph(&graph);
    
    printf("Graph Shortest Path Calculator\n");
    printf("==============================\n\n");
    
    int num_nodes = read_int("Enter number of nodes (1-100): ", 1, MAX_NODES);
    if (num_nodes == -1) {
        return 1;
    }
    
    for (int i = 0; i < num_nodes; i++) {
        add_node(&graph);
    }
    
    printf("\nEnter edges (source destination weight). Enter -1 -1 -1 to finish.\n");
    
    while (1) {
        int src = read_int("Source node (-1 to finish): ", -1, num_nodes - 1);
        if (src == -1) {
            break;
        }
        
        int dest = read_int("Destination node: ", 0, num_nodes - 1);
        int weight = read_int("Weight (0-1000): ", 0, 1000);
        
        if (!add_edge(&graph, src, dest, weight)) {
            printf("Failed to add edge. Maximum edges reached or invalid parameters.\n");
            break;
        }
        
        printf("Edge added: %d -> %d (weight %d)\n", src, dest, weight);
    }
    
    printf("\nGraph created with %d nodes and %d edges.\n", graph.num_nodes, graph.num_edges);
    
    int start_node = read_int("\nEnter start node for shortest path calculation: ", 0, num_nodes - 1);
    if (start_node == -1) {
        return 1;
    }