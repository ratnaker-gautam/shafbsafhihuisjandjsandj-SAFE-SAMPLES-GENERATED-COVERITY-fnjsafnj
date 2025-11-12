//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: function_pointers ; Variation: graph_algorithms
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

typedef void (*GraphOperation)(Graph*, int, int);

void initialize_graph(Graph *g, int nodes) {
    if (nodes <= 0 || nodes > MAX_NODES) {
        fprintf(stderr, "Invalid node count\n");
        exit(1);
    }
    g->node_count = nodes;
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            g->adjacency[i][j] = (i == j) ? 0 : INF;
        }
    }
}

void add_edge(Graph *g, int u, int v, int weight) {
    if (u < 0 || u >= g->node_count || v < 0 || v >= g->node_count) {
        fprintf(stderr, "Invalid edge nodes\n");
        return;
    }
    if (weight < 0) {
        fprintf(stderr, "Negative weights not allowed\n");
        return;
    }
    g->adjacency[u][v] = weight;
    g->adjacency[v][u] = weight;
}

void dijkstra_operation(Graph *g, int start, int end) {
    if (start < 0 || start >= g->node_count || end < 0 || end >= g->node_count) {
        fprintf(stderr, "Invalid start or end node\n");
        return;
    }
    
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
        
        if (min_index == -1 || min_dist == INF) break;
        
        visited[min_index] = 1;
        
        for (int v = 0; v < g->node_count; v++) {
            if (!visited[v] && g->adjacency[min_index][v] != INF && 
                dist[min_index] != INF) {
                long new_dist = (long)dist[min_index] + g->adjacency[min_index][v];
                if (new_dist < dist[v] && new_dist <= INT_MAX) {
                    dist[v] = (int)new_dist;
                }
            }
        }
    }
    
    if (dist[end] != INF) {
        printf("Shortest path from %d to %d: %d\n", start, end, dist[end]);
    } else {
        printf("No path exists from %d to %d\n", start, end);
    }
}

void bfs_operation(Graph *g, int start, int end) {
    if (start < 0 || start >= g->node_count || end < 0 || end >= g->node_count) {
        fprintf(stderr, "Invalid start or end node\n");
        return;
    }
    
    int visited[MAX_NODES] = {0};
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    int parent[MAX_NODES];
    int distance[MAX_NODES];
    
    for (int i = 0; i < g->node_count; i++) {
        parent[i] = -1;
        distance[i] = -1;
    }
    
    visited[start] = 1;
    distance[start] = 0;
    queue[rear++] = start;
    
    while (front < rear) {
        int current = queue[front++];
        
        if (current == end) break;
        
        for (int neighbor = 0; neighbor < g->node_count; neighbor++) {
            if (g->adjacency[current][neighbor] != INF && !visited[neighbor]) {
                visited[neighbor] = 1;
                parent[neighbor] = current;
                distance[neighbor] = distance[current] + 1;
                if (rear < MAX_NODES) {
                    queue[rear++] = neighbor;
                }
            }
        }
    }
    
    if (distance[end] != -1) {
        printf("BFS distance from %d to %d: %d\n", start, end, distance[end]);
    } else {
        printf("No BFS path from %d to %d\n", start, end);
    }
}

void execute_operation(Graph *g, GraphOperation op, int start, int end) {
    if (op == NULL) {
        fprintf(stderr, "Null operation\n");
        return;
    }
    op(g, start, end);
}

int main() {
    Graph g;
    int nodes, edges;
    
    printf("Enter number of nodes (1-100): ");
    if (scanf("%d", &nodes) != 1 || nodes < 1 || nodes > MAX_NODES) {
        fprintf(stderr, "Invalid node count\n");
        return 1;
    }
    
    initialize_graph(&g, nodes