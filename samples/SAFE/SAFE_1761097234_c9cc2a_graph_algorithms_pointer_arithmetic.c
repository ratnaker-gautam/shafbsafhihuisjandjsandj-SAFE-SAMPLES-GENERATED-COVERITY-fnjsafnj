//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 100
#define MAX_EDGES 500

struct Graph {
    int num_nodes;
    int num_edges;
    int adj_matrix[MAX_NODES][MAX_NODES];
};

void initialize_graph(struct Graph *g, int nodes) {
    if (g == NULL || nodes <= 0 || nodes > MAX_NODES) {
        return;
    }
    g->num_nodes = nodes;
    g->num_edges = 0;
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            *(*(g->adj_matrix + i) + j) = 0;
        }
    }
}

int add_edge(struct Graph *g, int u, int v) {
    if (g == NULL || u < 0 || v < 0 || u >= g->num_nodes || v >= g->num_nodes || u == v) {
        return 0;
    }
    if (*(*(g->adj_matrix + u) + v) == 0) {
        *(*(g->adj_matrix + u) + v) = 1;
        *(*(g->adj_matrix + v) + u) = 1;
        g->num_edges++;
        return 1;
    }
    return 0;
}

void dfs_util(struct Graph *g, int node, int *visited) {
    if (g == NULL || visited == NULL || node < 0 || node >= g->num_nodes) {
        return;
    }
    *(visited + node) = 1;
    printf("%d ", node);
    for (int i = 0; i < g->num_nodes; i++) {
        if (*(*(g->adj_matrix + node) + i) == 1 && *(visited + i) == 0) {
            dfs_util(g, i, visited);
        }
    }
}

void dfs(struct Graph *g, int start) {
    if (g == NULL || start < 0 || start >= g->num_nodes) {
        return;
    }
    int visited[MAX_NODES];
    for (int i = 0; i < g->num_nodes; i++) {
        *(visited + i) = 0;
    }
    printf("DFS traversal: ");
    dfs_util(g, start, visited);
    printf("\n");
}

void bfs(struct Graph *g, int start) {
    if (g == NULL || start < 0 || start >= g->num_nodes) {
        return;
    }
    int visited[MAX_NODES];
    for (int i = 0; i < g->num_nodes; i++) {
        *(visited + i) = 0;
    }
    int queue[MAX_NODES];
    int front = 0;
    int rear = 0;
    *(visited + start) = 1;
    *(queue + rear) = start;
    rear++;
    printf("BFS traversal: ");
    while (front < rear) {
        int current = *(queue + front);
        front++;
        printf("%d ", current);
        for (int i = 0; i < g->num_nodes; i++) {
            if (*(*(g->adj_matrix + current) + i) == 1 && *(visited + i) == 0) {
                *(visited + i) = 1;
                *(queue + rear) = i;
                rear++;
                if (rear >= MAX_NODES) {
                    return;
                }
            }
        }
    }
    printf("\n");
}

int main() {
    struct Graph graph;
    int num_nodes;
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &num_nodes) != 1 || num_nodes < 1 || num_nodes > MAX_NODES) {
        printf("Invalid number of nodes\n");
        return 1;
    }
    initialize_graph(&graph, num_nodes);
    int num_edges;
    printf("Enter number of edges: ");
    if (scanf("%d", &num_edges) != 1 || num_edges < 0 || num_edges > MAX_EDGES) {
        printf("Invalid number of edges\n");
        return 1;
    }
    printf("Enter edges (u v) for %d edges:\n", num_edges);
    for (int i = 0; i < num_edges; i++) {
        int u, v;
        if (scanf("%d %d", &u, &v) != 2) {
            printf("Invalid edge input\n");
            return 1;
        }
        if (!add_edge(&graph, u, v)) {
            printf("Invalid edge: %d %d\n", u, v);
            return 1;
        }
    }
    int start_node;
    printf("Enter starting node for traversals: ");
    if (scanf("%d", &start_node) != 1 || start_node < 0 || start_node >= num_nodes) {
        printf("Invalid starting node\n");
        return 1;
    }
    dfs(&graph, start_node);
    bfs(&graph, start_node);
    return 0;
}