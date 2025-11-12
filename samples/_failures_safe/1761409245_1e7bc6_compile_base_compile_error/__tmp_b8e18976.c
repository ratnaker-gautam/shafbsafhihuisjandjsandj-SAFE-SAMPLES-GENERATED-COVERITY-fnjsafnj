//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_NODES 20

typedef struct Node {
    int id;
    int visited;
    int adj_count;
    int adj[MAX_NODES];
} Node;

typedef struct Graph {
    int node_count;
    Node nodes[MAX_NODES];
} Graph;

void init_graph(Graph *g) {
    if (g == NULL) return;
    g->node_count = 0;
    for (int i = 0; i < MAX_NODES; i++) {
        g->nodes[i].id = i;
        g->nodes[i].visited = 0;
        g->nodes[i].adj_count = 0;
        for (int j = 0; j < MAX_NODES; j++) {
            g->nodes[i].adj[j] = -1;
        }
    }
}

int add_edge(Graph *g, int from, int to) {
    if (g == NULL) return 0;
    if (from < 0 || from >= MAX_NODES || to < 0 || to >= MAX_NODES) return 0;
    if (from == to) return 0;
    
    if (g->node_count <= from) g->node_count = from + 1;
    if (g->node_count <= to) g->node_count = to + 1;
    
    Node *node = &g->nodes[from];
    if (node->adj_count >= MAX_NODES) return 0;
    
    for (int i = 0; i < node->adj_count; i++) {
        if (node->adj[i] == to) return 1;
    }
    
    node->adj[node->adj_count] = to;
    node->adj_count++;
    return 1;
}

void reset_visited(Graph *g) {
    if (g == NULL) return;
    for (int i = 0; i < g->node_count; i++) {
        g->nodes[i].visited = 0;
    }
}

void dfs_recursive(Graph *g, int node_id) {
    if (g == NULL) return;
    if (node_id < 0 || node_id >= g->node_count) return;
    if (g->nodes[node_id].visited) return;
    
    printf("%d ", node_id);
    g->nodes[node_id].visited = 1;
    
    Node *node = &g->nodes[node_id];
    for (int i = 0; i < node->adj_count; i++) {
        int neighbor = node->adj[i];
        if (neighbor >= 0 && neighbor < g->node_count) {
            dfs_recursive(g, neighbor);
        }
    }
}

int has_cycle_dfs(Graph *g, int node_id, int parent) {
    if (g == NULL) return 0;
    if (node_id < 0 || node_id >= g->node_count) return 0;
    
    g->nodes[node_id].visited = 1;
    
    Node *node = &g->nodes[node_id];
    for (int i = 0; i < node->adj_count; i++) {
        int neighbor = node->adj[i];
        if (neighbor < 0 || neighbor >= g->node_count) continue;
        
        if (!g->nodes[neighbor].visited) {
            if (has_cycle_dfs(g, neighbor, node_id)) {
                return 1;
            }
        } else if (neighbor != parent) {
            return 1;
        }
    }
    return 0;
}

int has_cycle(Graph *g) {
    if (g == NULL) return 0;
    reset_visited(g);
    
    for (int i = 0; i < g->node_count; i++) {
        if (!g->nodes[i].visited) {
            if (has_cycle_dfs(g, i, -1)) {
                return 1;
            }
        }
    }
    return 0;
}

int main() {
    Graph g;
    init_graph(&g);
    
    printf("Enter number of edges: ");
    int edge_count;
    if (scanf("%d", &edge_count) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    if (edge_count < 0 || edge_count > 100) {
        printf("Invalid edge count\n");
        return 1;
    }
    
    printf("Enter edges (from to):\n");
    for (int i = 0; i < edge_count; i++) {
        int from, to;
        if (scanf("%d %d", &from, &to) != 2) {
            printf("Invalid edge input\n");
            return 1;
        }
        if (from < 0 || from >= MAX_NODES || to < 0 || to >= MAX_NODES) {
            printf("Invalid node ID\n");
            return 1;
        }
        if (!add_edge(&g, from, to)) {
            printf("Failed to add edge\n");
            return 1;
        }
    }
    
    printf("\nDFS traversal: ");
    reset_visited(&g);
    for (int i = 0; i < g.node_count; i++) {
        if (!g.nodes[i].visited) {
            dfs_recursive(&g, i);
        }
    }
    printf("\n");
    
    if (has_cycle(&g)) {
        printf("Graph contains cycle\n");
    } else {
        printf("Graph is acyclic\n");