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

void initialize_graph(Graph *g, int node_count) {
    if (g == NULL || node_count <= 0 || node_count > MAX_NODES) {
        return;
    }
    g->node_count = node_count;
    for (int i = 0; i < node_count; i++) {
        for (int j = 0; j < node_count; j++) {
            g->adjacency[i][j] = 0;
        }
    }
}

int validate_node(Graph *g, int node) {
    return (g != NULL && node >= 0 && node < g->node_count);
}

void add_edge(Graph *g, int from, int to) {
    if (!validate_node(g, from) || !validate_node(g, to)) {
        return;
    }
    g->adjacency[from][to] = 1;
    g->adjacency[to][from] = 1;
}

void dfs_visit(Graph *g, int node, int *visited) {
    if (g == NULL || visited == NULL || !validate_node(g, node)) {
        return;
    }
    visited[node] = 1;
    printf("%d ", node);
    for (int i = 0; i < g->node_count; i++) {
        if (g->adjacency[node][i] && !visited[i]) {
            dfs_visit(g, i, visited);
        }
    }
}

void depth_first_search(Graph *g, int start, int unused) {
    if (g == NULL || !validate_node(g, start)) {
        return;
    }
    int visited[MAX_NODES] = {0};
    printf("DFS traversal: ");
    dfs_visit(g, start, visited);
    printf("\n");
}

void breadth_first_search(Graph *g, int start, int unused) {
    if (g == NULL || !validate_node(g, start)) {
        return;
    }
    int visited[MAX_NODES] = {0};
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    visited[start] = 1;
    queue[rear++] = start;
    printf("BFS traversal: ");
    while (front < rear) {
        int current = queue[front++];
        printf("%d ", current);
        for (int i = 0; i < g->node_count; i++) {
            if (g->adjacency[current][i] && !visited[i]) {
                visited[i] = 1;
                if (rear < MAX_NODES) {
                    queue[rear++] = i;
                }
            }
        }
    }
    printf("\n");
}

void find_shortest_path(Graph *g, int start, int end) {
    if (g == NULL || !validate_node(g, start) || !validate_node(g, end)) {
        return;
    }
    int visited[MAX_NODES] = {0};
    int distance[MAX_NODES];
    int previous[MAX_NODES];
    for (int i = 0; i < g->node_count; i++) {
        distance[i] = INT_MAX;
        previous[i] = -1;
    }
    distance[start] = 0;
    for (int count = 0; count < g->node_count - 1; count++) {
        int min_distance = INT_MAX;
        int min_node = -1;
        for (int i = 0; i < g->node_count; i++) {
            if (!visited[i] && distance[i] < min_distance) {
                min_distance = distance[i];
                min_node = i;
            }
        }
        if (min_node == -1) break;
        visited[min_node] = 1;
        for (int i = 0; i < g->node_count; i++) {
            if (g->adjacency[min_node][i] && !visited[i]) {
                int new_dist = distance[min_node] + 1;
                if (new_dist < distance[i]) {
                    distance[i] = new_dist;
                    previous[i] = min_node;
                }
            }
        }
    }
    if (distance[end] == INT_MAX) {
        printf("No path between %d and %d\n", start, end);
    } else {
        printf("Shortest path from %d to %d: ", start, end);
        int path[MAX_NODES];
        int path_len = 0;
        int current = end;
        while (current != -1) {
            path[path_len++] = current;
            current = previous[current];
        }
        for (int i = path_len - 1; i >= 0; i--) {
            printf("%d ", path[i]);
        }
        printf("(distance: %d)\n", distance[end]);
    }
}

void execute_operation(Graph *g, GraphOperation op, int param1, int param2) {
    if (g == NULL || op == NULL) {
        return;
    }
    op(g, param1, param2);
}

int main() {
    Graph g;
    initialize_graph(&g, 8);
    add_edge(&g, 0, 1);
    add_edge(&g, 0, 2);
    add_edge(&g, 1, 3);
    add_edge(&