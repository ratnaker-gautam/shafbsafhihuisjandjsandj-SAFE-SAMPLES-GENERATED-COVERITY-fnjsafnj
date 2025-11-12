//DeepSeek-V3 SAFE v1.3 Category: Safe ; Style: pointer_arithmetic ; Variation: graph_algorithms
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
    memset(g->nodes, 0, sizeof(g->nodes));
    memset(g->edges, 0, sizeof(g->edges));
}

int add_node(struct Graph *g, int node) {
    if (g->node_count >= MAX_NODES) return 0;
    if (node < 0 || node >= MAX_NODES) return 0;
    
    int *ptr = g->nodes;
    for (int i = 0; i < g->node_count; i++) {
        if (*(ptr + i) == node) return 0;
    }
    
    *(g->nodes + g->node_count) = node;
    g->node_count++;
    return 1;
}

int add_edge(struct Graph *g, int from, int to) {
    if (g->edge_count >= MAX_EDGES) return 0;
    if (from < 0 || from >= MAX_NODES) return 0;
    if (to < 0 || to >= MAX_NODES) return 0;
    
    int found_from = 0, found_to = 0;
    int *node_ptr = g->nodes;
    for (int i = 0; i < g->node_count; i++) {
        if (*(node_ptr + i) == from) found_from = 1;
        if (*(node_ptr + i) == to) found_to = 1;
    }
    if (!found_from || !found_to) return 0;
    
    int (*edge_ptr)[2] = g->edges;
    for (int i = 0; i < g->edge_count; i++) {
        if (*(*(edge_ptr + i) + 0) == from && *(*(edge_ptr + i) + 1) == to) {
            return 0;
        }
    }
    
    *(*(g->edges + g->edge_count) + 0) = from;
    *(*(g->edges + g->edge_count) + 1) = to;
    g->edge_count++;
    return 1;
}

void dfs(struct Graph *g, int node, int *visited) {
    if (node < 0 || node >= MAX_NODES) return;
    if (*(visited + node)) return;
    
    *(visited + node) = 1;
    printf("%d ", node);
    
    int (*edge_ptr)[2] = g->edges;
    for (int i = 0; i < g->edge_count; i++) {
        if (*(*(edge_ptr + i) + 0) == node) {
            int next = *(*(edge_ptr + i) + 1);
            if (!*(visited + next)) {
                dfs(g, next, visited);
            }
        }
    }
}

void perform_dfs(struct Graph *g, int start_node) {
    int visited[MAX_NODES] = {0};
    int *node_ptr = g->nodes;
    int found = 0;
    
    for (int i = 0; i < g->node_count; i++) {
        if (*(node_ptr + i) == start_node) {
            found = 1;
            break;
        }
    }
    
    if (!found) {
        printf("Start node %d not found in graph\n", start_node);
        return;
    }
    
    printf("DFS traversal from node %d: ", start_node);
    dfs(g, start_node, visited);
    printf("\n");
}

void print_graph(struct Graph *g) {
    printf("Graph nodes: ");
    int *node_ptr = g->nodes;
    for (int i = 0; i < g->node_count; i++) {
        printf("%d ", *(node_ptr + i));
    }
    printf("\n");
    
    printf("Graph edges: ");
    int (*edge_ptr)[2] = g->edges;
    for (int i = 0; i < g->edge_count; i++) {
        printf("(%d->%d) ", *(*(edge_ptr + i) + 0), *(*(edge_ptr + i) + 1));
    }
    printf("\n");
}

int main() {
    struct Graph g;
    init_graph(&g);
    
    printf("Adding nodes 0-4\n");
    for (int i = 0; i < 5; i++) {
        if (!add_node(&g, i)) {
            printf("Failed to add node %d\n", i);
        }
    }
    
    printf("Adding edges: 0->1, 0->2, 1->3, 2->4, 3->4\n");
    int edges[][2] = {{0,1}, {0,2}, {