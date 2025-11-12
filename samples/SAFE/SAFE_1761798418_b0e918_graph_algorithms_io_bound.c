//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: graph_algorithms
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
    for (int i = 0; i < g->node_count; i++) {
        if (g->nodes[i] == node) return 0;
    }
    g->nodes[g->node_count++] = node;
    return 1;
}

int add_edge(struct Graph *g, int from, int to) {
    if (g->edge_count >= MAX_EDGES) return 0;
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
    for (int i = 0; i < g->node_count; i++) {
        if (g->nodes[i] == node) {
            visited[i] = 1;
            break;
        }
    }
    for (int i = 0; i < g->edge_count; i++) {
        if (g->edges[i][0] == node) {
            int next_node = g->edges[i][1];
            int next_index = -1;
            for (int j = 0; j < g->node_count; j++) {
                if (g->nodes[j] == next_node) {
                    next_index = j;
                    break;
                }
            }
            if (next_index != -1 && !visited[next_index]) {
                dfs(g, next_node, visited);
            }
        }
    }
}

int is_connected(struct Graph *g) {
    if (g->node_count == 0) return 1;
    int visited[MAX_NODES] = {0};
    dfs(g, g->nodes[0], visited);
    for (int i = 0; i < g->node_count; i++) {
        if (!visited[i]) return 0;
    }
    return 1;
}

int main() {
    struct Graph g;
    init_graph(&g);
    
    printf("Enter number of nodes: ");
    int node_count;
    if (scanf("%d", &node_count) != 1 || node_count < 0 || node_count > MAX_NODES) {
        printf("Invalid node count\n");
        return 1;
    }
    
    printf("Enter node IDs (0-%d):\n", MAX_NODES - 1);
    for (int i = 0; i < node_count; i++) {
        int node;
        if (scanf("%d", &node) != 1 || node < 0 || node >= MAX_NODES) {
            printf("Invalid node ID\n");
            return 1;
        }
        if (!add_node(&g, node)) {
            printf("Failed to add node\n");
            return 1;
        }
    }
    
    printf("Enter number of edges: ");
    int edge_count;
    if (scanf("%d", &edge_count) != 1 || edge_count < 0 || edge_count > MAX_EDGES) {
        printf("Invalid edge count\n");
        return 1;
    }
    
    printf("Enter edges (from to):\n");
    for (int i = 0; i < edge_count; i++) {
        int from, to;
        if (scanf("%d %d", &from, &to) != 2) {
            printf("Invalid edge format\n");
            return 1;
        }
        if (!add_edge(&g, from, to)) {
            printf("Failed to add edge\n");
            return 1;
        }
    }
    
    printf("Graph connectivity check:\n");
    if (is_connected(&g)) {
        printf("Graph is connected\n");
    } else {
        printf("Graph is not connected\n");
    }
    
    printf("Graph summary:\n");
    printf("Nodes: ");
    for (int i = 0; i < g.node_count; i++) {
        printf("%d ", g.nodes[i]);
    }
    printf("\nEdges: ");
    for (int i = 0; i < g.edge_count; i++) {
        printf("(%d->%d) ", g.edges[i][0], g.edges[i][1]);
    }
    printf("\n");
    
    return 0;
}