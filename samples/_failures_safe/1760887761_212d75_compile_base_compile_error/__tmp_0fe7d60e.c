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

void initialize_graph(Graph *g, int node_count) {
    if (node_count <= 0 || node_count > MAX_NODES) {
        fprintf(stderr, "Invalid node count\n");
        exit(EXIT_FAILURE);
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
        fprintf(stderr, "Invalid edge parameters\n");
        return;
    }
    g->adjacency[from][to] = 1;
    g->adjacency[to][from] = 1;
}

void dfs_util(Graph *g, int node, int visited[]) {
    if (node < 0 || node >= g->node_count) return;
    visited[node] = 1;
    printf("%d ", node);
    for (int i = 0; i < g->node_count; i++) {
        if (g->adjacency[node][i] && !visited[i]) {
            dfs_util(g, i, visited);
        }
    }
}

void depth_first_search(Graph *g, int start, int unused) {
    if (start < 0 || start >= g->node_count) {
        fprintf(stderr, "Invalid start node\n");
        return;
    }
    int visited[MAX_NODES] = {0};
    printf("DFS traversal: ");
    dfs_util(g, start, visited);
    printf("\n");
}

void breadth_first_search(Graph *g, int start, int unused) {
    if (start < 0 || start >= g->node_count) {
        fprintf(stderr, "Invalid start node\n");
        return;
    }
    int visited[MAX_NODES] = {0};
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    visited[start] = 1;
    queue[rear++] = start;
    printf("BFS traversal: ");
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
    printf("\n");
}

void count_components(Graph *g, int unused1, int unused2) {
    int visited[MAX_NODES] = {0};
    int component_count = 0;
    for (int i = 0; i < g->node_count; i++) {
        if (!visited[i]) {
            component_count++;
            int stack[MAX_NODES];
            int top = 0;
            stack[top++] = i;
            visited[i] = 1;
            while (top > 0) {
                int current = stack[--top];
                for (int j = 0; j < g->node_count; j++) {
                    if (g->adjacency[current][j] && !visited[j]) {
                        visited[j] = 1;
                        stack[top++] = j;
                        if (top >= MAX_NODES) {
                            fprintf(stderr, "Stack overflow\n");
                            return;
                        }
                    }
                }
            }
        }
    }
    printf("Number of connected components: %d\n", component_count);
}

void execute_operation(Graph *g, GraphOperation op, int param1, int param2) {
    if (g == NULL || op == NULL) {
        fprintf(stderr, "Invalid operation parameters\n");
        return;
    }
    op(g, param1, param2);
}

int main() {
    Graph graph;
    int node_count;
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &node_count) != 1) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    
    initialize_graph(&graph, node_count);
    
    int edge_count;
    printf("Enter number of edges: ");
    if (scanf("%d", &edge_count) != 1) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    
    if (edge_count < 0 || edge_count > node_count * (node_count - 1) / 2) {
        fprintf(stderr, "Invalid edge count\n");
        return EXIT_FAILURE;
    }
    
    printf("Enter edges (from to):\n");
    for (int i = 0; i < edge_count; i++) {
        int from, to;
        if (scanf("%d %d", &from, &to) != 2) {
            fprintf(stderr, "Invalid edge input\n");
            return EXIT_FAILURE;
        }