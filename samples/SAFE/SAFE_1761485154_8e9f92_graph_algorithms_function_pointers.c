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

void initialize_graph(Graph* graph, int nodes) {
    if (graph == NULL || nodes <= 0 || nodes > MAX_NODES) {
        return;
    }
    graph->node_count = nodes;
    graph->edge_count = 0;
    memset(graph->edges, 0, sizeof(graph->edges));
}

int add_edge(Graph* graph, int from, int to, int weight) {
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

void print_graph(Graph* graph) {
    if (graph == NULL) {
        return;
    }
    printf("Graph with %d nodes and %d edges:\n", graph->node_count, graph->edge_count);
    for (int i = 0; i < graph->edge_count; i++) {
        printf("  %d -> %d (weight: %d)\n", graph->edges[i].from, 
               graph->edges[i].to, graph->edges[i].weight);
    }
}

void bfs_traversal(Graph* graph) {
    if (graph == NULL || graph->node_count <= 0) {
        return;
    }
    
    int visited[MAX_NODES];
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    
    for (int i = 0; i < graph->node_count; i++) {
        visited[i] = 0;
    }
    
    printf("BFS traversal: ");
    visited[0] = 1;
    queue[rear++] = 0;
    
    while (front < rear) {
        int current = queue[front++];
        printf("%d ", current);
        
        for (int i = 0; i < graph->edge_count; i++) {
            if (graph->edges[i].from == current && !visited[graph->edges[i].to]) {
                visited[graph->edges[i].to] = 1;
                if (rear < MAX_NODES) {
                    queue[rear++] = graph->edges[i].to;
                }
            }
        }
    }
    printf("\n");
}

void dfs_util(Graph* graph, int node, int* visited) {
    if (graph == NULL || visited == NULL || node < 0 || node >= graph->node_count) {
        return;
    }
    
    printf("%d ", node);
    visited[node] = 1;
    
    for (int i = 0; i < graph->edge_count; i++) {
        if (graph->edges[i].from == node && !visited[graph->edges[i].to]) {
            dfs_util(graph, graph->edges[i].to, visited);
        }
    }
}

void dfs_traversal(Graph* graph) {
    if (graph == NULL || graph->node_count <= 0) {
        return;
    }
    
    int visited[MAX_NODES];
    for (int i = 0; i < graph->node_count; i++) {
        visited[i] = 0;
    }
    
    printf("DFS traversal: ");
    dfs_util(graph, 0, visited);
    printf("\n");
}

void run_algorithm(Graph* graph, GraphAlgorithm algorithm) {
    if (graph != NULL && algorithm != NULL) {
        algorithm(graph);
    }
}

int main() {
    Graph graph;
    initialize_graph(&graph, 6);
    
    add_edge(&graph, 0, 1, 1);
    add_edge(&graph, 0, 2, 1);
    add_edge(&graph, 1, 3, 1);
    add_edge(&graph, 1, 4, 1);
    add_edge(&graph, 2, 4, 1);
    add_edge(&graph, 3, 5, 1);
    add_edge(&graph, 4, 5, 1);
    
    print_graph(&graph);
    printf("\n");
    
    GraphAlgorithm algorithms[] = {bfs_traversal, dfs_traversal};
    const char* names[] = {"BFS", "DFS"};
    int algorithm_count = 2;
    
    for (int i = 0; i < algorithm_count; i++) {
        printf("Running %s:\n", names[i]);
        run_algorithm(&graph, algorithms[i]);
        printf("\n");
    }
    
    return 0;
}