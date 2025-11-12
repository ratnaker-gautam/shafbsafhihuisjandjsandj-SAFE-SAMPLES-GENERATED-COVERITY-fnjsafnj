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
    if (g == NULL || nodes <= 0 || nodes > MAX_NODES) return;
    g->node_count = nodes;
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
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
    
    int visited[MAX_NODES] = {0};
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    int dist[MAX_NODES];
    
    for (int i = 0; i < g->node_count; i++) {
        dist[i] = INF;
    }
    
    visited[start] = 1;
    dist[start] = 0;
    queue[rear++] = start;
    
    while (front < rear) {
        int current = queue[front++];
        
        if (current == end) break;
        
        for (int i = 0; i < g->node_count; i++) {
            if (g->adjacency[current][i] != INF && !visited[i]) {
                visited[i] = 1;
                dist[i] = dist[current] + 1;
                queue[rear++] = i;
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
    add_edge(&graph, 4, 5, 3);
    
    printf("Graph with %d nodes created.\n", nodes);
    printf("Available operations:\n");
    printf("1. Dijkstra shortest path\n");
    printf("2. BFS shortest path (unweighted)\n");
    
    int choice;
    printf("Enter operation choice (1 or 2): ");
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input.\n");
        return 1;
    }
    
    int start, end;
    printf("Enter start node (0-%d): ", nodes - 1);
    if (scanf("%d", &start) != 1 || start < 0 || start >= nodes) {