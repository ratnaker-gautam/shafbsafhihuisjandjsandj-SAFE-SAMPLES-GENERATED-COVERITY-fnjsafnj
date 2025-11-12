//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_NODES 100
#define MAX_EDGES 1000

struct Graph {
    int nodes;
    int edges;
    int adjacency[MAX_NODES][MAX_NODES];
    int visited[MAX_NODES];
    int distance[MAX_NODES];
};

void initialize_graph(struct Graph *g, int nodes) {
    if (g == NULL || nodes <= 0 || nodes > MAX_NODES) {
        return;
    }
    g->nodes = nodes;
    g->edges = 0;
    for (int i = 0; i < nodes; i++) {
        g->visited[i] = 0;
        g->distance[i] = INT_MAX;
        for (int j = 0; j < nodes; j++) {
            g->adjacency[i][j] = 0;
        }
    }
}

int add_edge(struct Graph *g, int u, int v, int weight) {
    if (g == NULL || u < 0 || u >= g->nodes || v < 0 || v >= g->nodes || weight < 0) {
        return 0;
    }
    if (g->edges >= MAX_EDGES) {
        return 0;
    }
    g->adjacency[u][v] = weight;
    g->adjacency[v][u] = weight;
    g->edges++;
    return 1;
}

void dfs(struct Graph *g, int node) {
    if (g == NULL || node < 0 || node >= g->nodes || g->visited[node]) {
        return;
    }
    g->visited[node] = 1;
    for (int i = 0; i < g->nodes; i++) {
        if (g->adjacency[node][i] != 0 && !g->visited[i]) {
            dfs(g, i);
        }
    }
}

void bfs(struct Graph *g, int start) {
    if (g == NULL || start < 0 || start >= g->nodes) {
        return;
    }
    for (int i = 0; i < g->nodes; i++) {
        g->visited[i] = 0;
        g->distance[i] = INT_MAX;
    }
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    g->visited[start] = 1;
    g->distance[start] = 0;
    queue[rear++] = start;
    while (front < rear) {
        int current = queue[front++];
        for (int i = 0; i < g->nodes; i++) {
            if (g->adjacency[current][i] != 0 && !g->visited[i]) {
                g->visited[i] = 1;
                g->distance[i] = g->distance[current] + 1;
                if (rear < MAX_NODES) {
                    queue[rear++] = i;
                }
            }
        }
    }
}

int dijkstra(struct Graph *g, int start, int end) {
    if (g == NULL || start < 0 || start >= g->nodes || end < 0 || end >= g->nodes) {
        return -1;
    }
    for (int i = 0; i < g->nodes; i++) {
        g->visited[i] = 0;
        g->distance[i] = INT_MAX;
    }
    g->distance[start] = 0;
    for (int count = 0; count < g->nodes; count++) {
        int min_dist = INT_MAX;
        int min_node = -1;
        for (int i = 0; i < g->nodes; i++) {
            if (!g->visited[i] && g->distance[i] < min_dist) {
                min_dist = g->distance[i];
                min_node = i;
            }
        }
        if (min_node == -1) {
            break;
        }
        g->visited[min_node] = 1;
        for (int i = 0; i < g->nodes; i++) {
            if (!g->visited[i] && g->adjacency[min_node][i] != 0) {
                long new_dist = (long)g->distance[min_node] + g->adjacency[min_node][i];
                if (new_dist < g->distance[i] && new_dist <= INT_MAX) {
                    g->distance[i] = (int)new_dist;
                }
            }
        }
    }
    return g->distance[end] == INT_MAX ? -1 : g->distance[end];
}

void print_graph(struct Graph *g) {
    if (g == NULL) {
        return;
    }
    printf("Graph with %d nodes and %d edges:\n", g->nodes, g->edges);
    for (int i = 0; i < g->nodes; i++) {
        printf("Node %d: ", i);
        for (int j = 0; j < g->nodes; j++) {
            if (g->adjacency[i][j] != 0) {
                printf("(%d, %d) ", j, g->adjacency[i][j]);
            }
        }
        printf("\n");
    }
}

int main() {
    struct Graph g;
    int nodes, edges;
    printf("Enter number of nodes (1-100): ");
    if (scanf("%d", &nodes) != 1 || nodes < 1 || nodes > MAX_NODES) {
        printf("Invalid input\n");
        return