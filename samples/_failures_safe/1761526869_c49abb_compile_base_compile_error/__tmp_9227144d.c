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
            fprintf(stderr, "Invalid edge: %d -> %d\n", g->edges[i].from, g->edges[i].to);
            exit(1);
        }
    }
}

void bfs_traversal(Graph* g) {
    validate_graph(g);
    printf("BFS Traversal:\n");
    
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

void dfs_traversal(Graph* g) {
    validate_graph(g);
    printf("DFS Traversal:\n");
    
    int visited[MAX_NODES] = {0};
    int stack[MAX_NODES];
    int top = 0;
    
    stack[top++] = 0;
    visited[0] = 1;
    
    while (top > 0) {
        int current = stack[--top];
        printf("%d ", current);
        
        for (int i = 0; i < g->edge_count; i++) {
            if (g->edges[i].from == current && !visited[g->edges[i].to]) {
                if (top >= MAX_NODES) {
                    fprintf(stderr, "Stack overflow\n");
                    return;
                }
                stack[top++] = g->edges[i].to;
                visited[g->edges[i].to] = 1;
            }
        }
    }
    printf("\n");
}

void find_connected_components(Graph* g) {
    validate_graph(g);
    printf("Connected Components:\n");
    
    int visited[MAX_NODES] = {0};
    int component_count = 0;
    
    for (int start = 0; start < g->node_count; start++) {
        if (!visited[start]) {
            printf("Component %d: ", ++component_count);
            
            int stack[MAX_NODES];
            int top = 0;
            stack[top++] = start;
            visited[start] = 1;
            
            while (top > 0) {
                int current = stack[--top];
                printf("%d ", current);
                
                for (int i = 0; i < g->edge_count; i++) {
                    if (g->edges[i].from == current && !visited[g->edges[i].to]) {
                        if (top >= MAX_NODES) {
                            fprintf(stderr, "Stack overflow\n");
                            return;
                        }
                        stack[top++] = g->edges[i].to;
                        visited[g->edges[i].to] = 1;
                    }
                    if (g->edges[i].to == current && !visited[g->edges[i].from]) {
                        if (top >= MAX_NODES) {
                            fprintf(stderr, "Stack overflow\n");
                            return;
                        }
                        stack[top++] = g->edges[i].from;
                        visited[g->edges[i].from] = 1;
                    }
                }
            }
            printf("\n");
        }
    }
}

void run_algorithm(Graph* g, GraphAlgorithm algorithm) {
    if (algorithm == NULL) {
        fprintf(stderr, "Algorithm function pointer is NULL\n");
        return;
    }
    algorithm(g);
}

int main() {
    Graph graph;
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &graph.node_count) != 1) {
        fprintf(stderr, "Invalid input for node count\n");
        return 1;
    }
    
    printf("Enter number of edges (0-%d): ", MAX_