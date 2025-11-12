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
    if (g == NULL || u < 0 || u >= g->node_count || v < 0 || v >= g->node_count) {
        fprintf(stderr, "Invalid edge parameters\n");
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
    if (g == NULL || start < 0 || start >= g->node_count || end < 0 || end >= g->node_count) {
        fprintf(stderr, "Invalid Dijkstra parameters\n");
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
                dist[min_index] != INF && 
                dist[min_index] + g->adjacency[min_index][v] < dist[v]) {
                dist[v] = dist[min_index] + g->adjacency[min_index][v];
            }
        }
    }
    
    if (dist[end] == INF) {
        printf("No path from %d to %d\n", start, end);
    } else {
        printf("Shortest distance from %d to %d: %d\n", start, end, dist[end]);
    }
}

void bfs_operation(Graph *g, int start, int end) {
    if (g == NULL || start < 0 || start >= g->node_count || end < 0 || end >= g->node_count) {
        fprintf(stderr, "Invalid BFS parameters\n");
        return;
    }
    
    int visited[MAX_NODES] = {0};
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    int parent[MAX_NODES];
    
    for (int i = 0; i < g->node_count; i++) {
        parent[i] = -1;
    }
    
    visited[start] = 1;
    queue[rear++] = start;
    
    while (front < rear) {
        int current = queue[front++];
        
        if (current == end) {
            printf("Path exists from %d to %d\n", start, end);
            return;
        }
        
        for (int v = 0; v < g->node_count; v++) {
            if (g->adjacency[current][v] != INF && !visited[v]) {
                visited[v] = 1;
                parent[v] = current;
                if (rear < MAX_NODES) {
                    queue[rear++] = v;
                }
            }
        }
    }
    
    printf("No path from %d to %d\n", start, end);
}

void execute_graph_operation(Graph *g, GraphOperation op, int start, int end) {
    if (g == NULL || op == NULL) {
        fprintf(stderr, "Invalid operation parameters\n");
        return;
    }
    op(g, start, end);
}

int main() {
    Graph graph;
    int nodes = 6;
    
    initialize_graph(&graph, nodes);
    
    add_edge(&graph, 0, 1, 4);
    add_edge(&graph, 0, 2, 2);
    add_edge(&graph, 1, 2, 1);
    add_edge(&graph, 1, 3, 5);
    add_edge(&graph, 2, 3, 8);
    add_edge(&graph, 2, 4, 10);
    add_edge(&graph, 3, 4, 2);
    add_edge(&graph, 3, 5, 6);