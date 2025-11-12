//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: io_bound ; Variation: graph_algorithms
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

void initialize_graph(struct Graph* graph, int nodes, int edges) {
    if (graph == NULL || nodes <= 0 || nodes > MAX_NODES || edges < 0 || edges > MAX_EDGES) {
        return;
    }
    graph->num_nodes = nodes;
    graph->num_edges = edges;
    for (int i = 0; i < edges; i++) {
        graph->edges[i].src = 0;
        graph->edges[i].dest = 0;
        graph->edges[i].weight = 0;
    }
}

int validate_edge_input(int src, int dest, int weight, int max_nodes) {
    if (src < 0 || src >= max_nodes || dest < 0 || dest >= max_nodes) {
        return 0;
    }
    if (src == dest) {
        return 0;
    }
    if (weight < -1000 || weight > 1000) {
        return 0;
    }
    return 1;
}

void read_graph(struct Graph* graph) {
    if (graph == NULL) {
        return;
    }
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    int nodes;
    if (scanf("%d", &nodes) != 1 || nodes <= 0 || nodes > MAX_NODES) {
        printf("Invalid number of nodes\n");
        exit(1);
    }
    
    printf("Enter number of edges (0-%d): ", MAX_EDGES);
    int edges;
    if (scanf("%d", &edges) != 1 || edges < 0 || edges > MAX_EDGES) {
        printf("Invalid number of edges\n");
        exit(1);
    }
    
    initialize_graph(graph, nodes, edges);
    
    printf("Enter edges as: source destination weight\n");
    for (int i = 0; i < edges; i++) {
        int src, dest, weight;
        printf("Edge %d: ", i + 1);
        if (scanf("%d %d %d", &src, &dest, &weight) != 3) {
            printf("Invalid edge input\n");
            exit(1);
        }
        
        if (!validate_edge_input(src, dest, weight, nodes)) {
            printf("Invalid edge parameters\n");
            exit(1);
        }
        
        graph->edges[i].src = src;
        graph->edges[i].dest = dest;
        graph->edges[i].weight = weight;
    }
}

void bellman_ford(struct Graph* graph, int start_node) {
    if (graph == NULL || start_node < 0 || start_node >= graph->num_nodes) {
        return;
    }
    
    int dist[MAX_NODES];
    for (int i = 0; i < graph->num_nodes; i++) {
        dist[i] = INT_MAX;
    }
    dist[start_node] = 0;
    
    for (int i = 0; i < graph->num_nodes - 1; i++) {
        for (int j = 0; j < graph->num_edges; j++) {
            int u = graph->edges[j].src;
            int v = graph->edges[j].dest;
            int weight = graph->edges[j].weight;
            
            if (dist[u] != INT_MAX) {
                long long new_dist = (long long)dist[u] + weight;
                if (new_dist < INT_MAX && new_dist > INT_MIN) {
                    if (new_dist < dist[v]) {
                        dist[v] = (int)new_dist;
                    }
                }
            }
        }
    }
    
    for (int j = 0; j < graph->num_edges; j++) {
        int u = graph->edges[j].src;
        int v = graph->edges[j].dest;
        int weight = graph->edges[j].weight;
        
        if (dist[u] != INT_MAX && dist[u] + weight < dist[v]) {
            printf("Graph contains negative weight cycle\n");
            return;
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
    
    printf("Graph Shortest Path Calculator\n");
    printf("==============================\n");
    
    read_graph(&graph);
    
    printf("\nEnter starting node for shortest path calculation: ");
    int start_node;
    if (scanf("%d", &start_node) != 1 || start_node < 0 || start_node >= graph.num_nodes) {
        printf("Invalid starting node\n");
        return 1;
    }
    
    bellman_ford(&graph, start_node);
    
    return 0;
}