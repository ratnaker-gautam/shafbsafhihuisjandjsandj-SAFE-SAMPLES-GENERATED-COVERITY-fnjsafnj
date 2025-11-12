//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: graph_algorithms
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

void initialize_graph(Graph *g, int node_count) {
    if (node_count <= 0 || node_count > MAX_NODES) {
        fprintf(stderr, "Invalid node count\n");
        exit(1);
    }
    g->node_count = node_count;
    for (int i = 0; i < node_count; i++) {
        for (int j = 0; j < node_count; j++) {
            g->adjacency[i][j] = 0;
        }
    }
}

void add_edge(Graph *g, int from, int to) {
    if (from < 0 || from >= g->node_count || to < 0 || to >= g->node_count) {
        fprintf(stderr, "Invalid edge nodes\n");
        return;
    }
    g->adjacency[from][to] = 1;
    g->adjacency[to][from] = 1;
}

void depth_first_traversal(Graph *g, int start, int visited[]) {
    if (start < 0 || start >= g->node_count) return;
    visited[start] = 1;
    printf("%d ", start);
    for (int i = 0; i < g->node_count; i++) {
        if (g->adjacency[start][i] && !visited[i]) {
            depth_first_traversal(g, i, visited);
        }
    }
}

void breadth_first_traversal(Graph *g, int start, int visited[]) {
    if (start < 0 || start >= g->node_count) return;
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    visited[start] = 1;
    queue[rear++] = start;
    while (front < rear) {
        int current = queue[front++];
        printf("%d ", current);
        for (int i = 0; i < g->node_count; i++) {
            if (g->adjacency[current][i] && !visited[i]) {
                visited[i] = 1;
                queue[rear++] = i;
                if (rear >= MAX_NODES) {
                    fprintf(stderr, "Queue overflow\n");
                    return;
                }
            }
        }
    }
}

void perform_operation(Graph *g, int start, GraphOperation op) {
    if (start < 0 || start >= g->node_count) {
        fprintf(stderr, "Invalid start node\n");
        return;
    }
    int visited[MAX_NODES] = {0};
    op(g, start, visited);
    printf("\n");
}

void dfs_wrapper(Graph *g, int start, int visited[]) {
    depth_first_traversal(g, start, visited);
}

void bfs_wrapper(Graph *g, int start, int visited[]) {
    breadth_first_traversal(g, start, visited);
}

int main() {
    Graph g;
    int node_count, edge_count;
    
    printf("Enter number of nodes (1-100): ");
    if (scanf("%d", &node_count) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    initialize_graph(&g, node_count);
    
    printf("Enter number of edges: ");
    if (scanf("%d", &edge_count) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    printf("Enter edges (from to):\n");
    for (int i = 0; i < edge_count; i++) {
        int from, to;
        if (scanf("%d %d", &from, &to) != 2) {
            fprintf(stderr, "Invalid edge input\n");
            return 1;
        }
        add_edge(&g, from, to);
    }
    
    int start_node;
    printf("Enter start node for traversal: ");
    if (scanf("%d", &start_node) != 1) {
        fprintf(stderr, "Invalid start node\n");
        return 1;
    }
    
    GraphOperation operations[] = {dfs_wrapper, bfs_wrapper};
    char* names[] = {"DFS", "BFS"};
    
    for (int i = 0; i < 2; i++) {
        printf("%s traversal: ", names[i]);
        perform_operation(&g, start_node, operations[i]);
    }
    
    return 0;
}