//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: graph_algorithms
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

void initialize_graph(struct Graph *graph, int nodes) {
    if (graph == NULL || nodes <= 0 || nodes > MAX_NODES) {
        fprintf(stderr, "Invalid graph initialization parameters\n");
        exit(EXIT_FAILURE);
    }
    graph->num_nodes = nodes;
    graph->num_edges = 0;
}

int add_edge(struct Graph *graph, int src, int dest, int weight) {
    if (graph == NULL || src < 0 || dest < 0 || src >= graph->num_nodes || dest >= graph->num_nodes) {
        return -1;
    }
    if (graph->num_edges >= MAX_EDGES) {
        return -1;
    }
    
    struct Edge *edge_ptr = graph->edges + graph->num_edges;
    edge_ptr->src = src;
    edge_ptr->dest = dest;
    edge_ptr->weight = weight;
    graph->num_edges++;
    return 0;
}

void print_graph(const struct Graph *graph) {
    if (graph == NULL) {
        return;
    }
    
    printf("Graph with %d nodes and %d edges:\n", graph->num_nodes, graph->num_edges);
    const struct Edge *edge_ptr = graph->edges;
    for (int i = 0; i < graph->num_edges; i++) {
        printf("Edge %d: %d -> %d (weight: %d)\n", i, edge_ptr->src, edge_ptr->dest, edge_ptr->weight);
        edge_ptr++;
    }
}

int find_shortest_path(const struct Graph *graph, int start, int end, int *path, int *path_length) {
    if (graph == NULL || path == NULL || path_length == NULL) {
        return -1;
    }
    if (start < 0 || end < 0 || start >= graph->num_nodes || end >= graph->num_nodes) {
        return -1;
    }
    
    int distances[MAX_NODES];
    int visited[MAX_NODES];
    int previous[MAX_NODES];
    
    for (int i = 0; i < graph->num_nodes; i++) {
        distances[i] = -1;
        visited[i] = 0;
        previous[i] = -1;
    }
    
    distances[start] = 0;
    
    for (int count = 0; count < graph->num_nodes - 1; count++) {
        int min_distance = -1;
        int min_node = -1;
        
        for (int i = 0; i < graph->num_nodes; i++) {
            if (!visited[i] && distances[i] != -1 && (min_distance == -1 || distances[i] < min_distance)) {
                min_distance = distances[i];
                min_node = i;
            }
        }
        
        if (min_node == -1) {
            break;
        }
        
        visited[min_node] = 1;
        
        const struct Edge *edge_ptr = graph->edges;
        for (int i = 0; i < graph->num_edges; i++) {
            if (edge_ptr->src == min_node && !visited[edge_ptr->dest]) {
                int new_distance = distances[min_node] + edge_ptr->weight;
                if (distances[edge_ptr->dest] == -1 || new_distance < distances[edge_ptr->dest]) {
                    distances[edge_ptr->dest] = new_distance;
                    previous[edge_ptr->dest] = min_node;
                }
            }
            edge_ptr++;
        }
    }
    
    if (distances[end] == -1) {
        return -1;
    }
    
    int current = end;
    int temp_path[MAX_NODES];
    int temp_length = 0;
    
    while (current != -1) {
        if (temp_length >= MAX_NODES) {
            return -1;
        }
        temp_path[temp_length++] = current;
        current = previous[current];
    }
    
    for (int i = 0; i < temp_length; i++) {
        path[i] = temp_path[temp_length - 1 - i];
    }
    *path_length = temp_length;
    
    return distances[end];
}

int main() {
    struct Graph graph;
    initialize_graph(&graph, 6);
    
    add_edge(&graph, 0, 1, 4);
    add_edge(&graph, 0, 2, 2);
    add_edge(&graph, 1, 2, 1);
    add_edge(&graph, 1, 3, 5);
    add_edge(&graph, 2, 3, 8);
    add_edge(&graph, 2, 4, 10);
    add_edge(&graph, 3, 4, 2);
    add_edge(&graph, 3, 5, 6);
    add_edge(&graph, 4, 5, 3);
    
    print_graph(&graph);
    
    int path[MAX_NODES];
    int path_length;
    int start_node = 0;
    int end_node = 5;
    
    int distance = find_shortest_path(&graph, start_node, end_node, path, &path_length);
    
    if (distance ==