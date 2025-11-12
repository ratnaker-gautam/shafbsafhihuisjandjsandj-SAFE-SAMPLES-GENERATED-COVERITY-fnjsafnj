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
    if (g == NULL || from < 0 || from >= g->node_count || 
        to < 0 || to >= g->node_count || weight < 0) return 0;
    g->adjacency[from][to] = weight;
    return 1;
}

int dijkstra_operation(Graph *g, int start, int target) {
    if (g == NULL || start < 0 || start >= g->node_count || 
        target < 0 || target >= g->node_count) return -1;
    
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
        
        if (min_index == -1) break;
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
    
    return (dist[target] == INF) ? -1 : dist[target];
}

int bfs_shortest_operation(Graph *g, int start, int target) {
    if (g == NULL || start < 0 || start >= g->node_count || 
        target < 0 || target >= g->node_count) return -1;
    
    int visited[MAX_NODES] = {0};
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    int distance[MAX_NODES];
    
    for (int i = 0; i < g->node_count; i++) {
        distance[i] = -1;
    }
    
    visited[start] = 1;
    distance[start] = 0;
    queue[rear++] = start;
    
    while (front < rear) {
        int current = queue[front++];
        
        if (current == target) {
            return distance[current];
        }
        
        for (int neighbor = 0; neighbor < g->node_count; neighbor++) {
            if (g->adjacency[current][neighbor] == 1 && !visited[neighbor]) {
                visited[neighbor] = 1;
                distance[neighbor] = distance[current] + 1;
                queue[rear++] = neighbor;
            }
        }
    }
    
    return -1;
}

int execute_graph_algorithm(GraphOperation op, Graph *g, int start, int target) {
    if (op == NULL || g == NULL) return -1;
    return op(g, start, target);
}

int main() {
    Graph g;
    int nodes, edges, start, target;
    
    printf("Enter number of nodes (1-100): ");
    if (scanf("%d", &nodes) != 1 || nodes < 1 || nodes > MAX_NODES) {
        printf("Invalid number of nodes\n");
        return 1;
    }
    
    initialize_graph(&g, nodes);
    
    printf("Enter number of edges: ");
    if (scanf("%d", &edges) != 1 || edges < 0 || edges > nodes * nodes) {
        printf("Invalid number of edges\n");
        return 1;
    }
    
    printf("Enter edges as 'from to weight' (0-based indices):\n");
    for (int i = 0; i < edges; i++) {
        int from, to, weight;
        if (scanf("%d %d %d", &from, &to, &weight) != 3 || 
            from < 0 || from >= nodes || to < 0 || to >= nodes || weight < 0) {
            printf("Invalid edge\n");
            return 1;
        }
        if (!add_edge(&g, from, to, weight)) {
            printf("Failed to add edge\n");
            return 1;
        }
    }
    
    printf("