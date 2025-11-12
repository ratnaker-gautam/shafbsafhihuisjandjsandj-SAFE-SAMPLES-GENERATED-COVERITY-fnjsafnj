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
        nodes = MAX_NODES;
    }
    g->node_count = nodes;
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            g->adjacency[i][j] = (i == j) ? 0 : INF;
        }
    }
}

int add_edge(Graph *g, int from, int to, int weight) {
    if (g == NULL || from < 0 || to < 0 || from >= g->node_count || to >= g->node_count) {
        return -1;
    }
    if (weight < 0 || weight > 10000) {
        return -1;
    }
    g->adjacency[from][to] = weight;
    return 0;
}

int shortest_path_dijkstra(Graph *g, int start, int end) {
    if (g == NULL || start < 0 || end < 0 || start >= g->node_count || end >= g->node_count) {
        return -1;
    }
    
    int dist[MAX_NODES];
    int visited[MAX_NODES];
    
    for (int i = 0; i < g->node_count; i++) {
        dist[i] = INF;
        visited[i] = 0;
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
        
        if (min_index == -1 || min_dist == INF) {
            break;
        }
        
        visited[min_index] = 1;
        
        for (int v = 0; v < g->node_count; v++) {
            if (!visited[v] && g->adjacency[min_index][v] != INF && dist[min_index] != INF) {
                int new_dist = dist[min_index] + g->adjacency[min_index][v];
                if (new_dist > 0 && new_dist < dist[v]) {
                    dist[v] = new_dist;
                }
            }
        }
    }
    
    return (dist[end] == INF) ? -1 : dist[end];
}

int has_path_dfs(Graph *g, int start, int end) {
    if (g == NULL || start < 0 || end < 0 || start >= g->node_count || end >= g->node_count) {
        return 0;
    }
    
    int visited[MAX_NODES] = {0};
    int stack[MAX_NODES];
    int top = -1;
    
    stack[++top] = start;
    visited[start] = 1;
    
    while (top >= 0) {
        int current = stack[top--];
        
        if (current == end) {
            return 1;
        }
        
        for (int i = 0; i < g->node_count; i++) {
            if (g->adjacency[current][i] != INF && !visited[i]) {
                visited[i] = 1;
                if (top + 1 < MAX_NODES) {
                    stack[++top] = i;
                }
            }
        }
    }
    
    return 0;
}

int execute_graph_operation(GraphOperation op, Graph *g, int start, int end) {
    if (op == NULL || g == NULL) {
        return -1;
    }
    return op(g, start, end);
}

int main() {
    Graph g;
    int nodes, edges;
    
    printf("Enter number of nodes (1-100): ");
    if (scanf("%d", &nodes) != 1 || nodes < 1 || nodes > MAX_NODES) {
        printf("Invalid input\n");
        return 1;
    }
    
    initialize_graph(&g, nodes);
    
    printf("Enter number of edges: ");
    if (scanf("%d", &edges) != 1 || edges < 0 || edges > nodes * nodes) {
        printf("Invalid input\n");
        return 1;
    }
    
    for (int i = 0; i < edges; i++) {
        int from, to, weight;
        printf("Enter edge %d (from to weight): ", i + 1);
        if (scanf("%d %d %d", &from, &to, &weight) != 3) {
            printf("Invalid input\n");
            return 1;
        }
        if (add_edge(&g, from, to, weight) != 0) {
            printf("Invalid edge\n");
            return 1;
        }
    }
    
    int start, end;
    printf("Enter start and end nodes: ");
    if (scanf("%d %d", &start, &end) != 2) {
        printf("Invalid input\n");
        return 1;
    }
    
    GraphOperation