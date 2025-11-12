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
    if (dist[target] != INT_MAX) {
        printf("Shortest path from %d to %d: %d\n", start, target, dist[target]);
    } else {
        printf("No path from %d to %d\n", start, target);
    }
}

void execute_operation(Graph *g, GraphOperation op, int start, int target) {
    int visited[MAX_NODES] = {0};
    op(g, start, target);
}

int main() {
    Graph g;
    int nodes, edges;
    printf("Enter number of nodes (1-100): ");
    if (scanf("%d", &nodes) != 1 || nodes < 1 || nodes > MAX_NODES) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    initialize_graph(&g, nodes);
    printf("Enter number of edges: ");
    if (scanf("%d", &edges) != 1 || edges < 0 || edges > nodes * (nodes - 1) / 2) {
        fprintf(stderr, "Invalid edge count\n");
        return 1;
    }
    for (int i = 0; i < edges; i++) {
        int from, to, weight;
        printf("Enter edge %d (from to weight): ", i + 1);
        if (scanf