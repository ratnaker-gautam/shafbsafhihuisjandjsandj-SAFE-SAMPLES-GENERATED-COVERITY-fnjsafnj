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

typedef void (*GraphOperation)(Graph*, int, int);

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

void add_edge(Graph *g, int from, int to, int weight) {
    if (g == NULL || from < 0 || from >= g->node_count || 
        to < 0 || to >= g->node_count || weight < 0) {
        return;
    }
    g->adjacency[from][to] = weight;
}

void print_graph(Graph *g, int start, int end) {
    if (g == NULL || start < 0 || start >= g->node_count || 
        end < 0 || end >= g->node_count) {
        return;
    }
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
    if (g == NULL || start < 0 || start >= g->node_count || 
        end < 0 || end >= g->node_count) {
        return;
    }
    
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
                    if (dist[i][j] > dist[i][k] + dist[k][j]) {
                        dist[i][j] = dist[i][k] + dist[k][j];
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
    if (g == NULL || node < 0 || node >= g->node_count || visited == NULL) {
        return;
    }
    visited[node] = 1;
    printf("%d ", node);
    for (int i = 0; i < g->node_count; i++) {
        if (g->adjacency[node][i] != INF && g->adjacency[node][i] != 0 && !visited[i]) {
            dfs_util(g, i, visited);
        }
    }
}

void depth_first_search(Graph *g, int start, int end) {
    if (g == NULL || start < 0 || start >= g->node_count || 
        end < 0 || end >= g->node_count) {
        return;
    }
    int visited[MAX_NODES] = {0};
    printf("DFS traversal from node %d: ", start);
    dfs_util(g, start, visited);
    printf("\n");
}

void execute_operation(GraphOperation op, Graph *g, int start, int end) {
    if (op == NULL || g == NULL) {
        return;
    }
    op(g, start, end);
}

int main() {
    Graph graph;
    int node_count = 5;
    
    initialize_graph(&graph, node_count);
    
    add_edge(&graph, 0, 1, 2);
    add_edge(&graph, 0, 3, 6);
    add_edge(&graph, 1, 2, 3);
    add_edge(&graph, 1, 3, 8);
    add_edge(&graph, 1, 4, 5);
    add_edge(&graph, 2, 1, 1);
    add_edge(&graph, 3, 2, 2);
    add_edge(&graph, 4, 3, 3);
    
    GraphOperation operations[] = {print_graph, floyd_warshall, depth_first_search};
    int num_operations = sizeof(operations) / sizeof(G