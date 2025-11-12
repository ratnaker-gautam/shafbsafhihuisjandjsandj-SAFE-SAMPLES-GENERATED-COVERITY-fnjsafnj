//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_NODES 100
#define INF INT_MAX

typedef struct {
    int adjacency[MAX_NODES][MAX_NODES];
    int node_count;
} Graph;

typedef int (*GraphOperation)(Graph*, int, int);

void initialize_graph(Graph *g, int node_count) {
    if (g == NULL || node_count <= 0 || node_count > MAX_NODES) return;
    g->node_count = node_count;
    for (int i = 0; i < node_count; i++) {
        for (int j = 0; j < node_count; j++) {
            g->adjacency[i][j] = (i == j) ? 0 : INF;
        }
    }
}

int add_edge(Graph *g, int from, int to, int weight) {
    if (g == NULL || from < 0 || to < 0 || from >= g->node_count || to >= g->node_count) return 0;
    if (weight < 0) return 0;
    g->adjacency[from][to] = weight;
    return 1;
}

int dijkstra_shortest_path(Graph *g, int start, int end) {
    if (g == NULL || start < 0 || end < 0 || start >= g->node_count || end >= g->node_count) return INF;
    
    int dist[MAX_NODES];
    int visited[MAX_NODES] = {0};
    
    for (int i = 0; i < g->node_count; i++) {
        dist[i] = INF;
    }
    dist[start] = 0;
    
    for (int count = 0; count < g->node_count - 1; count++) {
        int min_dist = INF;
        int min_index = -1;
        
        for (int v = 0; v < g->node_count; v++) {
            if (!visited[v] && dist[v] <= min_dist) {
                min_dist = dist[v];
                min_index = v;
            }
        }
        
        if (min_index == -1 || min_index == end) break;
        visited[min_index] = 1;
        
        for (int v = 0; v < g->node_count; v++) {
            if (!visited[v] && g->adjacency[min_index][v] != INF && dist[min_index] != INF) {
                long new_dist = (long)dist[min_index] + g->adjacency[min_index][v];
                if (new_dist < dist[v] && new_dist <= INT_MAX) {
                    dist[v] = (int)new_dist;
                }
            }
        }
    }
    
    return dist[end];
}

int bfs_shortest_path(Graph *g, int start, int end) {
    if (g == NULL || start < 0 || end < 0 || start >= g->node_count || end >= g->node_count) return INF;
    
    int dist[MAX_NODES];
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    
    for (int i = 0; i < g->node_count; i++) {
        dist[i] = INF;
    }
    
    dist[start] = 0;
    queue[rear++] = start;
    
    while (front < rear) {
        int current = queue[front++];
        
        if (current == end) break;
        
        for (int v = 0; v < g->node_count; v++) {
            if (g->adjacency[current][v] != INF && g->adjacency[current][v] > 0 && dist[v] == INF) {
                dist[v] = dist[current] + 1;
                if (rear < MAX_NODES) {
                    queue[rear++] = v;
                }
            }
        }
    }
    
    return dist[end];
}

int execute_graph_operation(GraphOperation op, Graph *g, int start, int end) {
    if (op == NULL || g == NULL) return INF;
    return op(g, start, end);
}

int main() {
    Graph g;
    int node_count, edge_count;
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &node_count) != 1 || node_count < 1 || node_count > MAX_NODES) {
        printf("Invalid node count\n");
        return 1;
    }
    
    initialize_graph(&g, node_count);
    
    printf("Enter number of edges: ");
    if (scanf("%d", &edge_count) != 1 || edge_count < 0) {
        printf("Invalid edge count\n");
        return 1;
    }
    
    printf("Enter edges as 'from to weight' (0-based indices):\n");
    for (int i = 0; i < edge_count; i++) {
        int from, to, weight;
        if (scanf("%d %d %d", &from, &to, &weight) != 3) {
            printf("Invalid edge input\n");
            return 1;
        }
        if (!add_edge(&g, from, to, weight)) {
            printf("Invalid edge parameters\n");
            return 1;
        }
    }
    
    int start, end;
    printf("Enter start and end nodes: ");
    if (scanf("%d %d", &start, &end) != 2) {
        printf("Invalid node input\n");
        return