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
    if (src < 0 || dest < 0 || src >= MAX_NODES || dest >= MAX_NODES) return 0;
    if (weight < 0) return 0;
    
    graph->edges[graph->num_edges].src = src;
    graph->edges[graph->num_edges].dest = dest;
    graph->edges[graph->num_edges].weight = weight;
    graph->num_edges++;
    
    if (src >= graph->num_nodes) graph->num_nodes = src + 1;
    if (dest >= graph->num_nodes) graph->num_nodes = dest + 1;
    
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
        
        if (min_node == -1) break;
        visited[min_node] = 1;
        
        for (int i = 0; i < graph->num_edges; i++) {
            if (graph->edges[i].src == min_node) {
                int neighbor = graph->edges[i].dest;
                int new_dist = dist[min_node] + graph->edges[i].weight;
                if (new_dist > 0 && new_dist < dist[neighbor]) {
                    dist[neighbor] = new_dist;
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

int main() {
    struct Graph graph;
    initialize_graph(&graph);
    
    printf("Graph Shortest Path Calculator\n");
    printf("Enter edges (source destination weight), one per line.\n");
    printf("Enter -1 -1 -1 to finish input.\n");
    
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
            printf("Failed to add edge. Check bounds and values.\n");
        }
    }
    
    if (graph.num_nodes == 0) {
        printf("No graph data entered.\n");
        return 1;
    }
    
    printf("Enter start node for Dijkstra's algorithm: ");
    if (scanf("%d", &src) != 1) {
        printf("Invalid start node\n");
        return 1;
    }
    
    dijkstra(&graph, src);
    
    return 0;
}