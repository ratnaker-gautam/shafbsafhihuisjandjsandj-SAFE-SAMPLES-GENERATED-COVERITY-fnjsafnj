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

void validate_graph(Graph* g) {
    if (g == NULL) {
        fprintf(stderr, "Graph is NULL\n");
        exit(1);
    }
    if (g->node_count <= 0 || g->node_count > MAX_NODES) {
        fprintf(stderr, "Invalid node count: %d\n", g->node_count);
        exit(1);
    }
    if (g->edge_count < 0 || g->edge_count > MAX_EDGES) {
        fprintf(stderr, "Invalid edge count: %d\n", g->edge_count);
        exit(1);
    }
    for (int i = 0; i < g->edge_count; i++) {
        if (g->edges[i].from < 0 || g->edges[i].from >= g->node_count ||
            g->edges[i].to < 0 || g->edges[i].to >= g->node_count) {
            fprintf(stderr, "Invalid edge endpoints\n");
            exit(1);
        }
    }
}

void bfs_traversal(Graph* g) {
    validate_graph(g);
    printf("BFS Traversal: ");
    
    int visited[MAX_NODES] = {0};
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    
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

void dfs_util(Graph* g, int node, int* visited) {
    printf("%d ", node);
    visited[node] = 1;
    
    for (int i = 0; i < g->edge_count; i++) {
        if (g->edges[i].from == node && !visited[g->edges[i].to]) {
            dfs_util(g, g->edges[i].to, visited);
        }
    }
}

void dfs_traversal(Graph* g) {
    validate_graph(g);
    printf("DFS Traversal: ");
    
    int visited[MAX_NODES] = {0};
    dfs_util(g, 0, visited);
    printf("\n");
}

void topological_sort_util(Graph* g, int node, int* visited, int* stack, int* stack_index) {
    visited[node] = 1;
    
    for (int i = 0; i < g->edge_count; i++) {
        if (g->edges[i].from == node && !visited[g->edges[i].to]) {
            topological_sort_util(g, g->edges[i].to, visited, stack, stack_index);
        }
    }
    
    if (*stack_index >= MAX_NODES) {
        fprintf(stderr, "Stack overflow\n");
        return;
    }
    stack[(*stack_index)++] = node;
}

void topological_sort(Graph* g) {
    validate_graph(g);
    printf("Topological Sort: ");
    
    int visited[MAX_NODES] = {0};
    int stack[MAX_NODES];
    int stack_index = 0;
    
    for (int i = 0; i < g->node_count; i++) {
        if (!visited[i]) {
            topological_sort_util(g, i, visited, stack, &stack_index);
        }
    }
    
    for (int i = stack_index - 1; i >= 0; i--) {
        printf("%d ", stack[i]);
    }
    printf("\n");
}

void run_algorithm(GraphAlgorithm algorithm, Graph* g) {
    if (algorithm == NULL) {
        fprintf(stderr, "Algorithm function pointer is NULL\n");
        return;
    }
    algorithm(g);
}

int main() {
    Graph g = {0};
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &g.node_count) != 1) {
        fprintf(stderr, "Invalid input for node count\n");
        return 1;
    }
    
    if (g.node_count <= 0 || g.node_count > MAX_NODES) {
        fprintf(stderr, "Node count must be between 1 and %d\n", MAX_NODES);
        return 1;
    }
    
    printf("Enter number of edges (0-%d): ", MAX_EDGES);
    if (scanf("%d", &g.edge_count) != 1) {
        fprintf(stderr, "Invalid input for edge count\n");
        return 1;
    }
    
    if (g.edge_count < 0 || g.edge_count > MAX_EDGES) {
        fprintf(stderr, "Edge count must be between