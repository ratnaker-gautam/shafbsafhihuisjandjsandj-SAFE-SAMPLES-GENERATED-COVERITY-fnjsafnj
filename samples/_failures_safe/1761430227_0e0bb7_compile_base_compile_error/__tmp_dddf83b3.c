//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_NODES 100
#define INF INT_MAX

struct Graph {
    int nodes;
    int matrix[MAX_NODES][MAX_NODES];
};

struct Graph* create_graph(int n) {
    if (n <= 0 || n > MAX_NODES) return NULL;
    struct Graph* g = malloc(sizeof(struct Graph));
    if (!g) return NULL;
    g->nodes = n;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            g->matrix[i][j] = (i == j) ? 0 : INF;
        }
    }
    return g;
}

void add_edge(struct Graph* g, int u, int v, int w) {
    if (!g || u < 0 || u >= g->nodes || v < 0 || v >= g->nodes || w < 0) return;
    g->matrix[u][v] = w;
    g->matrix[v][u] = w;
}

void print_graph(const struct Graph* g) {
    if (!g) return;
    printf("Graph adjacency matrix:\n");
    for (int i = 0; i < g->nodes; i++) {
        for (int j = 0; j < g->nodes; j++) {
            if (g->matrix[i][j] == INF) printf("INF ");
            else printf("%3d ", g->matrix[i][j]);
        }
        printf("\n");
    }
}

void dfs_util(const struct Graph* g, int v, int visited[], void (*visit)(int)) {
    if (!g || !visited || !visit || v < 0 || v >= g->nodes) return;
    visited[v] = 1;
    visit(v);
    for (int i = 0; i < g->nodes; i++) {
        if (g->matrix[v][i] != INF && g->matrix[v][i] != 0 && !visited[i]) {
            dfs_util(g, i, visited, visit);
        }
    }
}

void dfs(const struct Graph* g, int start, void (*visit)(int)) {
    if (!g || !visit || start < 0 || start >= g->nodes) return;
    int visited[MAX_NODES] = {0};
    dfs_util(g, start, visited, visit);
}

void bfs(const struct Graph* g, int start, void (*visit)(int)) {
    if (!g || !visit || start < 0 || start >= g->nodes) return;
    int visited[MAX_NODES] = {0};
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    visited[start] = 1;
    queue[rear++] = start;
    while (front < rear) {
        int current = queue[front++];
        visit(current);
        for (int i = 0; i < g->nodes; i++) {
            if (g->matrix[current][i] != INF && g->matrix[current][i] != 0 && !visited[i]) {
                visited[i] = 1;
                queue[rear++] = i;
            }
        }
    }
}

void dijkstra(const struct Graph* g, int src, void (*visit)(int, int)) {
    if (!g || !visit || src < 0 || src >= g->nodes) return;
    int dist[MAX_NODES];
    int visited[MAX_NODES] = {0};
    for (int i = 0; i < g->nodes; i++) dist[i] = INF;
    dist[src] = 0;
    for (int count = 0; count < g->nodes - 1; count++) {
        int min_dist = INF, min_index = -1;
        for (int v = 0; v < g->nodes; v++) {
            if (!visited[v] && dist[v] <= min_dist) {
                min_dist = dist[v];
                min_index = v;
            }
        }
        if (min_index == -1) break;
        visited[min_index] = 1;
        for (int v = 0; v < g->nodes; v++) {
            if (!visited[v] && g->matrix[min_index][v] != INF && dist[min_index] != INF) {
                int new_dist = dist[min_index] + g->matrix[min_index][v];
                if (new_dist > 0 && new_dist < dist[v]) {
                    dist[v] = new_dist;
                }
            }
        }
    }
    for (int i = 0; i < g->nodes; i++) {
        visit(i, dist[i]);
    }
}

void print_node(int node) {
    printf("%d ", node);
}

void print_distance(int node, int dist) {
    if (dist == INF) printf("Node %d: unreachable\n", node);
    else printf("Node %d: distance %d\n", node, dist);
}

int main() {
    struct Graph* g = create_graph(6);
    if (!g) {
        printf("Graph creation failed\n");
        return 1;
    }
    add_edge(g, 0, 1, 4);
    add_edge(g, 0, 2, 2);
    add_edge(g, 1, 2, 1);
    add_edge(g, 1, 3, 5);
    add_edge(g, 2, 3, 8