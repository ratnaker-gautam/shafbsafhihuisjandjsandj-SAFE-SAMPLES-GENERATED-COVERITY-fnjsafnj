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

int dijkstra_shortest_path(Graph *g, int start, int end) {
    if (g == NULL || start < 0 || start >= g->node_count || 
        end < 0 || end >= g->node_count) return INF;
    
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
    
    return dist[end];
}

int has_path_exists(Graph *g, int start, int end) {
    if (g == NULL || start < 0 || start >= g->node_count || 
        end < 0 || end >= g->node_count) return 0;
    
    int visited[MAX_NODES] = {0};
    int stack[MAX_NODES];
    int top = -1;
    
    stack[++top] = start;
    visited[start] = 1;
    
    while (top >= 0) {
        int current = stack[top--];
        if (current == end) return 1;
        
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
    if (op == NULL || g == NULL) return -1;
    return op(g, start, end);
}

int main() {
    Graph graph;
    int nodes, edges;
    
    printf("Enter number of nodes (1-100): ");
    if (scanf("%d", &nodes) != 1 || nodes < 1 || nodes > MAX_NODES) {
        printf("Invalid number of nodes\n");
        return 1;
    }
    
    initialize_graph(&graph, nodes);
    
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
        if (!add_edge(&graph, from, to, weight)) {
            printf("Failed to add edge\n");
            return 1;
        }
    }
    
    int start, end;
    printf("Enter start and end nodes: ");
    if (scanf("%d %d", &start, &end) != 2 || 
        start < 0 || start >= nodes || end < 0 || end >= nodes) {
        printf("Invalid nodes\n");
        return 1;
    }
    
    GraphOperation