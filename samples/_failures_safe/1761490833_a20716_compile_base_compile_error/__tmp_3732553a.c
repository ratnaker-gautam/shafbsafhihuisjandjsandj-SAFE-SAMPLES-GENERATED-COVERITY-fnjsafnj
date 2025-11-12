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
        fprintf(stderr, "Invalid node index\n");
        return;
    }
    if (weight < 0) {
        fprintf(stderr, "Negative weights not allowed\n");
        return;
    }
    g->adjacency[u][v] = weight;
    g->adjacency[v][u] = weight;
}

int dijkstra_shortest_path(Graph *g, int start, int end) {
    if (start < 0 || start >= g->node_count || end < 0 || end >= g->node_count) {
        return -1;
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
        
        if (min_index == -1 || min_index == end) break;
        
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
    
    return (dist[end] == INF) ? -1 : dist[end];
}

int bfs_shortest_path(Graph *g, int start, int end) {
    if (start < 0 || start >= g->node_count || end < 0 || end >= g->node_count) {
        return -1;
    }
    
    int visited[MAX_NODES] = {0};
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    int dist[MAX_NODES];
    
    for (int i = 0; i < g->node_count; i++) {
        dist[i] = -1;
    }
    
    visited[start] = 1;
    dist[start] = 0;
    queue[rear++] = start;
    
    while (front < rear) {
        int current = queue[front++];
        
        if (current == end) {
            return dist[current];
        }
        
        for (int neighbor = 0; neighbor < g->node_count; neighbor++) {
            if (g->adjacency[current][neighbor] == 1 && !visited[neighbor]) {
                visited[neighbor] = 1;
                dist[neighbor] = dist[current] + 1;
                queue[rear++] = neighbor;
            }
        }
    }
    
    return -1;
}

int execute_graph_operation(GraphOperation op, Graph *g, int start, int end) {
    if (op == NULL) {
        return -1;
    }
    return op(g, start, end);
}

int main() {
    Graph g;
    initialize_graph(&g, 6);
    
    add_edge(&g, 0, 1, 4);
    add_edge(&g, 0, 2, 2);
    add_edge(&g, 1, 2, 1);
    add_edge(&g, 1, 3, 5);
    add_edge(&g, 2, 3, 8);
    add_edge(&g, 2, 4, 10);
    add_edge(&g, 3, 4, 2);
    add_edge(&g, 3, 5, 6);
    add_edge(&g, 4, 5, 3);
    
    GraphOperation operations[] = {dijkstra_shortest_path, bfs_shortest_path};
    char* operation_names[] = {"Dijkstra", "BFS"};
    int operation_count = sizeof(operations) / sizeof(operations[0]);
    
    printf("Graph shortest path results:\n");
    printf("Nodes: 0-5, Testing paths from node