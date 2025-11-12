//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_NODES 100
#define INF INT_MAX

typedef struct {
    int adjacency[MAX_NODES][MAX_NODES];
    int node_count;
} Graph;

typedef int (*GraphOperation)(Graph*, int, int);

void initialize_graph(Graph *g, int node_count) {
    if (g == NULL || node_count <= 0 || node_count > MAX_NODES) {
        return;
    }
    g->node_count = node_count;
    for (int i = 0; i < node_count; i++) {
        for (int j = 0; j < node_count; j++) {
            g->adjacency[i][j] = (i == j) ? 0 : INF;
        }
    }
}

int add_edge(Graph *g, int from, int to, int weight) {
    if (g == NULL || from < 0 || to < 0 || from >= g->node_count || to >= g->node_count) {
        return 0;
    }
    if (weight < 0 || weight > 10000) {
        return 0;
    }
    g->adjacency[from][to] = weight;
    return 1;
}

int dijkstra_operation(Graph *g, int start, int end) {
    if (g == NULL || start < 0 || end < 0 || start >= g->node_count || end >= g->node_count) {
        return -1;
    }
    int dist[MAX_NODES];
    int visited[MAX_NODES];
    for (int i = 0; i < g->node_count; i++) {
        dist[i] = INF;
        visited[i] = 0;
    }
    dist[start] = 0;
    for (int count = 0; count < g->node_count - 1; count++) {
        int min_dist = INF;
        int min_index = -1;
        for (int v = 0; v < g->node_count; v++) {
            if (!visited[v] && dist[v] <= min_dist) {
                min_dist = dist[v];
                min_index = v;
            }
        }
        if (min_index == -1 || min_index == end) {
            break;
        }
        visited[min_index] = 1;
        for (int v = 0; v < g->node_count; v++) {
            if (!visited[v] && g->adjacency[min_index][v] != INF && dist[min_index] != INF) {
                long new_dist = (long)dist[min_index] + g->adjacency[min_index][v];
                if (new_dist < dist[v] && new_dist <= INT_MAX) {
                    dist[v] = (int)new_dist;
                }
            }
        }
    }
    return (dist[end] == INF) ? -1 : dist[end];
}

int bfs_operation(Graph *g, int start, int end) {
    if (g == NULL || start < 0 || end < 0 || start >= g->node_count || end >= g->node_count) {
        return -1;
    }
    if (start == end) {
        return 0;
    }
    int visited[MAX_NODES] = {0};
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    int distance[MAX_NODES];
    for (int i = 0; i < g->node_count; i++) {
        distance[i] = -1;
    }
    visited[start] = 1;
    distance[start] = 0;
    queue[rear++] = start;
    while (front < rear) {
        int current = queue[front++];
        for (int neighbor = 0; neighbor < g->node_count; neighbor++) {
            if (g->adjacency[current][neighbor] != INF && !visited[neighbor]) {
                visited[neighbor] = 1;
                distance[neighbor] = distance[current] + 1;
                if (neighbor == end) {
                    return distance[neighbor];
                }
                if (rear < MAX_NODES) {
                    queue[rear++] = neighbor;
                }
            }
        }
    }
    return -1;
}

int execute_graph_operation(GraphOperation op, Graph *g, int start, int end) {
    if (op == NULL || g == NULL) {
        return -1;
    }
    return op(g, start, end);
}

int main() {
    Graph g;
    int node_count;
    printf("Enter number of nodes (1-100): ");
    if (scanf("%d", &node_count) != 1 || node_count < 1 || node_count > MAX_NODES) {
        printf("Invalid node count\n");
        return 1;
    }
    initialize_graph(&g, node_count);
    int edge_count;
    printf("Enter number of edges: ");
    if (scanf("%d", &edge_count) != 1 || edge_count < 0 || edge_count > node_count * node_count) {
        printf("Invalid edge count\n");
        return 1;
    }
    for (int i = 0; i < edge_count; i++) {
        int from, to, weight;
        printf("Enter edge %d (from to weight): ", i + 1);
        if (scanf("%d %d %d", &from, &to, &weight) != 3) {
            printf("Invalid edge input\n");
            return 1;