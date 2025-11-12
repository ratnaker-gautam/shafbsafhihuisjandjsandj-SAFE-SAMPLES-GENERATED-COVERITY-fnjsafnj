//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_NODES 100

struct Graph {
    int nodes;
    int adj_matrix[MAX_NODES][MAX_NODES];
};

void init_graph(struct Graph *g, int nodes) {
    if (nodes <= 0 || nodes > MAX_NODES) {
        fprintf(stderr, "Invalid number of nodes\n");
        exit(1);
    }
    g->nodes = nodes;
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            g->adj_matrix[i][j] = 0;
        }
    }
}

void add_edge(struct Graph *g, int u, int v, int weight) {
    if (u < 0 || u >= g->nodes || v < 0 || v >= g->nodes) {
        fprintf(stderr, "Invalid node index\n");
        return;
    }
    if (weight < 0) {
        fprintf(stderr, "Weight must be non-negative\n");
        return;
    }
    g->adj_matrix[u][v] = weight;
    g->adj_matrix[v][u] = weight;
}

void dfs_util(struct Graph *g, int node, int visited[], void (*visit)(int)) {
    if (node < 0 || node >= g->nodes) return;
    visited[node] = 1;
    visit(node);
    for (int i = 0; i < g->nodes; i++) {
        if (g->adj_matrix[node][i] != 0 && !visited[i]) {
            dfs_util(g, i, visited, visit);
        }
    }
}

void dfs(struct Graph *g, int start, void (*visit)(int)) {
    if (start < 0 || start >= g->nodes) {
        fprintf(stderr, "Invalid start node\n");
        return;
    }
    int visited[MAX_NODES] = {0};
    dfs_util(g, start, visited, visit);
}

void bfs(struct Graph *g, int start, void (*visit)(int)) {
    if (start < 0 || start >= g->nodes) {
        fprintf(stderr, "Invalid start node\n");
        return;
    }
    int visited[MAX_NODES] = {0};
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    visited[start] = 1;
    queue[rear++] = start;
    while (front < rear) {
        int current = queue[front++];
        visit(current);
        for (int i = 0; i < g->nodes; i++) {
            if (g->adj_matrix[current][i] != 0 && !visited[i]) {
                visited[i] = 1;
                if (rear >= MAX_NODES) {
                    fprintf(stderr, "Queue overflow\n");
                    return;
                }
                queue[rear++] = i;
            }
        }
    }
}

void print_node(int node) {
    printf("%d ", node);
}

void print_node_with_newline(int node) {
    printf("Node %d\n", node);
}

void dijkstra(struct Graph *g, int start, void (*visit)(int, int)) {
    if (start < 0 || start >= g->nodes) {
        fprintf(stderr, "Invalid start node\n");
        return;
    }
    int dist[MAX_NODES];
    int visited[MAX_NODES] = {0};
    for (int i = 0; i < g->nodes; i++) {
        dist[i] = INT_MAX;
    }
    dist[start] = 0;
    for (int count = 0; count < g->nodes - 1; count++) {
        int min_dist = INT_MAX;
        int min_node = -1;
        for (int i = 0; i < g->nodes; i++) {
            if (!visited[i] && dist[i] < min_dist) {
                min_dist = dist[i];
                min_node = i;
            }
        }
        if (min_node == -1) break;
        visited[min_node] = 1;
        for (int i = 0; i < g->nodes; i++) {
            if (!visited[i] && g->adj_matrix[min_node][i] != 0) {
                long new_dist = (long)dist[min_node] + g->adj_matrix[min_node][i];
                if (new_dist < dist[i] && new_dist <= INT_MAX) {
                    dist[i] = (int)new_dist;
                }
            }
        }
    }
    for (int i = 0; i < g->nodes; i++) {
        visit(i, dist[i]);
    }
}

void print_distance(int node, int distance) {
    if (distance == INT_MAX) {
        printf("Node %d: unreachable\n", node);
    } else {
        printf("Node %d: distance %d\n", node, distance);
    }
}

void process_graph(struct Graph *g, void (*traversal)(struct Graph*, int, void (*)(int))) {
    if (g == NULL || traversal == NULL) {
        fprintf(stderr, "Invalid function pointer\n");
        return;
    }
    printf("Enter starting node (0-%d): ", g->nodes - 1);
    int start;
    if (scanf("%d", &start) != 1) {
        fprintf(stderr, "Invalid input\n");
        while (getchar() != '\n');
        return;
    }
    traversal(g, start