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
    if (graph->num_edges >= MAX_EDGES) {
        return 0;
    }
    if (src < 0 || src >= MAX_NODES || dest < 0 || dest >= MAX_NODES) {
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

void find_shortest_paths(struct Graph *graph, int start_node, int distances[]) {
    int i, j;
    
    for (i = 0; i < MAX_NODES; i++) {
        distances[i] = -1;
    }
    
    if (start_node < 0 || start_node >= MAX_NODES) {
        return;
    }
    
    distances[start_node] = 0;
    
    for (i = 0; i < graph->num_nodes - 1; i++) {
        int updated = 0;
        for (j = 0; j < graph->num_edges; j++) {
            int u = graph->edges[j].src;
            int v = graph->edges[j].dest;
            int w = graph->edges[j].weight;
            
            if (distances[u] != -1) {
                int new_dist = distances[u] + w;
                if (distances[v] == -1 || new_dist < distances[v]) {
                    if (new_dist >= 0) {
                        distances[v] = new_dist;
                        updated = 1;
                    }
                }
            }
        }
        if (!updated) {
            break;
        }
    }
}

int main() {
    struct Graph graph;
    int distances[MAX_NODES];
    int num_nodes, num_edges;
    int i, start_node;
    
    initialize_graph(&graph);
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &num_nodes) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    if (num_nodes < 1 || num_nodes > MAX_NODES) {
        printf("Invalid number of nodes\n");
        return 1;
    }
    graph.num_nodes = num_nodes;
    
    printf("Enter number of edges (0-%d): ", MAX_EDGES);
    if (scanf("%d", &num_edges) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    if (num_edges < 0 || num_edges > MAX_EDGES) {
        printf("Invalid number of edges\n");
        return 1;
    }
    
    printf("Enter edges as 'source destination weight' (nodes 0-%d):\n", num_nodes-1);
    for (i = 0; i < num_edges; i++) {
        int src, dest, weight;
        if (scanf("%d %d %d", &src, &dest, &weight) != 3) {
            printf("Invalid edge format\n");
            return 1;
        }
        if (!add_edge(&graph, src, dest, weight)) {
            printf("Failed to add edge\n");
            return 1;
        }
    }
    
    printf("Enter starting node (0-%d): ", num_nodes-1);
    if (scanf("%d", &start_node) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    if (start_node < 0 || start_node >= num_nodes) {
        printf("Invalid starting node\n");
        return 1;
    }
    
    find_shortest_paths(&graph, start_node, distances);
    
    printf("\nShortest distances from node %d:\n", start_node);
    for (i = 0; i < num_nodes; i++) {
        if (distances[i] == -1) {
            printf("Node %d: unreachable\n", i);
        } else {
            printf("Node %d: %d\n", i, distances[i]);
        }
    }
    
    return 0;
}