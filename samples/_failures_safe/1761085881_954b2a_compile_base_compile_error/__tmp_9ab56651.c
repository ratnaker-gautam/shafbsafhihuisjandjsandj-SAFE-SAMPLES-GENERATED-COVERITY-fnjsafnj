//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: function_pointers ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

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

typedef void (*GraphAlgorithm)(Graph*, int, int*);

void validate_graph(Graph *g) {
    if (g == NULL) {
        fprintf(stderr, "Graph is NULL\n");
        exit(EXIT_FAILURE);
    }
    if (g->node_count <= 0 || g->node_count > MAX_NODES) {
        fprintf(stderr, "Invalid node count\n");
        exit(EXIT_FAILURE);
    }
    if (g->edge_count < 0 || g->edge_count > MAX_EDGES) {
        fprintf(stderr, "Invalid edge count\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < g->edge_count; i++) {
        if (g->edges[i].from < 0 || g->edges[i].from >= g->node_count ||
            g->edges[i].to < 0 || g->edges[i].to >= g->node_count) {
            fprintf(stderr, "Invalid edge node indices\n");
            exit(EXIT_FAILURE);
        }
    }
}

void bfs_traversal(Graph *g, int start, int *result) {
    if (g == NULL || result == NULL) {
        fprintf(stderr, "Invalid parameters for BFS\n");
        return;
    }
    if (start < 0 || start >= g->node_count) {
        fprintf(stderr, "Invalid start node for BFS\n");
        return;
    }

    int visited[MAX_NODES] = {0};
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    int result_index = 0;

    visited[start] = 1;
    queue[rear++] = start;

    while (front < rear) {
        int current = queue[front++];
        result[result_index++] = current;

        for (int i = 0; i < g->edge_count; i++) {
            if (g->edges[i].from == current) {
                int neighbor = g->edges[i].to;
                if (!visited[neighbor]) {
                    visited[neighbor] = 1;
                    if (rear < MAX_NODES) {
                        queue[rear++] = neighbor;
                    } else {
                        fprintf(stderr, "Queue overflow in BFS\n");
                        return;
                    }
                }
            }
        }
    }

    for (int i = result_index; i < g->node_count; i++) {
        result[i] = -1;
    }
}

void dfs_util(Graph *g, int node, int *visited, int *result, int *index) {
    if (node < 0 || node >= g->node_count || visited == NULL || result == NULL || index == NULL) {
        return;
    }
    visited[node] = 1;
    result[(*index)++] = node;

    for (int i = 0; i < g->edge_count; i++) {
        if (g->edges[i].from == node) {
            int neighbor = g->edges[i].to;
            if (!visited[neighbor]) {
                dfs_util(g, neighbor, visited, result, index);
            }
        }
    }
}

void dfs_traversal(Graph *g, int start, int *result) {
    if (g == NULL || result == NULL) {
        fprintf(stderr, "Invalid parameters for DFS\n");
        return;
    }
    if (start < 0 || start >= g->node_count) {
        fprintf(stderr, "Invalid start node for DFS\n");
        return;
    }

    int visited[MAX_NODES] = {0};
    int index = 0;
    dfs_util(g, start, visited, result, &index);

    for (int i = index; i < g->node_count; i++) {
        result[i] = -1;
    }
}

void run_algorithm(GraphAlgorithm algo, Graph *g, int start, const char *name) {
    if (algo == NULL || g == NULL || name == NULL) {
        fprintf(stderr, "Invalid parameters for algorithm execution\n");
        return;
    }
    int result[MAX_NODES];
    algo(g, start, result);
    printf("%s traversal from node %d: ", name, start);
    for (int i = 0; i < g->node_count && result[i] != -1; i++) {
        printf("%d ", result[i]);
    }
    printf("\n");
}

Graph create_sample_graph() {
    Graph g;
    g.node_count = 6;
    g.edge_count = 7;
    Edge edges[] = {
        {0, 1, 1}, {0, 2, 1}, {1, 3, 1}, {2, 3, 1},
        {3, 4, 1}, {4, 5, 1}, {2, 5, 1}
    };
    for (int i = 0; i < g.edge_count; i++) {
        if (i < MAX_EDGES) {
            g.edges[i] = edges[i];
        }
    }
    return g;
}

int main()