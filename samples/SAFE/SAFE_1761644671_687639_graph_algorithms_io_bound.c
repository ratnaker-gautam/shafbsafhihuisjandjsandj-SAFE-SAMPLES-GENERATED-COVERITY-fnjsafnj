//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 100
#define MAX_EDGES 1000

struct Graph {
    int nodes;
    int edges[MAX_EDGES][2];
    int edge_count;
};

void initialize_graph(struct Graph *g, int nodes) {
    if (nodes <= 0 || nodes > MAX_NODES) {
        printf("Invalid number of nodes. Must be between 1 and %d.\n", MAX_NODES);
        exit(1);
    }
    g->nodes = nodes;
    g->edge_count = 0;
}

int add_edge(struct Graph *g, int u, int v) {
    if (g->edge_count >= MAX_EDGES) {
        printf("Maximum edge count reached.\n");
        return 0;
    }
    if (u < 0 || u >= g->nodes || v < 0 || v >= g->nodes) {
        printf("Invalid node indices. Must be between 0 and %d.\n", g->nodes - 1);
        return 0;
    }
    g->edges[g->edge_count][0] = u;
    g->edges[g->edge_count][1] = v;
    g->edge_count++;
    return 1;
}

void bfs(struct Graph *g, int start) {
    if (start < 0 || start >= g->nodes) {
        printf("Invalid start node.\n");
        return;
    }
    
    int visited[MAX_NODES] = {0};
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    
    visited[start] = 1;
    queue[rear++] = start;
    
    printf("BFS traversal starting from node %d: ", start);
    
    while (front < rear) {
        int current = queue[front++];
        printf("%d ", current);
        
        for (int i = 0; i < g->edge_count; i++) {
            if (g->edges[i][0] == current) {
                int neighbor = g->edges[i][1];
                if (!visited[neighbor]) {
                    visited[neighbor] = 1;
                    queue[rear++] = neighbor;
                }
            }
        }
    }
    printf("\n");
}

void dfs_util(struct Graph *g, int node, int visited[]) {
    visited[node] = 1;
    printf("%d ", node);
    
    for (int i = 0; i < g->edge_count; i++) {
        if (g->edges[i][0] == node) {
            int neighbor = g->edges[i][1];
            if (!visited[neighbor]) {
                dfs_util(g, neighbor, visited);
            }
        }
    }
}

void dfs(struct Graph *g, int start) {
    if (start < 0 || start >= g->nodes) {
        printf("Invalid start node.\n");
        return;
    }
    
    int visited[MAX_NODES] = {0};
    printf("DFS traversal starting from node %d: ", start);
    dfs_util(g, start, visited);
    printf("\n");
}

int main() {
    struct Graph g;
    int nodes, edges;
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &nodes) != 1) {
        printf("Invalid input.\n");
        return 1;
    }
    initialize_graph(&g, nodes);
    
    printf("Enter number of edges: ");
    if (scanf("%d", &edges) != 1) {
        printf("Invalid input.\n");
        return 1;
    }
    
    if (edges < 0 || edges > MAX_EDGES) {
        printf("Invalid number of edges. Must be between 0 and %d.\n", MAX_EDGES);
        return 1;
    }
    
    printf("Enter edges as pairs of node indices (0 to %d):\n", nodes - 1);
    for (int i = 0; i < edges; i++) {
        int u, v;
        if (scanf("%d %d", &u, &v) != 2) {
            printf("Invalid input.\n");
            return 1;
        }
        if (!add_edge(&g, u, v)) {
            return 1;
        }
    }
    
    int start_node;
    printf("Enter start node for traversals: ");
    if (scanf("%d", &start_node) != 1) {
        printf("Invalid input.\n");
        return 1;
    }
    
    bfs(&g, start_node);
    dfs(&g, start_node);
    
    return 0;
}