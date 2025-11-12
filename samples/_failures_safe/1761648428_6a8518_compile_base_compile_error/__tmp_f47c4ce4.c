//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_NODES 100
#define MAX_EDGES 1000

struct Graph {
    int num_nodes;
    int num_edges;
    int adjacency[MAX_NODES][MAX_NODES];
    int edge_weights[MAX_NODES][MAX_NODES];
};

struct Graph* create_graph(int nodes, int edges) {
    if (nodes <= 0 || nodes > MAX_NODES || edges < 0 || edges > MAX_EDGES) {
        return NULL;
    }
    
    struct Graph* graph = malloc(sizeof(struct Graph));
    if (graph == NULL) {
        return NULL;
    }
    
    graph->num_nodes = nodes;
    graph->num_edges = edges;
    
    for (int i = 0; i < MAX_NODES; i++) {
        for (int j = 0; j < MAX_NODES; j++) {
            graph->adjacency[i][j] = 0;
            graph->edge_weights[i][j] = 0;
        }
    }
    
    return graph;
}

int add_edge(struct Graph* graph, int from, int to, int weight) {
    if (graph == NULL) return 0;
    if (from < 0 || from >= graph->num_nodes) return 0;
    if (to < 0 || to >= graph->num_nodes) return 0;
    if (weight < 0) return 0;
    
    graph->adjacency[from][to] = 1;
    graph->edge_weights[from][to] = weight;
    return 1;
}

int find_min_distance(int distances[], int visited[], int num_nodes) {
    int min_distance = INT_MAX;
    int min_index = -1;
    
    for (int i = 0; i < num_nodes; i++) {
        if (!visited[i] && distances[i] <= min_distance) {
            min_distance = distances[i];
            min_index = i;
        }
    }
    
    return min_index;
}

void dijkstra(struct Graph* graph, int start_node) {
    if (graph == NULL) return;
    if (start_node < 0 || start_node >= graph->num_nodes) return;
    
    int distances[MAX_NODES];
    int visited[MAX_NODES];
    
    for (int i = 0; i < graph->num_nodes; i++) {
        distances[i] = INT_MAX;
        visited[i] = 0;
    }
    
    distances[start_node] = 0;
    
    for (int count = 0; count < graph->num_nodes - 1; count++) {
        int current = find_min_distance(distances, visited, graph->num_nodes);
        if (current == -1) break;
        
        visited[current] = 1;
        
        for (int neighbor = 0; neighbor < graph->num_nodes; neighbor++) {
            if (!visited[neighbor] && graph->adjacency[current][neighbor] && 
                distances[current] != INT_MAX) {
                int new_distance = distances[current] + graph->edge_weights[current][neighbor];
                if (new_distance < distances[neighbor]) {
                    distances[neighbor] = new_distance;
                }
            }
        }
    }
    
    printf("Shortest distances from node %d:\n", start_node);
    for (int i = 0; i < graph->num_nodes; i++) {
        if (distances[i] == INT_MAX) {
            printf("Node %d: unreachable\n", i);
        } else {
            printf("Node %d: %d\n", i, distances[i]);
        }
    }
}

void dfs_util(struct Graph* graph, int node, int visited[]) {
    if (graph == NULL) return;
    if (node < 0 || node >= graph->num_nodes) return;
    if (visited[node]) return;
    
    visited[node] = 1;
    printf("%d ", node);
    
    for (int i = 0; i < graph->num_nodes; i++) {
        if (graph->adjacency[node][i] && !visited[i]) {
            dfs_util(graph, i, visited);
        }
    }
}

void depth_first_search(struct Graph* graph, int start_node) {
    if (graph == NULL) return;
    if (start_node < 0 || start_node >= graph->num_nodes) return;
    
    int visited[MAX_NODES];
    for (int i = 0; i < graph->num_nodes; i++) {
        visited[i] = 0;
    }
    
    printf("DFS traversal starting from node %d: ", start_node);
    dfs_util(graph, start_node, visited);
    printf("\n");
}

int main() {
    struct Graph* graph = create_graph(6, 8);
    if (graph == NULL) {
        printf("Failed to create graph\n");
        return 1;
    }
    
    add_edge(graph, 0, 1, 4);
    add_edge(graph, 0, 2, 2);
    add_edge(graph, 1, 2, 1);
    add_edge(graph, 1, 3, 5);
    add_edge(graph, 2, 3, 8);
    add_edge(graph, 2, 4, 10);
    add_edge(graph, 3, 4, 2);
    add_edge(graph, 3, 5, 6);
    add_edge(graph, 4, 5, 3