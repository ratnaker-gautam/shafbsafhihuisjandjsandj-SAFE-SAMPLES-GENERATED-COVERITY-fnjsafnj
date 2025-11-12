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
    if (g == NULL || nodes <= 0 || nodes > MAX_NODES) return;
    g->node_count = nodes;
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            g->adjacency[i][j] = (i == j) ? 0 : INF;
        }
    }
}

void add_edge(Graph *g, int u, int v, int weight) {
    if (g == NULL || u < 0 || u >= g->node_count || v < 0 || v >= g->node_count) return;
    if (weight < 0 || weight > 10000) return;
    g->adjacency[u][v] = weight;
    g->adjacency[v][u] = weight;
}

void dijkstra_operation(Graph *g, int start, int target) {
    if (g == NULL || start < 0 || start >= g->node_count || target < 0 || target >= g->node_count) return;
    
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
            if (!visited[v] && g->adjacency[min_index][v] != INF && dist[min_index] != INF) {
                long new_dist = (long)dist[min_index] + g->adjacency[min_index][v];
                if (new_dist < dist[v] && new_dist <= INT_MAX) {
                    dist[v] = (int)new_dist;
                }
            }
        }
    }
    
    if (dist[target] != INF) {
        printf("Shortest distance from %d to %d: %d\n", start, target, dist[target]);
    } else {
        printf("No path exists from %d to %d\n", start, target);
    }
}

void bfs_operation(Graph *g, int start, int target) {
    if (g == NULL || start < 0 || start >= g->node_count || target < 0 || target >= g->node_count) return;
    
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
        
        if (current == target) {
            printf("BFS: Path exists from %d to %d\n", start, target);
            return;
        }
        
        for (int v = 0; v < g->node_count; v++) {
            if (!visited[v] && g->adjacency[current][v] != INF && g->adjacency[current][v] > 0) {
                visited[v] = 1;
                parent[v] = current;
                if (rear < MAX_NODES) {
                    queue[rear++] = v;
                }
            }
        }
    }
    
    printf("BFS: No path exists from %d to %d\n", start, target);
}

void execute_graph_operation(GraphOperation op, Graph *g, int start, int target) {
    if (op != NULL && g != NULL) {
        op(g, start, target);
    }
}

int main() {
    Graph g;
    int nodes, edges;
    
    printf("Enter number of nodes (1-100): ");
    if (scanf("%d", &nodes) != 1 || nodes < 1 || nodes > MAX_NODES) {
        printf("Invalid number of nodes\n");
        return 1;
    }
    
    initialize_graph(&g, nodes);
    
    printf("Enter number of edges: ");
    if (scanf("%d", &edges) != 1 || edges < 0 || edges > nodes * (nodes - 1) / 2) {
        printf("Invalid number of edges\n");
        return 1;
    }
    
    for (int i = 0; i < edges; i++) {
        int u, v, weight;
        printf("Enter edge %d (u v weight): ", i + 1);
        if (scanf("%d %d %d", &u, &v, &weight)