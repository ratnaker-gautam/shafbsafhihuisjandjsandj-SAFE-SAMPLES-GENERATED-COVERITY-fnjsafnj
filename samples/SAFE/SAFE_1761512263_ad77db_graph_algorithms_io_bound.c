//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 100
#define MAX_EDGES 1000
#define MAX_LINE 1024

typedef struct {
    int from;
    int to;
    int weight;
} Edge;

typedef struct {
    int node_count;
    int edge_count;
    Edge edges[MAX_EDGES];
} Graph;

int read_graph(Graph *graph) {
    char line[MAX_LINE];
    int node_count, edge_count;
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (fgets(line, sizeof(line), stdin) == NULL) return 0;
    if (sscanf(line, "%d", &node_count) != 1) return 0;
    if (node_count < 1 || node_count > MAX_NODES) return 0;
    
    printf("Enter number of edges (0-%d): ", MAX_EDGES);
    if (fgets(line, sizeof(line), stdin) == NULL) return 0;
    if (sscanf(line, "%d", &edge_count) != 1) return 0;
    if (edge_count < 0 || edge_count > MAX_EDGES) return 0;
    
    graph->node_count = node_count;
    graph->edge_count = 0;
    
    for (int i = 0; i < edge_count; i++) {
        int from, to, weight;
        printf("Enter edge %d (from to weight): ", i + 1);
        if (fgets(line, sizeof(line), stdin) == NULL) return 0;
        if (sscanf(line, "%d %d %d", &from, &to, &weight) != 3) return 0;
        
        if (from < 0 || from >= node_count || to < 0 || to >= node_count) {
            printf("Invalid node indices\n");
            return 0;
        }
        
        if (graph->edge_count >= MAX_EDGES) return 0;
        
        graph->edges[graph->edge_count].from = from;
        graph->edges[graph->edge_count].to = to;
        graph->edges[graph->edge_count].weight = weight;
        graph->edge_count++;
    }
    
    return 1;
}

void find_shortest_paths(Graph *graph, int start_node) {
    int distances[MAX_NODES];
    int visited[MAX_NODES];
    
    for (int i = 0; i < graph->node_count; i++) {
        distances[i] = -1;
        visited[i] = 0;
    }
    
    distances[start_node] = 0;
    
    for (int i = 0; i < graph->node_count - 1; i++) {
        int min_distance = -1;
        int current_node = -1;
        
        for (int j = 0; j < graph->node_count; j++) {
            if (!visited[j] && distances[j] != -1) {
                if (min_distance == -1 || distances[j] < min_distance) {
                    min_distance = distances[j];
                    current_node = j;
                }
            }
        }
        
        if (current_node == -1) break;
        visited[current_node] = 1;
        
        for (int j = 0; j < graph->edge_count; j++) {
            if (graph->edges[j].from == current_node) {
                int neighbor = graph->edges[j].to;
                int new_distance = distances[current_node] + graph->edges[j].weight;
                
                if (distances[neighbor] == -1 || new_distance < distances[neighbor]) {
                    distances[neighbor] = new_distance;
                }
            }
        }
    }
    
    printf("\nShortest paths from node %d:\n", start_node);
    for (int i = 0; i < graph->node_count; i++) {
        if (distances[i] == -1) {
            printf("Node %d: unreachable\n", i);
        } else {
            printf("Node %d: distance %d\n", i, distances[i]);
        }
    }
}

int main() {
    Graph graph;
    
    printf("Graph Shortest Path Finder\n");
    printf("==========================\n\n");
    
    if (!read_graph(&graph)) {
        printf("Error reading graph data\n");
        return 1;
    }
    
    char line[MAX_LINE];
    int start_node;
    
    printf("\nEnter starting node for shortest paths: ");
    if (fgets(line, sizeof(line), stdin) == NULL) return 1;
    if (sscanf(line, "%d", &start_node) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (start_node < 0 || start_node >= graph.node_count) {
        printf("Invalid starting node\n");
        return 1;
    }
    
    find_shortest_paths(&graph, start_node);
    
    return 0;
}