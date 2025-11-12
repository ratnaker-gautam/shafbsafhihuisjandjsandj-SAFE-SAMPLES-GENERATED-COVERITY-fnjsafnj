//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_NODES 100
#define MAX_EDGES 500

struct Graph {
    int num_nodes;
    int num_edges;
    int *adj_matrix;
};

struct Graph* create_graph(int nodes) {
    if (nodes <= 0 || nodes > MAX_NODES) return NULL;
    
    struct Graph *graph = malloc(sizeof(struct Graph));
    if (!graph) return NULL;
    
    graph->num_nodes = nodes;
    graph->num_edges = 0;
    
    size_t matrix_size = (size_t)nodes * (size_t)nodes * sizeof(int);
    if (matrix_size > SIZE_MAX / sizeof(int)) {
        free(graph);
        return NULL;
    }
    
    graph->adj_matrix = calloc((size_t)nodes * (size_t)nodes, sizeof(int));
    if (!graph->adj_matrix) {
        free(graph);
        return NULL;
    }
    
    return graph;
}

void destroy_graph(struct Graph *graph) {
    if (graph) {
        free(graph->adj_matrix);
        free(graph);
    }
}

int add_edge(struct Graph *graph, int from, int to, int weight) {
    if (!graph || from < 0 || to < 0 || from >= graph->num_nodes || to >= graph->num_nodes) return 0;
    if (from == to) return 0;
    
    int *matrix = graph->adj_matrix;
    *(matrix + from * graph->num_nodes + to) = weight;
    *(matrix + to * graph->num_nodes + from) = weight;
    
    graph->num_edges++;
    return 1;
}

void dfs_util(struct Graph *graph, int node, int *visited) {
    if (!graph || !visited || node < 0 || node >= graph->num_nodes) return;
    
    visited[node] = 1;
    printf("%d ", node);
    
    int *matrix = graph->adj_matrix;
    for (int i = 0; i < graph->num_nodes; i++) {
        int weight = *(matrix + node * graph->num_nodes + i);
        if (weight != 0 && !visited[i]) {
            dfs_util(graph, i, visited);
        }
    }
}

void depth_first_search(struct Graph *graph, int start) {
    if (!graph || start < 0 || start >= graph->num_nodes) return;
    
    int visited[MAX_NODES];
    memset(visited, 0, sizeof(visited));
    
    printf("DFS traversal from node %d: ", start);
    dfs_util(graph, start, visited);
    printf("\n");
}

void dijkstra(struct Graph *graph, int source) {
    if (!graph || source < 0 || source >= graph->num_nodes) return;
    
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
        
        int *matrix = graph->adj_matrix;
        for (int v = 0; v < graph->num_nodes; v++) {
            int weight = *(matrix + min_node * graph->num_nodes + v);
            if (!visited[v] && weight != 0 && dist[min_node] != INT_MAX) {
                int new_dist = dist[min_node] + weight;
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

int main() {
    struct Graph *graph = create_graph(6);
    if (!graph) {
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
    add_edge(graph, 4, 5, 3);
    
    printf("Graph with %d nodes and %d edges created\n", graph->num_nodes, graph->num_edges);
    
    depth_first_search(graph, 0);
    printf("\n");
    dijkstra(graph,