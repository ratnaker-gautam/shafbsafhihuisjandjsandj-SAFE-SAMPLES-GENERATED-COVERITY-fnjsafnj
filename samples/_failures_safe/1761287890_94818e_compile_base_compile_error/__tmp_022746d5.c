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

void bfs_traversal(Graph *g) {
    validate_graph(g);
    printf("BFS Traversal:\n");
    
    int visited[MAX_NODES] = {0};
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    
    if (rear >= MAX_NODES) {
        fprintf(stderr, "Queue overflow\n");
        return;
    }
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

void dfs_traversal(Graph *g) {
    validate_graph(g);
    printf("DFS Traversal:\n");
    
    int visited[MAX_NODES] = {0};
    int stack[MAX_NODES];
    int top = 0;
    
    if (top >= MAX_NODES) {
        fprintf(stderr, "Stack overflow\n");
        return;
    }
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

void topological_sort(Graph *g) {
    validate_graph(g);
    printf("Topological Sort:\n");
    
    int in_degree[MAX_NODES] = {0};
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    int result[MAX_NODES];
    int result_index = 0;
    
    for (int i = 0; i < g->edge_count; i++) {
        in_degree[g->edges[i].to]++;
    }
    
    for (int i = 0; i < g->node_count; i++) {
        if (in_degree[i] == 0) {
            if (rear >= MAX_NODES) {
                fprintf(stderr, "Queue overflow\n");
                return;
            }
            queue[rear++] = i;
        }
    }
    
    while (front < rear) {
        int current = queue[front++];
        result[result_index++] = current;
        
        for (int i = 0; i < g->edge_count; i++) {
            if (g->edges[i].from == current) {
                in_degree[g->edges[i].to]--;
                if (in_degree[g->edges[i].to] == 0) {
                    if (rear >= MAX_NODES) {
                        fprintf(stderr, "Queue overflow\n");
                        return;
                    }
                    queue[rear++] = g->edges[i].to;
                }
            }
        }
    }
    
    if (result_index != g->node_count) {
        printf("Graph has cycles, cannot perform topological sort\n");
        return;
    }
    
    for (int i = 0; i < result_index; i++) {
        printf("%d ", result[i]);
    }
    printf("\n");
}

void run_algorithm(GraphAlgorithm algo, Graph *g) {
    if (algo == NULL) {
        fprintf(stderr, "Algorithm function pointer is NULL\n");
        return;
    }
    algo(g);
}

Graph create_sample_graph() {
    Graph g;