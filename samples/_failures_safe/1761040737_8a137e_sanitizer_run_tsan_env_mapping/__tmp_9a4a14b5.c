//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: io_bound ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 100
#define MAX_EDGES 1000

struct Graph {
    int nodes[MAX_NODES];
    int edges[MAX_EDGES][2];
    int node_count;
    int edge_count;
};

void init_graph(struct Graph *g) {
    g->node_count = 0;
    g->edge_count = 0;
}

int add_node(struct Graph *g, int node) {
    if (g->node_count >= MAX_NODES) return 0;
    if (node < 0 || node >= MAX_NODES) return 0;
    for (int i = 0; i < g->node_count; i++) {
        if (g->nodes[i] == node) return 0;
    }
    g->nodes[g->node_count] = node;
    g->node_count++;
    return 1;
}

int add_edge(struct Graph *g, int from, int to) {
    if (g->edge_count >= MAX_EDGES) return 0;
    if (from < 0 || from >= MAX_NODES || to < 0 || to >= MAX_NODES) return 0;
    int from_exists = 0, to_exists = 0;
    for (int i = 0; i < g->node_count; i++) {
        if (g->nodes[i] == from) from_exists = 1;
        if (g->nodes[i] == to) to_exists = 1;
    }
    if (!from_exists || !to_exists) return 0;
    for (int i = 0; i < g->edge_count; i++) {
        if (g->edges[i][0] == from && g->edges[i][1] == to) return 0;
    }
    g->edges[g->edge_count][0] = from;
    g->edges[g->edge_count][1] = to;
    g->edge_count++;
    return 1;
}

void dfs(struct Graph *g, int node, int visited[]) {
    if (node < 0 || node >= MAX_NODES) return;
    visited[node] = 1;
    printf("%d ", node);
    for (int i = 0; i < g->edge_count; i++) {
        if (g->edges[i][0] == node && !visited[g->edges[i][1]]) {
            dfs(g, g->edges[i][1], visited);
        }
    }
}

void perform_dfs(struct Graph *g, int start_node) {
    int visited[MAX_NODES] = {0};
    int valid_start = 0;
    for (int i = 0; i < g->node_count; i++) {
        if (g->nodes[i] == start_node) {
            valid_start = 1;
            break;
        }
    }
    if (!valid_start) {
        printf("Start node not found in graph.\n");
        return;
    }
    printf("DFS traversal from node %d: ", start_node);
    dfs(g, start_node, visited);
    printf("\n");
}

int main() {
    struct Graph g;
    init_graph(&g);
    
    printf("Graph DFS Traversal Demo\n");
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    int node_count;
    if (scanf("%d", &node_count) != 1 || node_count < 1 || node_count > MAX_NODES) {
        printf("Invalid number of nodes.\n");
        return 1;
    }
    
    printf("Enter %d node IDs (0-%d): ", node_count, MAX_NODES-1);
    for (int i = 0; i < node_count; i++) {
        int node;
        if (scanf("%d", &node) != 1 || node < 0 || node >= MAX_NODES) {
            printf("Invalid node ID.\n");
            return 1;
        }
        if (!add_node(&g, node)) {
            printf("Failed to add node %d.\n", node);
            return 1;
        }
    }
    
    printf("Enter number of edges: ");
    int edge_count;
    if (scanf("%d", &edge_count) != 1 || edge_count < 0 || edge_count > MAX_EDGES) {
        printf("Invalid number of edges.\n");
        return 1;
    }
    
    if (edge_count > 0) {
        printf("Enter edges as 'from to' pairs:\n");
        for (int i = 0; i < edge_count; i++) {
            int from, to;
            if (scanf("%d %d", &from, &to) != 2) {
                printf("Invalid edge format.\n");
                return 1;
            }
            if (!add_edge(&g, from, to)) {
                printf("Failed to add edge %d->%d.\n", from, to);
                return 1;
            }
        }
    }
    
    printf("Enter start node for DFS: ");
    int start_node;
    if (scanf("%d", &start_node) != 1) {
        printf("Invalid start node.\n");
        return 1;
    }
    
    perform_dfs(&g, start_node);
    
    return 0;
}