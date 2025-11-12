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
    if (g == NULL || node_count <= 0 || node_count > MAX_NODES) {
        return;
    }
    g->node_count = node_count;
    for (int i = 0; i < node_count; i++) {
        for (int j = 0; j < node_count; j++) {
            g->adjacency[i][j] = (i == j) ? 0 : INF;
        }
    }
}

int add_edge(Graph *g, int from, int to, int weight) {
    if (g == NULL || from < 0 || from >= g->node_count || 
        to < 0 || to >= g->node_count || weight < 0) {
        return 0;
    }
    g->adjacency[from][to] = weight;
    return 1;
}

int dijkstra_operation(Graph *g, int start, int target) {
    if (g == NULL || start < 0 || start >= g->node_count || 
        target < 0 || target >= g->node_count) {
        return -1;
    }
    
    int dist[MAX_NODES];
    int visited[MAX_NODES];
    
    for (int i = 0; i < g->node_count; i++) {
        dist[i] = INF;
        visited[i] = 0;
    }
    dist[start] = 0;
    
    for (int count = 0; count < g->node_count; count++) {
        int min_dist = INF;
        int min_node = -1;
        
        for (int v = 0; v < g->node_count; v++) {
            if (!visited[v] && dist[v] < min_dist) {
                min_dist = dist[v];
                min_node = v;
            }
        }
        
        if (min_node == -1 || min_node == target) {
            break;
        }
        
        visited[min_node] = 1;
        
        for (int v = 0; v < g->node_count; v++) {
            if (!visited[v] && g->adjacency[min_node][v] != INF) {
                long new_dist = (long)dist[min_node] + g->adjacency[min_node][v];
                if (new_dist < dist[v] && new_dist <= INT_MAX) {
                    dist[v] = (int)new_dist;
                }
            }
        }
    }
    
    return (dist[target] == INF) ? -1 : dist[target];
}

int bfs_shortest_path(Graph *g, int start, int target) {
    if (g == NULL || start < 0 || start >= g->node_count || 
        target < 0 || target >= g->node_count) {
        return -1;
    }
    
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    int dist[MAX_NODES];
    
    for (int i = 0; i < g->node_count; i++) {
        dist[i] = -1;
    }
    
    dist[start] = 0;
    queue[rear++] = start;
    
    while (front < rear) {
        int current = queue[front++];
        
        if (current == target) {
            return dist[current];
        }
        
        for (int v = 0; v < g->node_count; v++) {
            if (g->adjacency[current][v] != INF && dist[v] == -1) {
                dist[v] = dist[current] + 1;
                if (rear < MAX_NODES) {
                    queue[rear++] = v;
                }
            }
        }
    }
    
    return -1;
}

int execute_graph_algorithm(GraphOperation op, Graph *g, int start, int target) {
    if (op == NULL || g == NULL) {
        return -1;
    }
    return op(g, start, target);
}

int main() {
    Graph graph;
    int node_count, edge_count;
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &node_count) != 1 || node_count < 1 || node_count > MAX_NODES) {
        printf("Invalid number of nodes\n");
        return 1;
    }
    
    initialize_graph(&graph, node_count);
    
    printf("Enter number of edges: ");
    if (scanf("%d", &edge_count) != 1 || edge_count < 0) {
        printf("Invalid number of edges\n");
        return 1;
    }
    
    printf("Enter edges as 'from to weight' (0-based indices):\n");
    for (int i = 0; i < edge_count; i++) {
        int from, to, weight;
        if (scanf("%d %d %d", &from, &to, &weight) != 3) {
            printf("Invalid edge input\n");
            return 1;
        }
        if (!add_edge(&graph, from, to, weight)) {
            printf("Invalid edge parameters\n");
            return 1;
        }
    }
    
    int start, target;
    printf("Enter start and target nodes: ");