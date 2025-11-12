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
    if (node_count <= 0 || node_count > MAX_NODES) {
        node_count = MAX_NODES;
    }
    g->node_count = node_count;
    for (int i = 0; i < node_count; i++) {
        for (int j = 0; j < node_count; j++) {
            g->adjacency[i][j] = (i == j) ? 0 : INF;
        }
    }
}

int add_edge(Graph *g, int from, int to, int weight) {
    if (g == NULL || from < 0 || from >= g->node_count || to < 0 || to >= g->node_count) {
        return 0;
    }
    if (weight < 0 || weight > 10000) {
        return 0;
    }
    g->adjacency[from][to] = weight;
    return 1;
}

int dijkstra_shortest_path(Graph *g, int start, int end) {
    if (g == NULL || start < 0 || start >= g->node_count || end < 0 || end >= g->node_count) {
        return INF;
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
        
        if (min_index == -1 || min_index == end) {
            break;
        }
        
        visited[min_index] = 1;
        
        for (int v = 0; v < g->node_count; v++) {
            if (!visited[v] && g->adjacency[min_index][v] != INF && dist[min_index] != INF) {
                long long new_dist = (long long)dist[min_index] + g->adjacency[min_index][v];
                if (new_dist > 0 && new_dist < INT_MAX && new_dist < dist[v]) {
                    dist[v] = (int)new_dist;
                }
            }
        }
    }
    
    return dist[end];
}

int has_path_exists(Graph *g, int start, int end) {
    if (g == NULL || start < 0 || start >= g->node_count || end < 0 || end >= g->node_count) {
        return 0;
    }
    
    int visited[MAX_NODES] = {0};
    int stack[MAX_NODES];
    int top = -1;
    
    if (top + 1 < MAX_NODES) {
        stack[++top] = start;
        visited[start] = 1;
    }
    
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
    Graph graph;
    initialize_graph(&graph, 6);
    
    add_edge(&graph, 0, 1, 4);
    add_edge(&graph, 0, 2, 2);
    add_edge(&graph, 1, 2, 1);
    add_edge(&graph, 1, 3, 5);
    add_edge(&graph, 2, 3, 8);
    add_edge(&graph, 2, 4, 10);
    add_edge(&graph, 3, 4, 2);
    add_edge(&graph, 3, 5, 6);
    add_edge(&graph, 4, 5, 3);
    
    GraphOperation operations[] = {dijkstra_shortest_path, has_path_exists};
    char* operation_names[] = {"Shortest Path Distance", "Path Exists"};
    
    printf("Graph Analysis Results:\n");
    printf("Nodes: 0-5, Edges configured\n\n");
    
    for (int i = 0; i < 2; i++) {
        printf("%s:\n", operation_names[i]);
        for (int start = 0; start < 3; start++) {
            for (int end = start + 1; end < 6