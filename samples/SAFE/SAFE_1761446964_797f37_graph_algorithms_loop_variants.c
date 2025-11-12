//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
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

void initialize_graph(struct Graph *graph, int nodes, int edges) {
    if (graph == NULL || nodes <= 0 || nodes > MAX_NODES || edges < 0 || edges > MAX_EDGES) {
        return;
    }
    graph->num_nodes = nodes;
    graph->num_edges = edges;
}

int validate_edge(int src, int dest, int weight, int num_nodes) {
    if (src < 0 || src >= num_nodes || dest < 0 || dest >= num_nodes) {
        return 0;
    }
    if (weight < -1000 || weight > 1000) {
        return 0;
    }
    return 1;
}

int add_edge(struct Graph *graph, int index, int src, int dest, int weight) {
    if (graph == NULL || index < 0 || index >= graph->num_edges) {
        return 0;
    }
    if (!validate_edge(src, dest, weight, graph->num_nodes)) {
        return 0;
    }
    graph->edges[index].src = src;
    graph->edges[index].dest = dest;
    graph->edges[index].weight = weight;
    return 1;
}

void bellman_ford(struct Graph *graph, int start) {
    if (graph == NULL || start < 0 || start >= graph->num_nodes) {
        return;
    }
    
    int dist[MAX_NODES];
    int i, j;
    
    for (i = 0; i < graph->num_nodes; i++) {
        dist[i] = INT_MAX;
    }
    dist[start] = 0;
    
    for (i = 0; i < graph->num_nodes - 1; i++) {
        int updated = 0;
        for (j = 0; j < graph->num_edges; j++) {
            int u = graph->edges[j].src;
            int v = graph->edges[j].dest;
            int w = graph->edges[j].weight;
            
            if (dist[u] != INT_MAX && dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                updated = 1;
            }
        }
        if (!updated) {
            break;
        }
    }
    
    for (i = 0; i < graph->num_edges; i++) {
        int u = graph->edges[i].src;
        int v = graph->edges[i].dest;
        int w = graph->edges[i].weight;
        
        if (dist[u] != INT_MAX && dist[u] + w < dist[v]) {
            printf("Graph contains negative weight cycle\n");
            return;
        }
    }
    
    printf("Shortest distances from node %d:\n", start);
    for (i = 0; i < graph->num_nodes; i++) {
        if (dist[i] == INT_MAX) {
            printf("Node %d: unreachable\n", i);
        } else {
            printf("Node %d: %d\n", i, dist[i]);
        }
    }
}

int main() {
    struct Graph graph;
    int num_nodes, num_edges;
    int i;
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &num_nodes) != 1 || num_nodes < 1 || num_nodes > MAX_NODES) {
        printf("Invalid number of nodes\n");
        return 1;
    }
    
    printf("Enter number of edges (0-%d): ", MAX_EDGES);
    if (scanf("%d", &num_edges) != 1 || num_edges < 0 || num_edges > MAX_EDGES) {
        printf("Invalid number of edges\n");
        return 1;
    }
    
    initialize_graph(&graph, num_nodes, num_edges);
    
    printf("Enter edges as 'source destination weight':\n");
    for (i = 0; i < num_edges; i++) {
        int src, dest, weight;
        printf("Edge %d: ", i);
        if (scanf("%d %d %d", &src, &dest, &weight) != 3) {
            printf("Invalid edge input\n");
            return 1;
        }
        if (!add_edge(&graph, i, src, dest, weight)) {
            printf("Invalid edge parameters\n");
            return 1;
        }
    }
    
    int start_node;
    printf("Enter starting node (0-%d): ", num_nodes - 1);
    if (scanf("%d", &start_node) != 1 || start_node < 0 || start_node >= num_nodes) {
        printf("Invalid starting node\n");
        return 1;
    }
    
    bellman_ford(&graph, start_node);
    
    return 0;
}