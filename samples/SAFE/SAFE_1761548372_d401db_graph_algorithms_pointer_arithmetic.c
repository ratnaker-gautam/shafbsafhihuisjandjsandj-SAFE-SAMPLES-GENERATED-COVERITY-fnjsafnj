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

void initialize_graph(struct Graph *graph) {
    graph->num_nodes = 0;
    graph->num_edges = 0;
}

int add_edge(struct Graph *graph, int src, int dest, int weight) {
    if (graph->num_edges >= MAX_EDGES) return 0;
    if (src < 0 || dest < 0 || src >= MAX_NODES || dest >= MAX_NODES) return 0;
    
    struct Edge *edge_ptr = graph->edges + graph->num_edges;
    edge_ptr->src = src;
    edge_ptr->dest = dest;
    edge_ptr->weight = weight;
    graph->num_edges++;
    return 1;
}

void print_graph(struct Graph *graph) {
    printf("Graph with %d nodes and %d edges:\n", graph->num_nodes, graph->num_edges);
    struct Edge *edge_ptr = graph->edges;
    for (int i = 0; i < graph->num_edges; i++) {
        printf("Edge %d: %d -> %d (weight %d)\n", i, edge_ptr->src, edge_ptr->dest, edge_ptr->weight);
        edge_ptr++;
    }
}

int find_shortest_path(struct Graph *graph, int start, int end) {
    if (start < 0 || end < 0 || start >= graph->num_nodes || end >= graph->num_nodes) return -1;
    
    int distances[MAX_NODES];
    int visited[MAX_NODES];
    
    for (int i = 0; i < graph->num_nodes; i++) {
        distances[i] = -1;
        visited[i] = 0;
    }
    
    distances[start] = 0;
    
    for (int count = 0; count < graph->num_nodes - 1; count++) {
        int min_dist = -1;
        int min_node = -1;
        
        for (int i = 0; i < graph->num_nodes; i++) {
            if (!visited[i] && distances[i] != -1 && (min_dist == -1 || distances[i] < min_dist)) {
                min_dist = distances[i];
                min_node = i;
            }
        }
        
        if (min_node == -1) break;
        visited[min_node] = 1;
        
        struct Edge *edge_ptr = graph->edges;
        for (int i = 0; i < graph->num_edges; i++) {
            if (edge_ptr->src == min_node && !visited[edge_ptr->dest]) {
                int new_dist = distances[min_node] + edge_ptr->weight;
                if (distances[edge_ptr->dest] == -1 || new_dist < distances[edge_ptr->dest]) {
                    distances[edge_ptr->dest] = new_dist;
                }
            }
            edge_ptr++;
        }
    }
    
    return distances[end];
}

int main() {
    struct Graph graph;
    initialize_graph(&graph);
    
    printf("Enter number of nodes (max %d): ", MAX_NODES);
    if (scanf("%d", &graph.num_nodes) != 1 || graph.num_nodes <= 0 || graph.num_nodes > MAX_NODES) {
        printf("Invalid number of nodes\n");
        return 1;
    }
    
    printf("Enter number of edges (max %d): ", MAX_EDGES);
    if (scanf("%d", &graph.num_edges) != 1 || graph.num_edges <= 0 || graph.num_edges > MAX_EDGES) {
        printf("Invalid number of edges\n");
        return 1;
    }
    
    printf("Enter edges as: source destination weight\n");
    for (int i = 0; i < graph.num_edges; i++) {
        int src, dest, weight;
        if (scanf("%d %d %d", &src, &dest, &weight) != 3) {
            printf("Invalid edge input\n");
            return 1;
        }
        if (src < 0 || dest < 0 || src >= graph.num_nodes || dest >= graph.num_nodes) {
            printf("Invalid node indices\n");
            return 1;
        }
        if (!add_edge(&graph, src, dest, weight)) {
            printf("Failed to add edge\n");
            return 1;
        }
    }
    
    print_graph(&graph);
    
    int start, end;
    printf("Enter start and end nodes for shortest path: ");
    if (scanf("%d %d", &start, &end) != 2) {
        printf("Invalid node input\n");
        return 1;
    }
    
    int shortest = find_shortest_path(&graph, start, end);
    if (shortest == -1) {
        printf("No path exists between %d and %d\n", start, end);
    } else {
        printf("Shortest path distance: %d\n", shortest);
    }
    
    return 0;
}