//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_NODES 100

typedef struct {
    int adjacency[MAX_NODES][MAX_NODES];
    int node_count;
} Graph;

typedef void (*GraphOperation)(Graph*, int, int);

void initialize_graph(Graph *g, int node_count) {
    if (node_count <= 0 || node_count > MAX_NODES) {
        fprintf(stderr, "Invalid node count\n");
        exit(EXIT_FAILURE);
    }
    g->node_count = node_count;
    for (int i = 0; i < node_count; i++) {
        for (int j = 0; j < node_count; j++) {
            g->adjacency[i][j] = 0;
        }
    }
}

void add_edge(Graph *g, int from, int to, int weight) {
    if (from < 0 || from >= g->node_count || to < 0 || to >= g->node_count) {
        fprintf(stderr, "Invalid node index\n");
        exit(EXIT_FAILURE);
    }
    if (weight < 0 || weight > 1000) {
        fprintf(stderr, "Invalid edge weight\n");
        exit(EXIT_FAILURE);
    }
    g->adjacency[from][to] = weight;
    g->adjacency[to][from] = weight;
}

void print_adjacency_matrix(Graph *g, int start, int end) {
    if (g->node_count <= 0) return;
    printf("Adjacency Matrix:\n");
    for (int i = 0; i < g->node_count; i++) {
        for (int j = 0; j < g->node_count; j++) {
            printf("%3d ", g->adjacency[i][j]);
        }
        printf("\n");
    }
}

void find_shortest_path(Graph *g, int start, int end) {
    if (start < 0 || start >= g->node_count || end < 0 || end >= g->node_count) {
        fprintf(stderr, "Invalid start or end node\n");
        return;
    }
    
    int dist[MAX_NODES];
    int visited[MAX_NODES];
    int prev[MAX_NODES];
    
    for (int i = 0; i < g->node_count; i++) {
        dist[i] = INT_MAX;
        visited[i] = 0;
        prev[i] = -1;
    }
    
    dist[start] = 0;
    
    for (int count = 0; count < g->node_count - 1; count++) {
        int min_dist = INT_MAX;
        int min_index = -1;
        
        for (int v = 0; v < g->node_count; v++) {
            if (!visited[v] && dist[v] <= min_dist) {
                min_dist = dist[v];
                min_index = v;
            }
        }
        
        if (min_index == -1) break;
        visited[min_index] = 1;
        
        for (int v = 0; v < g->node_count; v++) {
            if (!visited[v] && g->adjacency[min_index][v] != 0) {
                long new_dist = (long)dist[min_index] + g->adjacency[min_index][v];
                if (new_dist < INT_MAX && new_dist < dist[v]) {
                    dist[v] = (int)new_dist;
                    prev[v] = min_index;
                }
            }
        }
    }
    
    if (dist[end] == INT_MAX) {
        printf("No path exists from %d to %d\n", start, end);
    } else {
        printf("Shortest distance from %d to %d: %d\n", start, end, dist[end]);
        printf("Path: ");
        int path[MAX_NODES];
        int path_len = 0;
        int current = end;
        
        while (current != -1) {
            path[path_len++] = current;
            current = prev[current];
        }
        
        for (int i = path_len - 1; i >= 0; i--) {
            printf("%d", path[i]);
            if (i > 0) printf(" -> ");
        }
        printf("\n");
    }
}

void find_maximum_flow(Graph *g, int source, int sink) {
    if (source < 0 || source >= g->node_count || sink < 0 || sink >= g->node_count) {
        fprintf(stderr, "Invalid source or sink node\n");
        return;
    }
    
    int residual[MAX_NODES][MAX_NODES];
    for (int i = 0; i < g->node_count; i++) {
        for (int j = 0; j < g->node_count; j++) {
            residual[i][j] = g->adjacency[i][j];
        }
    }
    
    int parent[MAX_NODES];
    int max_flow = 0;
    
    while (1) {
        for (int i = 0; i < g->node_count; i++) {
            parent[i] = -1;
        }
        
        int queue[MAX_NODES];
        int front = 0, rear = 0;
        queue[rear++] = source;
        parent[source] = -2;
        
        while (front < rear) {
            int u = queue[front++];
            for (int v = 0; v < g->node_count; v++) {