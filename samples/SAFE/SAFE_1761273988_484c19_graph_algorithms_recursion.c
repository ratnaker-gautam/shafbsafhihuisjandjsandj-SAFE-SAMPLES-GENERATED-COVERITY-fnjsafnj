//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_NODES 100

typedef struct {
    int adjacency[MAX_NODES][MAX_NODES];
    int node_count;
} Graph;

void initialize_graph(Graph *g, int node_count) {
    if (node_count <= 0 || node_count > MAX_NODES) {
        fprintf(stderr, "Invalid node count\n");
        exit(1);
    }
    g->node_count = node_count;
    for (int i = 0; i < node_count; i++) {
        for (int j = 0; j < node_count; j++) {
            g->adjacency[i][j] = 0;
        }
    }
}

void add_edge(Graph *g, int from, int to) {
    if (from < 0 || from >= g->node_count || to < 0 || to >= g->node_count) {
        fprintf(stderr, "Invalid edge\n");
        exit(1);
    }
    g->adjacency[from][to] = 1;
    g->adjacency[to][from] = 1;
}

void dfs_recursive(const Graph *g, int node, int visited[]) {
    if (node < 0 || node >= g->node_count) return;
    if (visited[node]) return;
    
    visited[node] = 1;
    printf("%d ", node);
    
    for (int neighbor = 0; neighbor < g->node_count; neighbor++) {
        if (g->adjacency[node][neighbor] && !visited[neighbor]) {
            dfs_recursive(g, neighbor, visited);
        }
    }
}

void depth_first_search(const Graph *g, int start_node) {
    if (start_node < 0 || start_node >= g->node_count) {
        fprintf(stderr, "Invalid start node\n");
        return;
    }
    
    int visited[MAX_NODES] = {0};
    printf("DFS traversal from node %d: ", start_node);
    dfs_recursive(g, start_node, visited);
    printf("\n");
}

int has_cycle_recursive(const Graph *g, int node, int parent, int visited[]) {
    if (node < 0 || node >= g->node_count) return 0;
    
    visited[node] = 1;
    
    for (int neighbor = 0; neighbor < g->node_count; neighbor++) {
        if (g->adjacency[node][neighbor]) {
            if (!visited[neighbor]) {
                if (has_cycle_recursive(g, neighbor, node, visited)) {
                    return 1;
                }
            } else if (neighbor != parent) {
                return 1;
            }
        }
    }
    return 0;
}

int detect_cycle(const Graph *g) {
    int visited[MAX_NODES] = {0};
    
    for (int node = 0; node < g->node_count; node++) {
        if (!visited[node]) {
            if (has_cycle_recursive(g, node, -1, visited)) {
                return 1;
            }
        }
    }
    return 0;
}

int main() {
    Graph g;
    int node_count, edge_count;
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &node_count) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    if (node_count <= 0 || node_count > MAX_NODES) {
        fprintf(stderr, "Invalid node count\n");
        return 1;
    }
    
    initialize_graph(&g, node_count);
    
    printf("Enter number of edges: ");
    if (scanf("%d", &edge_count) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    if (edge_count < 0 || edge_count > node_count * (node_count - 1) / 2) {
        fprintf(stderr, "Invalid edge count\n");
        return 1;
    }
    
    printf("Enter edges (from to):\n");
    for (int i = 0; i < edge_count; i++) {
        int from, to;
        if (scanf("%d %d", &from, &to) != 2) {
            fprintf(stderr, "Invalid edge input\n");
            return 1;
        }
        add_edge(&g, from, to);
    }
    
    int start_node;
    printf("Enter start node for DFS: ");
    if (scanf("%d", &start_node) != 1) {
        fprintf(stderr, "Invalid start node\n");
        return 1;
    }
    
    depth_first_search(&g, start_node);
    
    if (detect_cycle(&g)) {
        printf("Graph contains a cycle\n");
    } else {
        printf("Graph does not contain a cycle\n");
    }
    
    return 0;
}