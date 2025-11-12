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
    if (g == NULL || node_count <= 0 || node_count > MAX_NODES) return;
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
    if (!validate_node(g, from) || !validate_node(g, to)) return;
    g->adjacency[from][to] = 1;
    g->adjacency[to][from] = 1;
}

void dfs_util(Graph *g, int node, int visited[]) {
    if (g == NULL || visited == NULL) return;
    if (!validate_node(g, node)) return;
    visited[node] = 1;
    printf("%d ", node);
    for (int i = 0; i < g->node_count; i++) {
        if (g->adjacency[node][i] && !visited[i]) {
            dfs_util(g, i, visited);
        }
    }
}

void depth_first_search(Graph *g, int start, int target) {
    if (g == NULL) return;
    int visited[MAX_NODES] = {0};
    if (!validate_node(g, start)) return;
    printf("DFS from %d: ", start);
    dfs_util(g, start, visited);
    printf("\n");
    if (validate_node(g, target) && visited[target]) {
        printf("Node %d is reachable from %d\n", target, start);
    } else if (validate_node(g, target)) {
        printf("Node %d is not reachable from %d\n", target, start);
    }
}

void breadth_first_search(Graph *g, int start, int target) {
    if (g == NULL) return;
    if (!validate_node(g, start)) return;
    int visited[MAX_NODES] = {0};
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    visited[start] = 1;
    queue[rear++] = start;
    printf("BFS from %d: ", start);
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
    printf("\n");
    if (validate_node(g, target) && visited[target]) {
        printf("Node %d is reachable from %d\n", target, start);
    } else if (validate_node(g, target)) {
        printf("Node %d is not reachable from %d\n", target, start);
    }
}

void execute_operation(Graph *g, GraphOperation op, int start, int target) {
    if (g == NULL || op == NULL) return;
    op(g, start, target);
}

int main() {
    Graph g;
    int node_count;
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &node_count) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    if (node_count <= 0 || node_count > MAX_NODES) {
        printf("Invalid node count\n");
        return 1;
    }
    initialize_graph(&g, node_count);
    int edge_count;
    printf("Enter number of edges: ");
    if (scanf("%d", &edge_count) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    if (edge_count < 0 || edge_count > node_count * (node_count - 1) / 2) {
        printf("Invalid edge count\n");
        return 1;
    }
    for (int i = 0; i < edge_count; i++) {
        int from, to;
        printf("Enter edge %d (from to): ", i + 1);
        if (scanf("%d %d", &from, &to) != 2) {
            printf("Invalid input\n");
            return 1;
        }
        if (from < 0 || from >= node_count || to < 0 || to >= node_count) {
            printf("Invalid node\n");
            return 1;
        }
        add_edge(&g, from, to);
    }
    int start_node, target_node;
    printf("Enter start node: ");
    if (scanf("%d", &start_node) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    printf("Enter target node: ");
    if (scanf("%d", &target_node) != 1) {
        printf("Invalid input\n");