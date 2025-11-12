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
    if (g == NULL || node_count <= 0 || node_count > MAX_NODES) {
        return;
    }
    g->node_count = node_count;
    for (int i = 0; i < node_count; i++) {
        for (int j = 0; j < node_count; j++) {
            g->adjacency[i][j] = 0;
        }
    }
}

int validate_node(Graph *g, int node) {
    return (g != NULL && node >= 0 && node < g->node_count);
}

void add_edge(Graph *g, int from, int to) {
    if (!validate_node(g, from) || !validate_node(g, to)) {
        return;
    }
    g->adjacency[from][to] = 1;
    g->adjacency[to][from] = 1;
}

void dfs_traversal(Graph *g, int start, int visited[]) {
    if (g == NULL || visited == NULL || !validate_node(g, start)) {
        return;
    }
    visited[start] = 1;
    printf("%d ", start);
    for (int i = 0; i < g->node_count; i++) {
        if (g->adjacency[start][i] && !visited[i]) {
            dfs_traversal(g, i, visited);
        }
    }
}

void bfs_traversal(Graph *g, int start, int visited[]) {
    if (g == NULL || visited == NULL || !validate_node(g, start)) {
        return;
    }
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
                if (rear < MAX_NODES) {
                    queue[rear++] = i;
                }
            }
        }
    }
}

void execute_operation(Graph *g, GraphOperation op, int start, int target) {
    if (g == NULL || op == NULL || !validate_node(g, start)) {
        return;
    }
    int visited[MAX_NODES] = {0};
    op(g, start, visited);
    printf("\n");
}

void print_graph(Graph *g, int unused1, int unused2) {
    if (g == NULL) {
        return;
    }
    printf("Graph adjacency matrix:\n");
    for (int i = 0; i < g->node_count; i++) {
        for (int j = 0; j < g->node_count; j++) {
            printf("%d ", g->adjacency[i][j]);
        }
        printf("\n");
    }
}

int main() {
    Graph g;
    int node_count;
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &node_count) != 1 || node_count <= 0 || node_count > MAX_NODES) {
        printf("Invalid node count\n");
        return 1;
    }
    initialize_graph(&g, node_count);
    int edge_count;
    printf("Enter number of edges: ");
    if (scanf("%d", &edge_count) != 1 || edge_count < 0) {
        printf("Invalid edge count\n");
        return 1;
    }
    for (int i = 0; i < edge_count; i++) {
        int from, to;
        printf("Enter edge %d (from to): ", i + 1);
        if (scanf("%d %d", &from, &to) != 2) {
            printf("Invalid edge\n");
            return 1;
        }
        if (!validate_node(&g, from) || !validate_node(&g, to)) {
            printf("Invalid node index\n");
            return 1;
        }
        add_edge(&g, from, to);
    }
    GraphOperation operations[] = {print_graph, dfs_traversal, bfs_traversal};
    int operation_count = sizeof(operations) / sizeof(operations[0]);
    int choice;
    printf("Choose operation (0=print, 1=DFS, 2=BFS): ");
    if (scanf("%d", &choice) != 1 || choice < 0 || choice >= operation_count) {
        printf("Invalid choice\n");
        return 1;
    }
    int start_node;
    printf("Enter start node: ");
    if (scanf("%d", &start_node) != 1 || !validate_node(&g, start_node)) {
        printf("Invalid start node\n");
        return 1;
    }
    execute_operation(&g, operations[choice], start_node, 0);
    return 0;
}