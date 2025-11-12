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

int add_edge(struct Graph *graph, int src, int dest, int weight) {
    if (graph->num_edges >= MAX_EDGES) return 0;
    if (src < 0 || dest < 0 || src >= MAX_NODES || dest >= MAX_NODES) return 0;
    
    graph->edges[graph->num_edges].src = src;
    graph->edges[graph->num_edges].dest = dest;
    graph->edges[graph->num_edges].weight = weight;
    graph->num_edges++;
    return 1;
}

void find_shortest_paths(struct Graph *graph, int start_node) {
    if (start_node < 0 || start_node >= graph->num_nodes) {
        printf("Invalid start node\n");
        return;
    }
    
    int distances[MAX_NODES];
    for (int i = 0; i < MAX_NODES; i++) {
        distances[i] = 999999;
    }
    distances[start_node] = 0;
    
    for (int i = 0; i < graph->num_nodes - 1; i++) {
        for (int j = 0; j < graph->num_edges; j++) {
            int u = graph->edges[j].src;
            int v = graph->edges[j].dest;
            int w = graph->edges[j].weight;
            
            if (distances[u] != 999999 && distances[u] + w < distances[v]) {
                distances[v] = distances[u] + w;
            }
        }
    }
    
    printf("Shortest distances from node %d:\n", start_node);
    for (int i = 0; i < graph->num_nodes; i++) {
        if (distances[i] == 999999) {
            printf("Node %d: unreachable\n", i);
        } else {
            printf("Node %d: %d\n", i, distances[i]);
        }
    }
}

int main() {
    struct Graph graph;
    initialize_graph(&graph);
    
    printf("Enter number of nodes (max %d): ", MAX_NODES);
    int num_nodes;
    if (scanf("%d", &num_nodes) != 1 || num_nodes <= 0 || num_nodes > MAX_NODES) {
        printf("Invalid number of nodes\n");
        return 1;
    }
    graph.num_nodes = num_nodes;
    
    printf("Enter number of edges (max %d): ", MAX_EDGES);
    int num_edges;
    if (scanf("%d", &num_edges) != 1 || num_edges <= 0 || num_edges > MAX_EDGES) {
        printf("Invalid number of edges\n");
        return 1;
    }
    
    printf("Enter edges as 'source destination weight' (nodes 0 to %d):\n", num_nodes - 1);
    for (int i = 0; i < num_edges; i++) {
        int src, dest, weight;
        if (scanf("%d %d %d", &src, &dest, &weight) != 3) {
            printf("Invalid edge format\n");
            return 1;
        }
        
        if (src < 0 || src >= num_nodes || dest < 0 || dest >= num_nodes) {
            printf("Invalid node index\n");
            return 1;
        }
        
        if (!add_edge(&graph, src, dest, weight)) {
            printf("Failed to add edge\n");
            return 1;
        }
    }
    
    printf("Enter start node for shortest paths: ");
    int start_node;
    if (scanf("%d", &start_node) != 1) {
        printf("Invalid start node\n");
        return 1;
    }
    
    find_shortest_paths(&graph, start_node);
    
    return 0;
}