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

void initialize_graph(Graph* graph, int node_count) {
    if (graph == NULL || node_count <= 0 || node_count > MAX_NODES) {
        return;
    }
    graph->node_count = node_count;
    graph->edge_count = 0;
    memset(graph->edges, 0, sizeof(graph->edges));
}

int add_edge(Graph* graph, int from, int to, int weight) {
    if (graph == NULL || from < 0 || from >= graph->node_count || 
        to < 0 || to >= graph->node_count || 
        graph->edge_count >= MAX_EDGES) {
        return 0;
    }
    
    graph->edges[graph->edge_count].from = from;
    graph->edges[graph->edge_count].to = to;
    graph->edges[graph->edge_count].weight = weight;
    graph->edge_count++;
    return 1;
}

void print_graph(Graph* graph) {
    if (graph == NULL) return;
    
    printf("Graph with %d nodes and %d edges:\n", graph->node_count, graph->edge_count);
    for (int i = 0; i < graph->edge_count; i++) {
        printf("  %d -> %d (weight: %d)\n", 
               graph->edges[i].from, 
               graph->edges[i].to, 
               graph->edges[i].weight);
    }
}

void bfs_traversal(Graph* graph) {
    if (graph == NULL || graph->node_count == 0) return;
    
    int visited[MAX_NODES] = {0};
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    
    printf("BFS Traversal: ");
    queue[rear++] = 0;
    visited[0] = 1;
    
    while (front < rear) {
        int current = queue[front++];
        printf("%d ", current);
        
        for (int i = 0; i < graph->edge_count; i++) {
            if (graph->edges[i].from == current && !visited[graph->edges[i].to]) {
                queue[rear++] = graph->edges[i].to;
                visited[graph->edges[i].to] = 1;
            }
        }
    }
    printf("\n");
}

void dfs_util(Graph* graph, int node, int* visited) {
    if (node < 0 || node >= graph->node_count || visited == NULL) return;
    
    printf("%d ", node);
    visited[node] = 1;
    
    for (int i = 0; i < graph->edge_count; i++) {
        if (graph->edges[i].from == node && !visited[graph->edges[i].to]) {
            dfs_util(graph, graph->edges[i].to, visited);
        }
    }
}

void dfs_traversal(Graph* graph) {
    if (graph == NULL || graph->node_count == 0) return;
    
    int visited[MAX_NODES] = {0};
    printf("DFS Traversal: ");
    dfs_util(graph, 0, visited);
    printf("\n");
}

void find_minimum_edge(Graph* graph) {
    if (graph == NULL || graph->edge_count == 0) {
        printf("No edges in graph\n");
        return;
    }
    
    Edge min_edge = graph->edges[0];
    for (int i = 1; i < graph->edge_count; i++) {
        if (graph->edges[i].weight < min_edge.weight) {
            min_edge = graph->edges[i];
        }
    }
    
    printf("Minimum edge: %d -> %d (weight: %d)\n", 
           min_edge.from, min_edge.to, min_edge.weight);
}

void run_algorithm(Graph* graph, GraphAlgorithm algorithm) {
    if (graph != NULL && algorithm != NULL) {
        algorithm(graph);
    }
}

int main() {
    Graph graph;
    initialize_graph(&graph, 6);
    
    add_edge(&graph, 0, 1, 4);
    add_edge(&graph, 0, 2, 2);
    add_edge(&graph, 1, 3, 5);
    add_edge(&graph, 2, 3, 8);
    add_edge(&graph, 2, 4, 3);
    add_edge(&graph, 3, 5, 6);
    add_edge(&graph, 4, 5, 1);
    
    print_graph(&graph);
    printf("\n");
    
    GraphAlgorithm algorithms[] = {bfs_traversal, dfs_traversal, find_minimum_edge};
    const char* algorithm_names[] = {"BFS", "DFS", "Minimum Edge"};
    int algorithm_count = sizeof(algorithms) / sizeof(algorithms[0]);
    
    for (int i = 0; i < algorithm_count; i++) {
        printf("Running %s algorithm:\n", algorithm_names[i]);
        run_algorithm(&graph, algorithms[i]);
        printf("\n");
    }
    
    return 0;
}