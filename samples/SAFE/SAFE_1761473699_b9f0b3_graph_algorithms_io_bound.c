//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int validate_node(int node, int max_nodes) {
    return (node >= 0 && node < max_nodes);
}

int add_edge(struct Graph *graph, int src, int dest, int weight) {
    if (graph->num_edges >= MAX_EDGES) {
        return 0;
    }
    if (!validate_node(src, MAX_NODES) || !validate_node(dest, MAX_NODES)) {
        return 0;
    }
    if (src == dest) {
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

void find_shortest_paths(struct Graph *graph, int start_node) {
    if (!validate_node(start_node, MAX_NODES)) {
        printf("Invalid start node\n");
        return;
    }
    
    int distances[MAX_NODES];
    int visited[MAX_NODES];
    
    for (int i = 0; i < MAX_NODES; i++) {
        distances[i] = -1;
        visited[i] = 0;
    }
    distances[start_node] = 0;
    
    for (int count = 0; count < MAX_NODES - 1; count++) {
        int min_distance = -1;
        int current_node = -1;
        
        for (int i = 0; i < MAX_NODES; i++) {
            if (!visited[i] && distances[i] != -1 && (min_distance == -1 || distances[i] < min_distance)) {
                min_distance = distances[i];
                current_node = i;
            }
        }
        
        if (current_node == -1) {
            break;
        }
        
        visited[current_node] = 1;
        
        for (int i = 0; i < graph->num_edges; i++) {
            if (graph->edges[i].src == current_node) {
                int neighbor = graph->edges[i].dest;
                int new_distance = distances[current_node] + graph->edges[i].weight;
                
                if (distances[neighbor] == -1 || new_distance < distances[neighbor]) {
                    distances[neighbor] = new_distance;
                }
            }
        }
    }
    
    printf("Shortest paths from node %d:\n", start_node);
    for (int i = 0; i < MAX_NODES; i++) {
        if (distances[i] != -1 && i != start_node) {
            printf("Node %d: distance %d\n", i, distances[i]);
        }
    }
}

int main() {
    struct Graph graph;
    initialize_graph(&graph);
    
    printf("Graph Shortest Path Finder\n");
    printf("Enter edges (source destination weight), one per line\n");
    printf("Enter -1 -1 -1 to finish\n");
    
    int src, dest, weight;
    while (1) {
        if (scanf("%d %d %d", &src, &dest, &weight) != 3) {
            printf("Invalid input format\n");
            while (getchar() != '\n');
            continue;
        }
        
        if (src == -1 && dest == -1 && weight == -1) {
            break;
        }
        
        if (!add_edge(&graph, src, dest, weight)) {
            printf("Invalid edge: %d %d %d\n", src, dest, weight);
        } else {
            printf("Added edge: %d -> %d (weight %d)\n", src, dest, weight);
        }
    }
    
    printf("Enter start node for shortest path calculation: ");
    int start_node;
    if (scanf("%d", &start_node) != 1) {
        printf("Invalid start node\n");
        return 1;
    }
    
    find_shortest_paths(&graph, start_node);
    
    return 0;
}