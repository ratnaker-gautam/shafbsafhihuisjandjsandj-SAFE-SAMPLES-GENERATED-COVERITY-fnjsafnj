//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: graph_algorithms
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

void init_graph(struct Graph *graph, int nodes, int edges) {
    if (graph == NULL || nodes <= 0 || nodes > MAX_NODES || edges < 0 || edges > MAX_EDGES) {
        return;
    }
    graph->num_nodes = nodes;
    graph->num_edges = edges;
    memset(graph->edges, 0, sizeof(graph->edges));
}

int add_edge(struct Graph *graph, int src, int dest, int weight) {
    if (graph == NULL || src < 0 || src >= graph->num_nodes || 
        dest < 0 || dest >= graph->num_nodes || 
        graph->num_edges >= MAX_EDGES) {
        return 0;
    }
    struct Edge *edge_ptr = graph->edges + graph->num_edges;
    edge_ptr->src = src;
    edge_ptr->dest = dest;
    edge_ptr->weight = weight;
    graph->num_edges++;
    return 1;
}

void print_graph(struct Graph *graph) {
    if (graph == NULL) return;
    printf("Graph with %d nodes and %d edges:\n", graph->num_nodes, graph->num_edges);
    struct Edge *edge_ptr = graph->edges;
    for (int i = 0; i < graph->num_edges; i++) {
        printf("Edge %d: %d -> %d (weight %d)\n", i, edge_ptr->src, edge_ptr->dest, edge_ptr->weight);
        edge_ptr++;
    }
}

void bellman_ford(struct Graph *graph, int start) {
    if (graph == NULL || start < 0 || start >= graph->num_nodes) return;
    
    int dist[MAX_NODES];
    for (int i = 0; i < graph->num_nodes; i++) {
        dist[i] = 1000000;
    }
    dist[start] = 0;
    
    struct Edge *edge_ptr = graph->edges;
    for (int i = 0; i < graph->num_nodes - 1; i++) {
        for (int j = 0; j < graph->num_edges; j++) {
            if (dist[edge_ptr->src] != 1000000 && 
                dist[edge_ptr->src] + edge_ptr->weight < dist[edge_ptr->dest]) {
                dist[edge_ptr->dest] = dist[edge_ptr->src] + edge_ptr->weight;
            }
            edge_ptr++;
        }
        edge_ptr = graph->edges;
    }
    
    edge_ptr = graph->edges;
    for (int j = 0; j < graph->num_edges; j++) {
        if (dist[edge_ptr->src] != 1000000 && 
            dist[edge_ptr->src] + edge_ptr->weight < dist[edge_ptr->dest]) {
            printf("Graph contains negative weight cycle\n");
            return;
        }
        edge_ptr++;
    }
    
    printf("Shortest distances from node %d:\n", start);
    for (int i = 0; i < graph->num_nodes; i++) {
        if (dist[i] == 1000000) {
            printf("Node %d: unreachable\n", i);
        } else {
            printf("Node %d: %d\n", i, dist[i]);
        }
    }
}

int main() {
    struct Graph graph;
    int num_nodes, num_edges;
    
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
    
    init_graph(&graph, num_nodes, num_edges);
    
    printf("Enter edges as 'source destination weight':\n");
    for (int i = 0; i < num_edges; i++) {
        int src, dest, weight;
        if (scanf("%d %d %d", &src, &dest, &weight) != 3) {
            printf("Invalid edge input\n");
            return 1;
        }
        if (!add_edge(&graph, src, dest, weight)) {
            printf("Failed to add edge\n");
            return 1;
        }
    }
    
    print_graph(&graph);
    
    int start_node;
    printf("Enter starting node for Bellman-Ford: ");
    if (scanf("%d", &start_node) != 1 || start_node < 0 || start_node >= num_nodes) {
        printf("Invalid starting node\n");
        return 1;
    }
    
    bellman_ford(&graph, start_node);
    
    return 0;
}