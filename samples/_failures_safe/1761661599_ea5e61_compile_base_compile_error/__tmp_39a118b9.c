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

void initialize_graph(Graph *g, int nodes) {
    if (nodes <= 0 || nodes > MAX_NODES) {
        fprintf(stderr, "Invalid node count\n");
        exit(EXIT_FAILURE);
    }
    g->node_count = nodes;
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            g->adjacency[i][j] = 0;
        }
    }
}

void add_edge(Graph *g, int from, int to, int weight) {
    if (from < 0 || from >= g->node_count || to < 0 || to >= g->node_count) {
        fprintf(stderr, "Invalid node index\n");
        exit(EXIT_FAILURE);
    }
    if (weight < 0 || weight > 1000) {
        fprintf(stderr, "Invalid edge weight\n");
        exit(EXIT_FAILURE);
    }
    g->adjacency[from][to] = weight;
    g->adjacency[to][from] = weight;
}

void dfs_traversal(Graph *g, int start, int visited[]) {
    if (start < 0 || start >= g->node_count) return;
    visited[start] = 1;
    printf("%d ", start);
    for (int i = 0; i < g->node_count; i++) {
        if (g->adjacency[start][i] != 0 && !visited[i]) {
            dfs_traversal(g, i, visited);
        }
    }
}

void bfs_traversal(Graph *g, int start, int target) {
    if (start < 0 || start >= g->node_count || target < 0 || target >= g->node_count) {
        fprintf(stderr, "Invalid start or target node\n");
        return;
    }
    int visited[MAX_NODES] = {0};
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    visited[start] = 1;
    queue[rear++] = start;
    printf("BFS from %d: ", start);
    while (front < rear) {
        int current = queue[front++];
        printf("%d ", current);
        if (current == target) {
            printf("(target found)");
            break;
        }
        for (int i = 0; i < g->node_count; i++) {
            if (g->adjacency[current][i] != 0 && !visited[i]) {
                visited[i] = 1;
                if (rear >= MAX_NODES) {
                    fprintf(stderr, "Queue overflow\n");
                    return;
                }
                queue[rear++] = i;
            }
        }
    }
    printf("\n");
}

void dijkstra_shortest_path(Graph *g, int start, int target) {
    if (start < 0 || start >= g->node_count || target < 0 || target >= g->node_count) {
        fprintf(stderr, "Invalid start or target node\n");
        return;
    }
    int dist[MAX_NODES];
    int visited[MAX_NODES] = {0};
    for (int i = 0; i < g->node_count; i++) {
        dist[i] = INT_MAX;
    }
    dist[start] = 0;
    for (int count = 0; count < g->node_count - 1; count++) {
        int min_dist = INT_MAX;
        int min_index = -1;
        for (int i = 0; i < g->node_count; i++) {
            if (!visited[i] && dist[i] < min_dist) {
                min_dist = dist[i];
                min_index = i;
            }
        }
        if (min_index == -1) break;
        visited[min_index] = 1;
        for (int i = 0; i < g->node_count; i++) {
            if (!visited[i] && g->adjacency[min_index][i] != 0 && dist[min_index] != INT_MAX) {
                int new_dist = dist[min_index] + g->adjacency[min_index][i];
                if (new_dist > 0 && new_dist < dist[i]) {
                    dist[i] = new_dist;
                }
            }
        }
    }
    printf("Shortest distance from %d to %d: %d\n", start, target, dist[target] == INT_MAX ? -1 : dist[target]);
}

void execute_operation(Graph *g, GraphOperation op, int param1, int param2) {
    if (g == NULL || op == NULL) {
        fprintf(stderr, "Invalid graph or operation\n");
        return;
    }
    op(g, param1, param2);
}

void print_dfs(Graph *g, int start, int unused) {
    int visited[MAX_NODES] = {0};
    printf("DFS from %d: ", start);
    dfs_traversal(g, start, visited);
    printf("\n");
}

void print_bfs(Graph *g, int start, int target) {
    bfs_traversal(g, start, target);
}

void print_dijkstra(Graph *g, int start, int target) {