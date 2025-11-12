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

void initialize_graph(struct Graph* graph) {
    if (graph == NULL) return;
    graph->num_nodes = 0;
    graph->num_edges = 0;
}

int add_edge(struct Graph* graph, int src, int dest, int weight) {
    if (graph == NULL) return 0;
    if (graph->num_edges >= MAX_EDGES) return 0;
    if (src < 0 || dest < 0) return 0;
    if (weight < 0) return 0;
    
    graph->edges[graph->num_edges].src = src;
    graph->edges[graph->num_edges].dest = dest;
    graph->edges[graph->num_edges].weight = weight;
    graph->num_edges++;
    
    int max_node = (src > dest) ? src : dest;
    if (max_node >= graph->num_nodes) {
        graph->num_nodes = max_node + 1;
    }
    
    return 1;
}

void dijkstra(struct Graph* graph, int start_node) {
    if (graph == NULL) return;
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
                
                if (!visited[neighbor] && dist[min_node] != INT_MAX) {
                    long new_dist = (long)dist[min_node] + weight;
                    if (new_dist < dist[neighbor] && new_dist < INT_MAX) {
                        dist[neighbor] = (int)new_dist;
                    }
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
    
    printf("Graph Algorithm Demo\n");
    printf("Adding edges to graph...\n");
    
    if (!add_edge(&graph, 0, 1, 4)) return 1;
    if (!add_edge(&graph, 0, 2, 1)) return 1;
    if (!add_edge(&graph, 1, 2, 2)) return 1;
    if (!add_edge(&graph, 1, 3, 5)) return 1;
    if (!add_edge(&graph, 2, 3, 8)) return 1;
    if (!add_edge(&graph, 2, 4, 10)) return 1;
    if (!add_edge(&graph, 3, 4, 2)) return 1;
    if (!add_edge(&graph, 4, 5, 6)) return 1;
    
    printf("Graph has %d nodes and %d edges\n", graph.num_nodes, graph.num_edges);
    
    printf("\nRunning Dijkstra's algorithm from node 0:\n");
    dijkstra(&graph, 0);
    
    printf("\nRunning Dijkstra's algorithm from node 3:\n");
    dijkstra(&graph, 3);
    
    return 0;
}