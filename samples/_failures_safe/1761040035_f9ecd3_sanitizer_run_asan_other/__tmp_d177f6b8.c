//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: function_pointers ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_NODES 100

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
            g->adjacency[i][j] = 0;
        }
    }
}

void add_edge(Graph *g, int from, int to, int weight) {
    if (from < 0 || from >= g->node_count || to < 0 || to >= g->node_count) {
        fprintf(stderr, "Invalid edge nodes\n");
        return;
    }
    if (weight < 0 || weight > 1000) {
        fprintf(stderr, "Invalid edge weight\n");
        return;
    }
    g->adjacency[from][to] = weight;
    g->adjacency[to][from] = weight;
}

void dfs_traversal(Graph *g, int start, int visited[]) {
    if (start < 0 || start >= g->node_count) return;
    visited[start] = 1;
    printf("%d ", start);
    for (int i = 0; i < g->node_count; i++) {
        if (g->adjacency[start][i] != 0 && !visited[i]) {
            dfs_traversal(g, i, visited);
        }
    }
}

void bfs_traversal(Graph *g, int start, int visited[]) {
    if (start < 0 || start >= g->node_count) return;
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    visited[start] = 1;
    queue[rear++] = start;
    while (front < rear) {
        int current = queue[front++];
        printf("%d ", current);
        for (int i = 0; i < g->node_count; i++) {
            if (g->adjacency[current][i] != 0 && !visited[i]) {
                visited[i] = 1;
                if (rear < MAX_NODES) {
                    queue[rear++] = i;
                }
            }
        }
    }
}

void perform_operation(Graph *g, GraphOperation op, int start, int target) {
    if (start < 0 || start >= g->node_count) {
        fprintf(stderr, "Invalid start node\n");
        return;
    }
    int visited[MAX_NODES] = {0};
    printf("Operation result: ");
    op(g, start, visited);
    printf("\n");
}

void dfs_wrapper(Graph *g, int start, int visited[]) {
    dfs_traversal(g, start, visited);
}

void bfs_wrapper(Graph *g, int start, int visited[]) {
    bfs_traversal(g, start, visited);
}

int main() {
    Graph g;
    initialize_graph(&g, 6);
    
    add_edge(&g, 0, 1, 5);
    add_edge(&g, 0, 2, 3);
    add_edge(&g, 1, 3, 2);
    add_edge(&g, 2, 3, 1);
    add_edge(&g, 3, 4, 4);
    add_edge(&g, 4, 5, 6);
    
    GraphOperation operations[] = {dfs_wrapper, bfs_wrapper};
    char* names[] = {"DFS", "BFS"};
    
    for (int i = 0; i < 2; i++) {
        printf("%s traversal from node 0: ", names[i]);
        perform_operation(&g, operations[i], 0, -1);
    }
    
    printf("Testing edge cases:\n");
    perform_operation(&g, dfs_wrapper, -1, -1);
    perform_operation(&g, bfs_wrapper, 10, -1);
    
    Graph small_graph;
    initialize_graph(&small_graph, 1);
    printf("Single node graph DFS: ");
    perform_operation(&small_graph, dfs_wrapper, 0, -1);
    
    return 0;
}