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
    struct Edge *edges;
};

void init_graph(struct Graph *graph, int nodes, int edges) {
    if (nodes <= 0 || nodes > MAX_NODES || edges <= 0 || edges > MAX_EDGES) {
        fprintf(stderr, "Invalid graph parameters\n");
        exit(1);
    }
    
    graph->num_nodes = nodes;
    graph->num_edges = edges;
    graph->edges = malloc(edges * sizeof(struct Edge));
    if (graph->edges == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
}

void free_graph(struct Graph *graph) {
    free(graph->edges);
}

int validate_node(struct Graph *graph, int node) {
    return node >= 0 && node < graph->num_nodes;
}

void add_edge(struct Graph *graph, int idx, int src, int dest, int weight) {
    if (idx < 0 || idx >= graph->num_edges) {
        fprintf(stderr, "Invalid edge index\n");
        exit(1);
    }
    
    if (!validate_node(graph, src) || !validate_node(graph, dest)) {
        fprintf(stderr, "Invalid node index\n");
        exit(1);
    }
    
    struct Edge *edge_ptr = graph->edges + idx;
    edge_ptr->src = src;
    edge_ptr->dest = dest;
    edge_ptr->weight = weight;
}

void dijkstra(struct Graph *graph, int start) {
    if (!validate_node(graph, start)) {
        fprintf(stderr, "Invalid start node\n");
        return;
    }
    
    int dist[MAX_NODES];
    int visited[MAX_NODES];
    
    for (int i = 0; i < graph->num_nodes; i++) {
        dist[i] = -1;
        visited[i] = 0;
    }
    
    dist[start] = 0;
    
    for (int count = 0; count < graph->num_nodes - 1; count++) {
        int min_dist = -1;
        int min_node = -1;
        
        for (int i = 0; i < graph->num_nodes; i++) {
            if (!visited[i] && dist[i] != -1 && (min_dist == -1 || dist[i] < min_dist)) {
                min_dist = dist[i];
                min_node = i;
            }
        }
        
        if (min_node == -1) break;
        
        visited[min_node] = 1;
        
        struct Edge *edge_ptr = graph->edges;
        for (int i = 0; i < graph->num_edges; i++) {
            if (edge_ptr->src == min_node && !visited[edge_ptr->dest]) {
                int new_dist = dist[min_node] + edge_ptr->weight;
                if (dist[edge_ptr->dest] == -1 || new_dist < dist[edge_ptr->dest]) {
                    dist[edge_ptr->dest] = new_dist;
                }
            }
            edge_ptr++;
        }
    }
    
    printf("Shortest distances from node %d:\n", start);
    for (int i = 0; i < graph->num_nodes; i++) {
        if (dist[i] != -1) {
            printf("Node %d: %d\n", i, dist[i]);
        } else {
            printf("Node %d: unreachable\n", i);
        }
    }
}

int main() {
    struct Graph graph;
    int nodes, edges;
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &nodes) != 1 || nodes < 1 || nodes > MAX_NODES) {
        fprintf(stderr, "Invalid number of nodes\n");
        return 1;
    }
    
    printf("Enter number of edges (1-%d): ", MAX_EDGES);
    if (scanf("%d", &edges) != 1 || edges < 1 || edges > MAX_EDGES) {
        fprintf(stderr, "Invalid number of edges\n");
        return 1;
    }
    
    init_graph(&graph, nodes, edges);
    
    printf("Enter edges (source destination weight):\n");
    for (int i = 0; i < edges; i++) {
        int src, dest, weight;
        if (scanf("%d %d %d", &src, &dest, &weight) != 3) {
            fprintf(stderr, "Invalid edge input\n");
            free_graph(&graph);
            return 1;
        }
        
        if (src < 0 || src >= nodes || dest < 0 || dest >= nodes) {
            fprintf(stderr, "Invalid node indices\n");
            free_graph(&graph);
            return 1;
        }
        
        if (weight < 0) {
            fprintf(stderr, "Negative weights not allowed\n");
            free_graph(&graph);
            return 1;
        }
        
        add_edge(&graph, i, src, dest, weight);
    }
    
    int start_node;
    printf("Enter start node for Dijkstra: ");
    if (scanf("%d", &start_node) != 1) {
        fprintf(stderr, "Invalid start node\n");
        free_graph(&graph);
        return 1;
    }