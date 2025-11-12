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
    if (from == to) return 0;
    
    graph->adjacency[from][to] = 1;
    graph->edge_weights[from][to] = weight;
    return 1;
}

void dfs_util(struct Graph* graph, int node, int visited[]) {
    if (graph == NULL || visited == NULL) return;
    if (node < 0 || node >= graph->num_nodes) return;
    
    visited[node] = 1;
    printf("%d ", node);
    
    for (int i = 0; i < graph->num_nodes; i++) {
        if (graph->adjacency[node][i] && !visited[i]) {
            dfs_util(graph, i, visited);
        }
    }
}

void depth_first_search(struct Graph* graph, int start) {
    if (graph == NULL) return;
    if (start < 0 || start >= graph->num_nodes) return;
    
    int visited[MAX_NODES];
    for (int i = 0; i < MAX_NODES; i++) {
        visited[i] = 0;
    }
    
    printf("DFS traversal: ");
    dfs_util(graph, start, visited);
    printf("\n");
}

void dijkstra(struct Graph* graph, int source) {
    if (graph == NULL) return;
    if (source < 0 || source >= graph->num_nodes) return;
    
    int dist[MAX_NODES];
    int visited[MAX_NODES];
    
    for (int i = 0; i < graph->num_nodes; i++) {
        dist[i] = INT_MAX;
        visited[i] = 0;
    }
    dist[source] = 0;
    
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
        
        for (int v = 0; v < graph->num_nodes; v++) {
            if (!visited[v] && graph->adjacency[min_node][v] && 
                dist[min_node] != INT_MAX) {
                int new_dist = dist[min_node] + graph->edge_weights[min_node][v];
                if (new_dist > 0 && new_dist < dist[v]) {
                    dist[v] = new_dist;
                }
            }
        }
    }
    
    printf("Shortest distances from node %d:\n", source);
    for (int i = 0; i < graph->num_nodes; i++) {
        if (dist[i] == INT_MAX) {
            printf("Node %d: unreachable\n", i);
        } else {
            printf("Node %d: %d\n", i, dist[i]);
        }
    }
}

int validate_input_int(const char* prompt, int min_val, int max_val) {
    if (prompt == NULL) return -1;
    
    int value;
    char buffer[100];
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
        }
        
        if (sscanf(buffer, "%d", &value) == 1) {
            if (value >= min_val && value <= max_val) {
                return value;
            }
        }
        printf("Invalid input. Please enter a number between %d and %d.\n", min_val, max_val);
    }
}

int main() {
    printf("Graph Algorithms Demo\n");
    printf("====================\n");
    
    int nodes = validate_input_int("Enter number of nodes (1-100): ", 1, MAX_NODES);
    if (nodes == -1) {
        printf("Input error.\n");
        return 1;
    }
    
    int edges =