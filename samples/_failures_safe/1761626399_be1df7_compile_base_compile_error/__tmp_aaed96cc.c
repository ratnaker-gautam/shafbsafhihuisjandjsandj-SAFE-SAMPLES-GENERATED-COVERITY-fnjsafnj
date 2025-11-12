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

void dfs_visit(Graph *g, int node, int *visited) {
    if (node < 0 || node >= g->node_count) return;
    visited[node] = 1;
    printf("%d ", node);
    for (int i = 0; i < g->node_count; i++) {
        if (g->adjacency[node][i] != 0 && !visited[i]) {
            dfs_visit(g, i, visited);
        }
    }
}

void depth_first_search(Graph *g, int start, int unused) {
    if (start < 0 || start >= g->node_count) {
        fprintf(stderr, "Invalid start node\n");
        return;
    }
    int visited[MAX_NODES] = {0};
    printf("DFS traversal: ");
    dfs_visit(g, start, visited);
    printf("\n");
}

void breadth_first_search(Graph *g, int start, int unused) {
    if (start < 0 || start >= g->node_count) {
        fprintf(stderr, "Invalid start node\n");
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
            if (g->adjacency[current][i] != 0 && !visited[i]) {
                visited[i] = 1;
                if (rear < MAX_NODES) {
                    queue[rear++] = i;
                } else {
                    fprintf(stderr, "Queue overflow\n");
                    return;
                }
            }
        }
    }
    printf("\n");
}

void find_minimum_spanning_tree(Graph *g, int unused1, int unused2) {
    int parent[MAX_NODES];
    int key[MAX_NODES];
    int mst_set[MAX_NODES];
    for (int i = 0; i < g->node_count; i++) {
        key[i] = INT_MAX;
        mst_set[i] = 0;
    }
    key[0] = 0;
    parent[0] = -1;
    for (int count = 0; count < g->node_count - 1; count++) {
        int min_key = INT_MAX;
        int min_index = -1;
        for (int v = 0; v < g->node_count; v++) {
            if (!mst_set[v] && key[v] < min_key) {
                min_key = key[v];
                min_index = v;
            }
        }
        if (min_index == -1) break;
        mst_set[min_index] = 1;
        for (int v = 0; v < g->node_count; v++) {
            if (g->adjacency[min_index][v] && !mst_set[v] && g->adjacency[min_index][v] < key[v]) {
                parent[v] = min_index;
                key[v] = g->adjacency[min_index][v];
            }
        }
    }
    printf("Minimum Spanning Tree edges:\n");
    for (int i = 1; i < g->node_count; i++) {
        if (parent[i] != -1) {
            printf("%d - %d (weight %d)\n", parent[i], i, g->adjacency[i][parent[i]]);
        }
    }
}

void execute_graph_operation(Graph *g, GraphOperation op, int param1, int param2) {
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
    add_edge(&g, 0, 2, 3);
    add_edge(&g, 1, 2,