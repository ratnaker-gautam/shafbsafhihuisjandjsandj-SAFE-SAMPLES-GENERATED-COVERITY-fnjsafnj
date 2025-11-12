//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: function_pointers ; Variation: graph_algorithms
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

typedef void (*GraphOperation)(Graph*, int, int);

void initialize_graph(Graph *g, int node_count) {
    if (node_count <= 0 || node_count > MAX_NODES) {
        fprintf(stderr, "Invalid node count\n");
        exit(1);
    }
    g->node_count = node_count;
    for (int i = 0; i < node_count; i++) {
        for (int j = 0; j < node_count; j++) {
            g->adjacency[i][j] = (i == j) ? 0 : INF;
        }
    }
}

void add_edge(Graph *g, int from, int to, int weight) {
    if (from < 0 || from >= g->node_count || to < 0 || to >= g->node_count) {
        fprintf(stderr, "Invalid node index\n");
        return;
    }
    if (weight < 0) {
        fprintf(stderr, "Negative weights not allowed\n");
        return;
    }
    g->adjacency[from][to] = weight;
}

void print_graph(Graph *g, int start, int end) {
    if (g == NULL) return;
    printf("Graph adjacency matrix:\n");
    for (int i = 0; i < g->node_count; i++) {
        for (int j = 0; j < g->node_count; j++) {
            if (g->adjacency[i][j] == INF) {
                printf("INF ");
            } else {
                printf("%3d ", g->adjacency[i][j]);
            }
        }
        printf("\n");
    }
}

void floyd_warshall(Graph *g, int start, int end) {
    if (g == NULL) return;
    int dist[MAX_NODES][MAX_NODES];
    
    for (int i = 0; i < g->node_count; i++) {
        for (int j = 0; j < g->node_count; j++) {
            dist[i][j] = g->adjacency[i][j];
        }
    }
    
    for (int k = 0; k < g->node_count; k++) {
        for (int i = 0; i < g->node_count; i++) {
            for (int j = 0; j < g->node_count; j++) {
                if (dist[i][k] != INF && dist[k][j] != INF) {
                    if (dist[i][k] > INF - dist[k][j]) {
                        continue;
                    }
                    int new_dist = dist[i][k] + dist[k][j];
                    if (dist[i][j] > new_dist) {
                        dist[i][j] = new_dist;
                    }
                }
            }
        }
    }
    
    printf("Shortest path distances:\n");
    for (int i = 0; i < g->node_count; i++) {
        for (int j = 0; j < g->node_count; j++) {
            if (dist[i][j] == INF) {
                printf("INF ");
            } else {
                printf("%3d ", dist[i][j]);
            }
        }
        printf("\n");
    }
}

void dfs_util(Graph *g, int node, int visited[]) {
    if (node < 0 || node >= g->node_count || visited[node]) return;
    visited[node] = 1;
    printf("%d ", node);
    
    for (int i = 0; i < g->node_count; i++) {
        if (g->adjacency[node][i] != INF && g->adjacency[node][i] != 0 && !visited[i]) {
            dfs_util(g, i, visited);
        }
    }
}

void depth_first_search(Graph *g, int start, int end) {
    if (g == NULL || start < 0 || start >= g->node_count) return;
    int visited[MAX_NODES] = {0};
    printf("DFS traversal from node %d: ", start);
    dfs_util(g, start, visited);
    printf("\n");
}

void execute_operation(Graph *g, GraphOperation op, int param1, int param2) {
    if (g == NULL || op == NULL) return;
    op(g, param1, param2);
}

int main() {
    Graph g;
    initialize_graph(&g, 6);
    
    add_edge(&g, 0, 1, 2);
    add_edge(&g, 0, 2, 4);
    add_edge(&g, 1, 2, 1);
    add_edge(&g, 1, 3, 7);
    add_edge(&g, 2, 4, 3);
    add_edge(&g, 3, 4, 1);
    add_edge(&g, 3, 5, 5);
    add_edge(&g, 4, 5, 2);
    
    GraphOperation operations[] = {print_graph, floyd_warshall, depth_first_search};
    int operation_count = sizeof(operations) / sizeof(operations[0]);
    
    for (int i = 0; i < operation_count; i++) {
        execute_operation(&g, operations[i], 0, 0);