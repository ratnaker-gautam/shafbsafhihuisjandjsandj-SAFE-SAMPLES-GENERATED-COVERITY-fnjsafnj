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

void add_edge(Graph *g, int from, int to, int weight) {
    if (from < 0 || from >= g->node_count || to < 0 || to >= g->node_count) {
        fprintf(stderr, "Invalid edge nodes\n");
        return;
    }
    if (weight < 0) {
        fprintf(stderr, "Negative weight not allowed\n");
        return;
    }
    g->adjacency[from][to] = weight;
}

void dijkstra_operation(Graph *g, int start, int target) {
    if (start < 0 || start >= g->node_count || target < 0 || target >= g->node_count) {
        fprintf(stderr, "Invalid start or target node\n");
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
        int min_node = -1;
        
        for (int v = 0; v < g->node_count; v++) {
            if (!visited[v] && dist[v] <= min_dist) {
                min_dist = dist[v];
                min_node = v;
            }
        }
        
        if (min_node == -1) break;
        visited[min_node] = 1;
        
        for (int v = 0; v < g->node_count; v++) {
            if (!visited[v] && g->adjacency[min_node][v] != INF && dist[min_node] != INF) {
                long new_dist = (long)dist[min_node] + g->adjacency[min_node][v];
                if (new_dist < dist[v] && new_dist <= INT_MAX) {
                    dist[v] = (int)new_dist;
                }
            }
        }
    }
    
    if (dist[target] != INF) {
        printf("Shortest path from %d to %d: %d\n", start, target, dist[target]);
    } else {
        printf("No path exists from %d to %d\n", start, target);
    }
}

void bfs_operation(Graph *g, int start, int target) {
    if (start < 0 || start >= g->node_count || target < 0 || target >= g->node_count) {
        fprintf(stderr, "Invalid start or target node\n");
        return;
    }
    
    int visited[MAX_NODES] = {0};
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    
    visited[start] = 1;
    queue[rear++] = start;
    
    while (front < rear) {
        int current = queue[front++];
        
        if (current == target) {
            printf("Node %d is reachable from %d via BFS\n", target, start);
            return;
        }
        
        for (int i = 0; i < g->node_count; i++) {
            if (g->adjacency[current][i] != INF && !visited[i]) {
                visited[i] = 1;
                if (rear < MAX_NODES) {
                    queue[rear++] = i;
                }
            }
        }
    }
    
    printf("Node %d is not reachable from %d via BFS\n", target, start);
}

void execute_operation(Graph *g, GraphOperation op, int start, int target) {
    if (op == NULL) {
        fprintf(stderr, "Invalid operation\n");
        return;
    }
    op(g, start, target);
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
    
    printf("Performing Dijkstra's algorithm:\n");
    execute_operation(&g, dijkstra_operation, 0, 5);
    
    printf("\nPerforming BFS reachability:\n");
    execute