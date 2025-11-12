//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 100
#define MAX_EDGES 1000

typedef struct {
    int from;
    int to;
    int weight;
} Edge;

typedef struct {
    int node_count;
    int edge_count;
    Edge edges[MAX_EDGES];
} Graph;

typedef void (*GraphAlgorithm)(Graph*);

void validate_graph(Graph *g) {
    if (g == NULL) {
        fprintf(stderr, "Graph is NULL\n");
        exit(1);
    }
    if (g->node_count <= 0 || g->node_count > MAX_NODES) {
        fprintf(stderr, "Invalid node count\n");
        exit(1);
    }
    if (g->edge_count < 0 || g->edge_count > MAX_EDGES) {
        fprintf(stderr, "Invalid edge count\n");
        exit(1);
    }
    for (int i = 0; i < g->edge_count; i++) {
        if (g->edges[i].from < 0 || g->edges[i].from >= g->node_count ||
            g->edges[i].to < 0 || g->edges[i].to >= g->node_count) {
            fprintf(stderr, "Invalid edge node indices\n");
            exit(1);
        }
    }
}

void print_graph(Graph *g) {
    validate_graph(g);
    printf("Graph with %d nodes and %d edges:\n", g->node_count, g->edge_count);
    for (int i = 0; i < g->edge_count; i++) {
        printf("  %d -> %d (weight: %d)\n", 
               g->edges[i].from, g->edges[i].to, g->edges[i].weight);
    }
}

void bfs_traversal(Graph *g) {
    validate_graph(g);
    if (g->node_count == 0) return;
    
    int visited[MAX_NODES] = {0};
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    
    printf("BFS traversal: ");
    queue[rear++] = 0;
    visited[0] = 1;
    
    while (front < rear) {
        int current = queue[front++];
        printf("%d ", current);
        
        for (int i = 0; i < g->edge_count; i++) {
            if (g->edges[i].from == current && !visited[g->edges[i].to]) {
                if (rear >= MAX_NODES) {
                    fprintf(stderr, "Queue overflow\n");
                    return;
                }
                queue[rear++] = g->edges[i].to;
                visited[g->edges[i].to] = 1;
            }
        }
    }
    printf("\n");
}

void dfs_visit(Graph *g, int node, int *visited) {
    printf("%d ", node);
    visited[node] = 1;
    
    for (int i = 0; i < g->edge_count; i++) {
        if (g->edges[i].from == node && !visited[g->edges[i].to]) {
            dfs_visit(g, g->edges[i].to, visited);
        }
    }
}

void dfs_traversal(Graph *g) {
    validate_graph(g);
    if (g->node_count == 0) return;
    
    int visited[MAX_NODES] = {0};
    printf("DFS traversal: ");
    dfs_visit(g, 0, visited);
    printf("\n");
}

void topological_sort_util(Graph *g, int node, int *visited, int *stack, int *stack_top) {
    visited[node] = 1;
    
    for (int i = 0; i < g->edge_count; i++) {
        if (g->edges[i].from == node && !visited[g->edges[i].to]) {
            topological_sort_util(g, g->edges[i].to, visited, stack, stack_top);
        }
    }
    
    if (*stack_top >= MAX_NODES - 1) {
        fprintf(stderr, "Stack overflow\n");
        return;
    }
    stack[++(*stack_top)] = node;
}

void topological_sort(Graph *g) {
    validate_graph(g);
    if (g->node_count == 0) return;
    
    int visited[MAX_NODES] = {0};
    int stack[MAX_NODES];
    int stack_top = -1;
    
    for (int i = 0; i < g->node_count; i++) {
        if (!visited[i]) {
            topological_sort_util(g, i, visited, stack, &stack_top);
        }
    }
    
    printf("Topological sort: ");
    while (stack_top >= 0) {
        printf("%d ", stack[stack_top--]);
    }
    printf("\n");
}

void run_algorithm(Graph *g, GraphAlgorithm algorithm) {
    if (algorithm == NULL) {
        fprintf(stderr, "Algorithm function is NULL\n");
        return;
    }
    algorithm(g);
}

Graph create_sample_graph() {
    Graph g;
    g.node_count = 6;
    g.edge_count = 7;
    
    Edge edges[] = {
        {0, 1, 1}, {0, 2, 1}, {1, 3, 1}, 
        {2, 3, 1}, {2, 4, 1}, {3, 5, 1}, {4