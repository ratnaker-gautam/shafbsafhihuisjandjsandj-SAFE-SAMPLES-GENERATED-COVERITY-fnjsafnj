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

void initialize_graph(Graph *g) {
    g->node_count = 0;
    g->edge_count = 0;
}

int add_edge(Graph *g, int from, int to, int weight) {
    if (g->edge_count >= MAX_EDGES) return 0;
    if (from < 0 || from >= MAX_NODES || to < 0 || to >= MAX_NODES) return 0;
    
    g->edges[g->edge_count].from = from;
    g->edges[g->edge_count].to = to;
    g->edges[g->edge_count].weight = weight;
    g->edge_count++;
    
    if (from >= g->node_count) g->node_count = from + 1;
    if (to >= g->node_count) g->node_count = to + 1;
    
    return 1;
}

void dfs_util(Graph *g, int node, int *visited) {
    if (node < 0 || node >= g->node_count || visited[node]) return;
    
    visited[node] = 1;
    printf("%d ", node);
    
    for (int i = 0; i < g->edge_count; i++) {
        if (g->edges[i].from == node) {
            dfs_util(g, g->edges[i].to, visited);
        }
    }
}

void depth_first_search(Graph *g, int start) {
    if (g == NULL || start < 0 || start >= g->node_count) return;
    
    int visited[MAX_NODES] = {0};
    printf("DFS from node %d: ", start);
    dfs_util(g, start, visited);
    printf("\n");
}

void breadth_first_search(Graph *g, int start) {
    if (g == NULL || start < 0 || start >= g->node_count) return;
    
    int visited[MAX_NODES] = {0};
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    
    visited[start] = 1;
    queue[rear++] = start;
    
    printf("BFS from node %d: ", start);
    
    while (front < rear) {
        int current = queue[front++];
        printf("%d ", current);
        
        for (int i = 0; i < g->edge_count; i++) {
            if (g->edges[i].from == current) {
                int neighbor = g->edges[i].to;
                if (!visited[neighbor] && neighbor < g->node_count) {
                    visited[neighbor] = 1;
                    queue[rear++] = neighbor;
                }
            }
        }
    }
    printf("\n");
}

void run_algorithm(Graph *g, int start, GraphAlgorithm algorithm) {
    if (g == NULL || algorithm == NULL) return;
    algorithm(g, start);
}

int main() {
    Graph g;
    initialize_graph(&g);
    
    printf("Building graph with 6 nodes...\n");
    
    add_edge(&g, 0, 1, 1);
    add_edge(&g, 0, 2, 1);
    add_edge(&g, 1, 3, 1);
    add_edge(&g, 1, 4, 1);
    add_edge(&g, 2, 5, 1);
    add_edge(&g, 3, 4, 1);
    
    printf("Graph has %d nodes and %d edges\n", g.node_count, g.edge_count);
    
    GraphAlgorithm algorithms[] = {depth_first_search, breadth_first_search};
    const char* names[] = {"Depth First Search", "Breadth First Search"};
    int algorithm_count = 2;
    
    for (int start_node = 0; start_node < 3; start_node++) {
        printf("\nStarting from node %d:\n", start_node);
        for (int i = 0; i < algorithm_count; i++) {
            printf("%s: ", names[i]);
            run_algorithm(&g, start_node, algorithms[i]);
        }
    }
    
    printf("\nTesting edge cases:\n");
    run_algorithm(&g, -1, depth_first_search);
    run_algorithm(&g, 100, breadth_first_search);
    
    Graph empty;
    initialize_graph(&empty);
    run_algorithm(&empty, 0, depth_first_search);
    
    return 0;
}