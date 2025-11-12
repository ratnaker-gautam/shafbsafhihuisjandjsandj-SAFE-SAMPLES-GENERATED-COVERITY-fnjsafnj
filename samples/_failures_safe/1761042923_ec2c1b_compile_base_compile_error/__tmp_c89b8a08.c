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

void initialize_graph(Graph *g, int nodes) {
    if (nodes <= 0 || nodes > MAX_NODES) {
        fprintf(stderr, "Invalid node count\n");
        exit(1);
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
        exit(1);
    }
    if (weight < 0 || weight > 1000) {
        fprintf(stderr, "Invalid edge weight\n");
        exit(1);
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

void bfs_traversal(Graph *g, int start, int visited[]) {
    if (start < 0 || start >= g->node_count) return;
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    visited[start] = 1;
    queue[rear++] = start;
    while (front < rear) {
        int current = queue[front++];
        printf("%d ", current);
        for (int i = 0; i < g->node_count; i++) {
            if (g->adjacency[current][i] != 0 && !visited[i]) {
                visited[i] = 1;
                if (rear >= MAX_NODES) {
                    fprintf(stderr, "Queue overflow\n");
                    exit(1);
                }
                queue[rear++] = i;
            }
        }
    }
}

void perform_dfs(Graph *g, int start, int unused) {
    int visited[MAX_NODES] = {0};
    printf("DFS traversal: ");
    dfs_traversal(g, start, visited);
    printf("\n");
}

void perform_bfs(Graph *g, int start, int unused) {
    int visited[MAX_NODES] = {0};
    printf("BFS traversal: ");
    bfs_traversal(g, start, visited);
    printf("\n");
}

void find_shortest_path(Graph *g, int start, int end) {
    if (start < 0 || start >= g->node_count || end < 0 || end >= g->node_count) {
        fprintf(stderr, "Invalid node indices\n");
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
            if (!visited[i] && g->adjacency[min_index][i] != 0) {
                long new_dist = (long)dist[min_index] + g->adjacency[min_index][i];
                if (new_dist < dist[i] && new_dist <= INT_MAX) {
                    dist[i] = (int)new_dist;
                }
            }
        }
    }
    if (dist[end] == INT_MAX) {
        printf("No path from %d to %d\n", start, end);
    } else {
        printf("Shortest distance from %d to %d: %d\n", start, end, dist[end]);
    }
}

void execute_operation(Graph *g, GraphOperation op, int param1, int param2) {
    if (g == NULL || op == NULL) {
        fprintf(stderr, "Invalid operation parameters\n");
        return;
    }
    op(g, param1, param2);
}

int main() {
    Graph g;
    initialize_graph(&g, 6);
    add_edge(&g, 0, 1, 4);
    add_edge(&g, 0, 2, 2);
    add_edge(&g, 1, 2, 1);
    add_edge(&g