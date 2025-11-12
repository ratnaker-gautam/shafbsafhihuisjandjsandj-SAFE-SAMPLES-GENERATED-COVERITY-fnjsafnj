//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_NODES 100
#define MAX_EDGES 1000

struct Graph {
    int nodes;
    int edges[MAX_EDGES][3];
    int edge_count;
};

struct Graph* create_graph(int nodes) {
    if (nodes <= 0 || nodes > MAX_NODES) return NULL;
    struct Graph* g = malloc(sizeof(struct Graph));
    if (!g) return NULL;
    g->nodes = nodes;
    g->edge_count = 0;
    return g;
}

int add_edge(struct Graph* g, int u, int v, int weight) {
    if (!g || u < 0 || u >= g->nodes || v < 0 || v >= g->nodes || 
        g->edge_count >= MAX_EDGES || weight < 0) return 0;
    g->edges[g->edge_count][0] = u;
    g->edges[g->edge_count][1] = v;
    g->edges[g->edge_count][2] = weight;
    g->edge_count++;
    return 1;
}

void dfs_util(struct Graph* g, int node, int* visited, void (*visit)(int)) {
    if (!g || !visited || !visit || node < 0 || node >= g->nodes) return;
    if (visited[node]) return;
    visited[node] = 1;
    visit(node);
    for (int i = 0; i < g->edge_count; i++) {
        if (g->edges[i][0] == node && !visited[g->edges[i][1]]) {
            dfs_util(g, g->edges[i][1], visited, visit);
        }
    }
}

void dfs(struct Graph* g, int start, void (*visit)(int)) {
    if (!g || !visit || start < 0 || start >= g->nodes) return;
    int visited[MAX_NODES] = {0};
    dfs_util(g, start, visited, visit);
}

void bfs(struct Graph* g, int start, void (*visit)(int)) {
    if (!g || !visit || start < 0 || start >= g->nodes) return;
    int visited[MAX_NODES] = {0};
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    visited[start] = 1;
    queue[rear++] = start;
    while (front < rear) {
        int current = queue[front++];
        visit(current);
        for (int i = 0; i < g->edge_count; i++) {
            if (g->edges[i][0] == current && !visited[g->edges[i][1]]) {
                visited[g->edges[i][1]] = 1;
                if (rear < MAX_NODES) {
                    queue[rear++] = g->edges[i][1];
                }
            }
        }
    }
}

void dijkstra(struct Graph* g, int start, void (*result)(int, int)) {
    if (!g || !result || start < 0 || start >= g->nodes) return;
    int dist[MAX_NODES];
    int visited[MAX_NODES] = {0};
    for (int i = 0; i < g->nodes; i++) dist[i] = INT_MAX;
    dist[start] = 0;
    for (int count = 0; count < g->nodes; count++) {
        int min_dist = INT_MAX, min_index = -1;
        for (int v = 0; v < g->nodes; v++) {
            if (!visited[v] && dist[v] <= min_dist) {
                min_dist = dist[v];
                min_index = v;
            }
        }
        if (min_index == -1) break;
        visited[min_index] = 1;
        for (int i = 0; i < g->edge_count; i++) {
            if (g->edges[i][0] == min_index) {
                int v = g->edges[i][1];
                int weight = g->edges[i][2];
                if (!visited[v] && dist[min_index] != INT_MAX && 
                    dist[min_index] + weight < dist[v]) {
                    dist[v] = dist[min_index] + weight;
                }
            }
        }
    }
    for (int i = 0; i < g->nodes; i++) {
        result(i, dist[i]);
    }
}

void print_node(int node) {
    printf("Visited node: %d\n", node);
}

void print_distance(int node, int dist) {
    if (dist == INT_MAX) {
        printf("Node %d: unreachable\n", node);
    } else {
        printf("Node %d: distance %d\n", node, dist);
    }
}

void process_graph(struct Graph* g, void (*algorithm)(struct Graph*, int, void (*)(int))) {
    if (!g || !algorithm) return;
    printf("Running graph algorithm...\n");
    algorithm(g, 0, print_node);
}

int main() {
    struct Graph* g = create_graph(6);
    if (!g) {
        printf("Failed to create graph\n");
        return 1;
    }
    add_edge(g, 0, 1, 4);
    add_edge(g, 0, 2, 2);
    add_edge(g, 1, 2, 1);
    add_edge(g, 1, 3, 5);
    add_edge