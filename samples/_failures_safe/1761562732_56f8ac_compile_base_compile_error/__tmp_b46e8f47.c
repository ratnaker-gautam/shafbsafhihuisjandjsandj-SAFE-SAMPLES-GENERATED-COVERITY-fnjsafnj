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

void add_edge(Graph *g, int from, int to, int weight) {
    if (from < 0 || from >= g->node_count || to < 0 || to >= g->node_count) {
        fprintf(stderr, "Invalid node index\n");
        return;
    }
    if (weight < 0) {
        fprintf(stderr, "Negative weights not allowed\n");
        return;
    }
    g->adjacency[from][to] = weight;
}

int shortest_path_dijkstra(Graph *g, int start, int end) {
    if (start < 0 || start >= g->node_count || end < 0 || end >= g->node_count) {
        fprintf(stderr, "Invalid start or end node\n");
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
                dist[min_index] != INF && 
                dist[min_index] + g->adjacency[min_index][v] < dist[v]) {
                dist[v] = dist[min_index] + g->adjacency[min_index][v];
            }
        }
    }
    
    return (dist[end] == INF) ? -1 : dist[end];
}

int path_exists_dfs(Graph *g, int start, int end) {
    if (start < 0 || start >= g->node_count || end < 0 || end >= g->node_count) {
        fprintf(stderr, "Invalid start or end node\n");
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
                stack[++top] = i;
                if (top >= MAX_NODES - 1) {
                    fprintf(stderr, "Stack overflow\n");
                    return 0;
                }
            }
        }
    }
    
    return 0;
}

int execute_graph_operation(GraphOperation op, Graph *g, int start, int end) {
    if (op == NULL) {
        fprintf(stderr, "Null function pointer\n");
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
    
    printf("Graph operations:\n");
    
    GraphOperation operations[] = {shortest_path_dijkstra, path_exists_dfs};
    char* operation_names[] = {"Shortest path", "Path exists"};
    
    for (int i = 0; i < 2; i++) {
        for (int start = 0; start < 3; start++) {
            for (int end = 3; end < 6; end++) {
                int result = execute_graph_operation(operations