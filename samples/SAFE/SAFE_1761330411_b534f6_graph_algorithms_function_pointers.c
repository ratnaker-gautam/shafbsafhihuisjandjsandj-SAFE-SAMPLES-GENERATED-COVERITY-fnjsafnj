//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

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
        exit(EXIT_FAILURE);
    }
    if (g->node_count <= 0 || g->node_count > MAX_NODES) {
        fprintf(stderr, "Invalid node count: %d\n", g->node_count);
        exit(EXIT_FAILURE);
    }
    if (g->edge_count < 0 || g->edge_count > MAX_EDGES) {
        fprintf(stderr, "Invalid edge count: %d\n", g->edge_count);
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < g->edge_count; i++) {
        if (g->edges[i].from < 0 || g->edges[i].from >= g->node_count ||
            g->edges[i].to < 0 || g->edges[i].to >= g->node_count) {
            fprintf(stderr, "Invalid edge: %d -> %d\n", g->edges[i].from, g->edges[i].to);
            exit(EXIT_FAILURE);
        }
    }
}

void bfs_traversal(Graph *g, int start) {
    validate_graph(g);
    if (start < 0 || start >= g->node_count) {
        fprintf(stderr, "Invalid start node: %d\n", start);
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
                } else {
                    fprintf(stderr, "Queue overflow\n");
                    return;
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
        fprintf(stderr, "Invalid start node: %d\n", start);
        return;
    }
    
    int visited[MAX_NODES] = {0};
    printf("DFS traversal from node %d: ", start);
    dfs_util(g, start, visited);
    printf("\n");
}

void run_algorithm(GraphAlgorithm algo, Graph *g, int start) {
    if (algo == NULL) {
        fprintf(stderr, "Algorithm function is NULL\n");
        return;
    }
    algo(g, start);
}

Graph create_sample_graph() {
    Graph g;
    g.node_count = 6;
    g.edge_count = 7;
    
    Edge edges[] = {
        {0, 1, 1}, {0, 2, 1}, {1, 3, 1}, 
        {2, 3, 1}, {2, 4, 1}, {3, 5, 1}, {4, 5, 1}
    };
    
    for (int i = 0; i < g.edge_count; i++) {
        g.edges[i] = edges[i];
    }
    
    return g;
}

int main() {
    Graph g = create_sample_graph();
    validate_graph(&g);
    
    printf("Graph with %d nodes and %d edges\n", g.node_count, g.edge_count);
    printf("Edges: ");
    for (int i = 0; i < g.edge_count; i++) {
        printf("(%d->%d) ", g.edges[i].from, g.edges[i].to);
    }
    printf("\n\n");
    
    GraphAlgorithm algorithms[] = {bfs_traversal, dfs_traversal};
    const char *names[] = {"BFS", "DFS"};
    int algorithm_count = sizeof(algorithms) / sizeof(algorithms[0]);
    
    for (int i = 0; i < algorithm_count; i++) {
        printf("Running %s:\n", names[i]);
        run_algorithm(algorithms[i], &g, 0);
        printf("\n");
    }
    
    return 0;
}