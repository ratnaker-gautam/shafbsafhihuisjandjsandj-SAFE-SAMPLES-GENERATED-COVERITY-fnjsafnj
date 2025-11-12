//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_NODES 100
#define INF INT_MAX

struct Graph {
    int nodes;
    int adj[MAX_NODES][MAX_NODES];
};

void init_graph(struct Graph *g, int nodes) {
    if (nodes <= 0 || nodes > MAX_NODES) {
        fprintf(stderr, "Invalid number of nodes\n");
        exit(1);
    }
    g->nodes = nodes;
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            g->adj[i][j] = (i == j) ? 0 : INF;
        }
    }
}

void add_edge(struct Graph *g, int u, int v, int weight) {
    if (u < 0 || u >= g->nodes || v < 0 || v >= g->nodes) {
        fprintf(stderr, "Invalid node index\n");
        return;
    }
    if (weight < 0) {
        fprintf(stderr, "Negative weights not allowed\n");
        return;
    }
    g->adj[u][v] = weight;
    g->adj[v][u] = weight;
}

void print_graph(const struct Graph *g) {
    printf("Graph adjacency matrix:\n");
    for (int i = 0; i < g->nodes; i++) {
        for (int j = 0; j < g->nodes; j++) {
            if (g->adj[i][j] == INF) {
                printf("INF ");
            } else {
                printf("%3d ", g->adj[i][j]);
            }
        }
        printf("\n");
    }
}

void dfs_util(const struct Graph *g, int node, int visited[], void (*visit)(int)) {
    if (node < 0 || node >= g->nodes) return;
    visited[node] = 1;
    visit(node);
    for (int i = 0; i < g->nodes; i++) {
        if (g->adj[node][i] != INF && g->adj[node][i] != 0 && !visited[i]) {
            dfs_util(g, i, visited, visit);
        }
    }
}

void dfs(const struct Graph *g, int start, void (*visit)(int)) {
    if (start < 0 || start >= g->nodes) {
        fprintf(stderr, "Invalid start node\n");
        return;
    }
    int visited[MAX_NODES] = {0};
    dfs_util(g, start, visited, visit);
}

void bfs(const struct Graph *g, int start, void (*visit)(int)) {
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
            if (g->adj[current][i] != INF && g->adj[current][i] != 0 && !visited[i]) {
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

void dijkstra(const struct Graph *g, int start, void (*visit)(int, int)) {
    if (start < 0 || start >= g->nodes) {
        fprintf(stderr, "Invalid start node\n");
        return;
    }
    int dist[MAX_NODES];
    int visited[MAX_NODES] = {0};
    for (int i = 0; i < g->nodes; i++) {
        dist[i] = INF;
    }
    dist[start] = 0;
    for (int count = 0; count < g->nodes; count++) {
        int min_dist = INF;
        int min_node = -1;
        for (int i = 0; i < g->nodes; i++) {
            if (!visited[i] && dist[i] < min_dist) {
                min_dist = dist[i];
                min_node = i;
            }
        }
        if (min_node == -1) break;
        visited[min_node] = 1;
        visit(min_node, dist[min_node]);
        for (int i = 0; i < g->nodes; i++) {
            if (!visited[i] && g->adj[min_node][i] != INF && dist[min_node] != INF) {
                long new_dist = (long)dist[min_node] + g->adj[min_node][i];
                if (new_dist < dist[i] && new_dist <= INF) {
                    dist[i] = (int)new_dist;
                }
            }
        }
    }
}

void print_node(int node) {
    printf("Visited node: %d\n", node);
}

void print_node_dist(int node, int dist) {
    printf("Node %d distance: %d\n", node, dist);
}

int main() {
    struct Graph g;
    init_graph(&g, 6);
    add_edge(&g, 0, 1, 4);
    add_edge(&g, 0, 2, 2);