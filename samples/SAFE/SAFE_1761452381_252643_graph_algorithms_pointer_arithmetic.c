//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: graph_algorithms
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
    for (int i = 0; i < MAX_NODES; i++) {
        *(g->nodes + i) = -1;
    }
    for (int i = 0; i < MAX_EDGES; i++) {
        *((*(g->edges + i)) + 0) = -1;
        *((*(g->edges + i)) + 1) = -1;
    }
}

int add_node(struct Graph *g, int id) {
    if (g->node_count >= MAX_NODES) return 0;
    if (id < 0 || id >= MAX_NODES) return 0;
    for (int i = 0; i < g->node_count; i++) {
        if (*(g->nodes + i) == id) return 0;
    }
    *(g->nodes + g->node_count) = id;
    g->node_count++;
    return 1;
}

int add_edge(struct Graph *g, int from, int to) {
    if (g->edge_count >= MAX_EDGES) return 0;
    if (from < 0 || from >= MAX_NODES) return 0;
    if (to < 0 || to >= MAX_NODES) return 0;
    
    int found_from = 0, found_to = 0;
    for (int i = 0; i < g->node_count; i++) {
        if (*(g->nodes + i) == from) found_from = 1;
        if (*(g->nodes + i) == to) found_to = 1;
    }
    if (!found_from || !found_to) return 0;
    
    for (int i = 0; i < g->edge_count; i++) {
        if (*((*(g->edges + i)) + 0) == from && *((*(g->edges + i)) + 1) == to) {
            return 0;
        }
    }
    
    *((*(g->edges + g->edge_count)) + 0) = from;
    *((*(g->edges + g->edge_count)) + 1) = to;
    g->edge_count++;
    return 1;
}

void dfs(struct Graph *g, int node, int *visited) {
    if (node < 0 || node >= MAX_NODES) return;
    if (*(visited + node)) return;
    
    *(visited + node) = 1;
    printf("%d ", node);
    
    for (int i = 0; i < g->edge_count; i++) {
        if (*((*(g->edges + i)) + 0) == node) {
            int next = *((*(g->edges + i)) + 1);
            if (!*(visited + next)) {
                dfs(g, next, visited);
            }
        }
    }
}

void depth_first_search(struct Graph *g, int start) {
    if (start < 0 || start >= MAX_NODES) return;
    
    int visited[MAX_NODES];
    for (int i = 0; i < MAX_NODES; i++) {
        *(visited + i) = 0;
    }
    
    int valid_start = 0;
    for (int i = 0; i < g->node_count; i++) {
        if (*(g->nodes + i) == start) {
            valid_start = 1;
            break;
        }
    }
    if (!valid_start) return;
    
    printf("DFS traversal from %d: ", start);
    dfs(g, start, visited);
    printf("\n");
}

int main() {
    struct Graph g;
    init_graph(&g);
    
    printf("Adding nodes 0-4\n");
    for (int i = 0; i < 5; i++) {
        if (!add_node(&g, i)) {
            printf("Failed to add node %d\n", i);
            return 1;
        }
    }
    
    printf("Adding edges: 0-1, 0-2, 1-3, 2-4\n");
    int edges[][2] = {{0,1}, {0,2}, {1,3}, {2,4}};
    for (int i = 0; i < 4; i++) {
        if (!add_edge(&g, *(*(edges + i) + 0), *(*(edges + i) + 1))) {
            printf("Failed to add edge %d-%d\n", *(*(edges + i) + 0), *(*(edges + i) + 1));
            return 1;
        }
    }
    
    depth_first_search(&g, 0);
    depth_first_search(&g, 3);
    
    printf("Testing invalid inputs:\n");
    add_node(&g, -1);
    add_node(&g, 1000);
    add_edge(&g, 0, 10);
    add_edge(&g, 10, 0);
    depth_first_search(&g, 10);
    
    return 0;
}