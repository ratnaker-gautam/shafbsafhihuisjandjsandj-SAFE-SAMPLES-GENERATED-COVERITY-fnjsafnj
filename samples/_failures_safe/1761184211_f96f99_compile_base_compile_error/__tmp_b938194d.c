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

void breadth_first_search(struct Graph* graph, int start) {
    if (graph == NULL) return;
    if (start < 0 || start >= graph->num_nodes) return;
    
    int visited[MAX_NODES];
    for (int i = 0; i < MAX_NODES; i++) {
        visited[i] = 0;
    }
    
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    
    visited[start] = 1;
    queue[rear++] = start;
    
    printf("BFS traversal: ");
    
    while (front < rear) {
        int current = queue[front++];
        printf("%d ", current);
        
        for (int i = 0; i < graph->num_nodes; i++) {
            if (graph->adjacency[current][i] && !visited[i]) {
                visited[i] = 1;
                queue[rear++] = i;
            }
        }
    }
    printf("\n");
}

int find_min_distance(int dist[], int visited[], int n) {
    int min = INT_MAX;
    int min_index = -1;
    
    for (int v = 0; v < n; v++) {
        if (!visited[v] && dist[v] <= min) {
            min = dist[v];
            min_index = v;
        }
    }
    
    return min_index;
}

void dijkstra_shortest_path(struct Graph* graph, int start) {
    if (graph == NULL) return;
    if (start < 0 || start >= graph->num_nodes) return;
    
    int dist[MAX_NODES];
    int visited[MAX_NODES];
    
    for (int i = 0; i < graph->num_nodes; i++) {
        dist[i] = INT_MAX;
        visited[i] = 0;
    }
    
    dist[start] = 0;
    
    for (int count = 0; count < graph->num_nodes - 1; count++) {
        int u = find_min_distance(dist, visited, graph->num_nodes);
        if (u == -1) break;
        
        visited[u] = 1;
        
        for (int v = 0; v < graph->num_nodes; v++) {
            if (!visited[v] && graph->adjacency[u][v] && dist[u] != INT_MAX) {
                long new_dist = (long)dist[u] + graph->edge_weights[u][v];
                if (new_dist < dist[v] && new_dist <= INT_MAX) {
                    dist[v] = (int)new_dist;
                }
            }
        }
    }
    
    printf("Shortest distances from node %d:\n", start);
    for (int i = 0; i < graph->num_nodes; i++) {
        if (dist[i] == INT_MAX) {
            printf("Node %d: unreachable\n", i);