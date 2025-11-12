//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 100
#define MAX_EDGES 1000

struct Graph {
    int num_nodes;
    int num_edges;
    int adjacency[MAX_NODES][MAX_NODES];
    int visited[MAX_NODES];
};

void initialize_graph(struct Graph *g) {
    g->num_nodes = 0;
    g->num_edges = 0;
    for (int i = 0; i < MAX_NODES; i++) {
        for (int j = 0; j < MAX_NODES; j++) {
            g->adjacency[i][j] = 0;
        }
        g->visited[i] = 0;
    }
}

int add_node(struct Graph *g) {
    if (g->num_nodes >= MAX_NODES) {
        return -1;
    }
    return g->num_nodes++;
}

int add_edge(struct Graph *g, int from, int to) {
    if (from < 0 || from >= g->num_nodes || to < 0 || to >= g->num_nodes) {
        return -1;
    }
    if (g->adjacency[from][to] == 0) {
        g->adjacency[from][to] = 1;
        g->num_edges++;
        return 0;
    }
    return -1;
}

void dfs(struct Graph *g, int node) {
    if (node < 0 || node >= g->num_nodes || g->visited[node]) {
        return;
    }
    g->visited[node] = 1;
    printf("%d ", node);
    for (int i = 0; i < g->num_nodes; i++) {
        if (g->adjacency[node][i] && !g->visited[i]) {
            dfs(g, i);
        }
    }
}

void reset_visited(struct Graph *g) {
    for (int i = 0; i < g->num_nodes; i++) {
        g->visited[i] = 0;
    }
}

int main() {
    struct Graph g;
    initialize_graph(&g);
    
    printf("Graph Construction\n");
    printf("Enter number of nodes (max %d): ", MAX_NODES);
    int node_count;
    if (scanf("%d", &node_count) != 1 || node_count <= 0 || node_count > MAX_NODES) {
        printf("Invalid number of nodes\n");
        return 1;
    }
    
    for (int i = 0; i < node_count; i++) {
        if (add_node(&g) < 0) {
            printf("Failed to add node\n");
            return 1;
        }
    }
    
    printf("Enter number of edges: ");
    int edge_count;
    if (scanf("%d", &edge_count) != 1 || edge_count < 0 || edge_count > MAX_EDGES) {
        printf("Invalid number of edges\n");
        return 1;
    }
    
    printf("Enter edges as 'from to' pairs (0-indexed):\n");
    for (int i = 0; i < edge_count; i++) {
        int from, to;
        if (scanf("%d %d", &from, &to) != 2) {
            printf("Invalid edge input\n");
            return 1;
        }
        if (add_edge(&g, from, to) < 0) {
            printf("Invalid edge: %d -> %d\n", from, to);
            return 1;
        }
    }
    
    printf("\nGraph Information:\n");
    printf("Nodes: %d\n", g.num_nodes);
    printf("Edges: %d\n", g.num_edges);
    
    printf("\nDFS Traversal starting from node 0:\n");
    reset_visited(&g);
    dfs(&g, 0);
    printf("\n");
    
    printf("\nDFS Traversal starting from all unvisited nodes:\n");
    reset_visited(&g);
    for (int i = 0; i < g.num_nodes; i++) {
        if (!g.visited[i]) {
            printf("Component starting at %d: ", i);
            dfs(&g, i);
            printf("\n");
        }
    }
    
    return 0;
}