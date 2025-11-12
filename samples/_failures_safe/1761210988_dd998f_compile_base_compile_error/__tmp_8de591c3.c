//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_NODES 100

struct Graph {
    int nodes;
    int edges;
    int adj[MAX_NODES][MAX_NODES];
};

void init_graph(struct Graph *g, int nodes) {
    if (g == NULL || nodes <= 0 || nodes > MAX_NODES) {
        fprintf(stderr, "Invalid graph initialization\n");
        exit(EXIT_FAILURE);
    }
    
    g->nodes = nodes;
    g->edges = 0;
    
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            *(*(g->adj + i) + j) = 0;
        }
    }
}

void add_edge(struct Graph *g, int u, int v) {
    if (g == NULL || u < 0 || u >= g->nodes || v < 0 || v >= g->nodes) {
        fprintf(stderr, "Invalid edge parameters\n");
        return;
    }
    
    if (*(*(g->adj + u) + v) == 0) {
        *(*(g->adj + u) + v) = 1;
        *(*(g->adj + v) + u) = 1;
        g->edges++;
    }
}

void dfs_util(struct Graph *g, int node, int *visited) {
    if (g == NULL || visited == NULL || node < 0 || node >= g->nodes) {
        return;
    }
    
    *(visited + node) = 1;
    printf("%d ", node);
    
    for (int i = 0; i < g->nodes; i++) {
        if (*(*(g->adj + node) + i) && !*(visited + i)) {
            dfs_util(g, i, visited);
        }
    }
}

void dfs(struct Graph *g, int start) {
    if (g == NULL || start < 0 || start >= g->nodes) {
        fprintf(stderr, "Invalid DFS parameters\n");
        return;
    }
    
    int visited[MAX_NODES];
    for (int i = 0; i < g->nodes; i++) {
        *(visited + i) = 0;
    }
    
    printf("DFS traversal: ");
    dfs_util(g, start, visited);
    printf("\n");
}

void bfs(struct Graph *g, int start) {
    if (g == NULL || start < 0 || start >= g->nodes) {
        fprintf(stderr, "Invalid BFS parameters\n");
        return;
    }
    
    int visited[MAX_NODES];
    for (int i = 0; i < g->nodes; i++) {
        *(visited + i) = 0;
    }
    
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    
    *(visited + start) = 1;
    *(queue + rear) = start;
    rear = (rear + 1) % MAX_NODES;
    
    printf("BFS traversal: ");
    
    while (front != rear) {
        int current = *(queue + front);
        front = (front + 1) % MAX_NODES;
        printf("%d ", current);
        
        for (int i = 0; i < g->nodes; i++) {
            if (*(*(g->adj + current) + i) && !*(visited + i)) {
                *(visited + i) = 1;
                *(queue + rear) = i;
                rear = (rear + 1) % MAX_NODES;
            }
        }
    }
    printf("\n");
}

void print_graph(struct Graph *g) {
    if (g == NULL) {
        fprintf(stderr, "Invalid graph\n");
        return;
    }
    
    printf("Graph with %d nodes and %d edges:\n", g->nodes, g->edges);
    for (int i = 0; i < g->nodes; i++) {
        printf("Node %d: ", i);
        for (int j = 0; j < g->nodes; j++) {
            if (*(*(g->adj + i) + j)) {
                printf("%d ", j);
            }
        }
        printf("\n");
    }
}

int main() {
    struct Graph g;
    int nodes, edges;
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &nodes) != 1 || nodes <= 0 || nodes > MAX_NODES) {
        fprintf(stderr, "Invalid number of nodes\n");
        return EXIT_FAILURE;
    }
    
    init_graph(&g, nodes);
    
    printf("Enter number of edges: ");
    if (scanf("%d", &edges) != 1 || edges < 0 || edges > nodes * (nodes - 1) / 2) {
        fprintf(stderr, "Invalid number of edges\n");
        return EXIT_FAILURE;
    }
    
    printf("Enter edges (u v pairs):\n");
    for (int i = 0; i < edges; i++) {
        int u, v;
        if (scanf("%d %d", &u, &v) != 2 || u < 0 || u >= nodes || v < 0 || v >= nodes) {
            fprintf(stderr, "Invalid edge\n");
            return EXIT_FAILURE;
        }
        add_edge(&g, u, v);