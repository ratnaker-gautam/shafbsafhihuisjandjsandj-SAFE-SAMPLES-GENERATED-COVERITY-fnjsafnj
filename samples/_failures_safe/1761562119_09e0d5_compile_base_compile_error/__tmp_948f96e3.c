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

typedef void (*GraphAlgorithm)(Graph*);

void initialize_graph(Graph *graph, int node_count) {
    if (graph == NULL || node_count <= 0 || node_count > MAX_NODES) {
        return;
    }
    graph->node_count = node_count;
    graph->edge_count = 0;
}

int add_edge(Graph *graph, int from, int to, int weight) {
    if (graph == NULL || from < 0 || to < 0 || from >= graph->node_count || 
        to >= graph->node_count || graph->edge_count >= MAX_EDGES) {
        return 0;
    }
    graph->edges[graph->edge_count].from = from;
    graph->edges[graph->edge_count].to = to;
    graph->edges[graph->edge_count].weight = weight;
    graph->edge_count++;
    return 1;
}

void print_graph(Graph *graph) {
    if (graph == NULL) return;
    printf("Graph with %d nodes and %d edges:\n", graph->node_count, graph->edge_count);
    for (int i = 0; i < graph->edge_count; i++) {
        printf("  %d -> %d (weight: %d)\n", graph->edges[i].from, 
               graph->edges[i].to, graph->edges[i].weight);
    }
}

void bfs_traversal(Graph *graph) {
    if (graph == NULL || graph->node_count == 0) return;
    
    int visited[MAX_NODES] = {0};
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    
    printf("BFS traversal: ");
    queue[rear++] = 0;
    visited[0] = 1;
    
    while (front < rear) {
        int current = queue[front++];
        printf("%d ", current);
        
        for (int i = 0; i < graph->edge_count; i++) {
            if (graph->edges[i].from == current && !visited[graph->edges[i].to]) {
                if (rear < MAX_NODES) {
                    queue[rear++] = graph->edges[i].to;
                    visited[graph->edges[i].to] = 1;
                }
            }
        }
    }
    printf("\n");
}

void dfs_util(Graph *graph, int node, int visited[]) {
    if (node < 0 || node >= graph->node_count || visited[node]) return;
    
    printf("%d ", node);
    visited[node] = 1;
    
    for (int i = 0; i < graph->edge_count; i++) {
        if (graph->edges[i].from == node && !visited[graph->edges[i].to]) {
            dfs_util(graph, graph->edges[i].to, visited);
        }
    }
}

void dfs_traversal(Graph *graph) {
    if (graph == NULL || graph->node_count == 0) return;
    
    int visited[MAX_NODES] = {0};
    printf("DFS traversal: ");
    dfs_util(graph, 0, visited);
    printf("\n");
}

void find_minimum_spanning_tree(Graph *graph) {
    if (graph == NULL || graph->node_count == 0 || graph->edge_count == 0) return;
    
    int parent[MAX_NODES];
    int key[MAX_NODES];
    int mst_set[MAX_NODES];
    
    for (int i = 0; i < graph->node_count; i++) {
        key[i] = 2147483647;
        mst_set[i] = 0;
        parent[i] = -1;
    }
    
    key[0] = 0;
    
    for (int count = 0; count < graph->node_count - 1; count++) {
        int min_key = 2147483647;
        int min_index = -1;
        
        for (int v = 0; v < graph->node_count; v++) {
            if (!mst_set[v] && key[v] < min_key) {
                min_key = key[v];
                min_index = v;
            }
        }
        
        if (min_index == -1) break;
        mst_set[min_index] = 1;
        
        for (int i = 0; i < graph->edge_count; i++) {
            if (graph->edges[i].from == min_index && 
                !mst_set[graph->edges[i].to] && 
                graph->edges[i].weight < key[graph->edges[i].to]) {
                parent[graph->edges[i].to] = min_index;
                key[graph->edges[i].to] = graph->edges[i].weight;
            }
        }
    }
    
    printf("Minimum Spanning Tree edges:\n");
    int total_weight = 0;
    for (int i = 1; i < graph->node_count; i++) {
        if (parent[i] != -1) {
            for (int j = 0; j < graph->edge_count; j++) {
                if ((graph->edges[j].from == parent[i] && graph->edges[j].