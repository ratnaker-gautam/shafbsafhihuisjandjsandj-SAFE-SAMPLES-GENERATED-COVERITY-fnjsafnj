//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 100
#define MAX_EDGES 1000

typedef struct {
    int from;
    int to;
    int weight;
} Edge;

typedef struct {
    int node_count;
    int edge_count;
    Edge edges[MAX_EDGES];
} Graph;

typedef void (*GraphAlgorithm)(Graph*, int);

void validate_graph(Graph *g) {
    if (g == NULL) {
        fprintf(stderr, "Graph is NULL\n");
        exit(1);
    }
    if (g->node_count <= 0 || g->node_count > MAX_NODES) {
        fprintf(stderr, "Invalid node count\n");
        exit(1);
    }
    if (g->edge_count < 0 || g->edge_count > MAX_EDGES) {
        fprintf(stderr, "Invalid edge count\n");
        exit(1);
    }
    for (int i = 0; i < g->edge_count; i++) {
        if (g->edges[i].from < 0 || g->edges[i].from >= g->node_count ||
            g->edges[i].to < 0 || g->edges[i].to >= g->node_count) {
            fprintf(stderr, "Invalid edge node indices\n");
            exit(1);
        }
    }
}

void bfs_traversal(Graph *g, int start) {
    validate_graph(g);
    if (start < 0 || start >= g->node_count) {
        fprintf(stderr, "Invalid start node\n");
        return;
    }
    
    int visited[MAX_NODES] = {0};
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    
    visited[start] = 1;
    queue[rear++] = start;
    
    printf("BFS traversal from node %d: ", start);
    
    while (front < rear) {
        int current = queue[front++];
        printf("%d ", current);
        
        for (int i = 0; i < g->edge_count; i++) {
            if (g->edges[i].from == current && !visited[g->edges[i].to]) {
                visited[g->edges[i].to] = 1;
                if (rear < MAX_NODES) {
                    queue[rear++] = g->edges[i].to;
                }
            }
        }
    }
    printf("\n");
}

void dfs_util(Graph *g, int node, int visited[]) {
    printf("%d ", node);
    visited[node] = 1;
    
    for (int i = 0; i < g->edge_count; i++) {
        if (g->edges[i].from == node && !visited[g->edges[i].to]) {
            dfs_util(g, g->edges[i].to, visited);
        }
    }
}

void dfs_traversal(Graph *g, int start) {
    validate_graph(g);
    if (start < 0 || start >= g->node_count) {
        fprintf(stderr, "Invalid start node\n");
        return;
    }
    
    int visited[MAX_NODES] = {0};
    printf("DFS traversal from node %d: ", start);
    dfs_util(g, start, visited);
    printf("\n");
}

void print_graph(Graph *g, int dummy) {
    validate_graph(g);
    printf("Graph with %d nodes and %d edges:\n", g->node_count, g->edge_count);
    for (int i = 0; i < g->edge_count; i++) {
        printf("  %d -> %d (weight: %d)\n", 
               g->edges[i].from, g->edges[i].to, g->edges[i].weight);
    }
}

void run_algorithm(GraphAlgorithm algo, Graph *g, int start) {
    if (algo == NULL) {
        fprintf(stderr, "Algorithm function pointer is NULL\n");
        return;
    }
    algo(g, start);
}

int main() {
    Graph g = {0};
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &g.node_count) != 1) {
        fprintf(stderr, "Invalid input for node count\n");
        return 1;
    }
    
    if (g.node_count <= 0 || g.node_count > MAX_NODES) {
        fprintf(stderr, "Node count out of range\n");
        return 1;
    }
    
    printf("Enter number of edges (0-%d): ", MAX_EDGES);
    if (scanf("%d", &g.edge_count) != 1) {
        fprintf(stderr, "Invalid input for edge count\n");
        return 1;
    }
    
    if (g.edge_count < 0 || g.edge_count > MAX_EDGES) {
        fprintf(stderr, "Edge count out of range\n");
        return 1;
    }
    
    printf("Enter edges as 'from to weight' (node indices 0-%d):\n", g.node_count-1);
    for (int i = 0; i < g.edge_count; i++) {
        if (scanf("%d %d %d", &g.edges[i].from, &g.edges[i].to, &g.edges[i].weight) != 3) {
            fprintf(stderr, "Invalid edge input\n");
            return 1;
        }
        if (g.edges[i].from < 0 || g