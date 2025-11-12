//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_NODES 100
#define INF INT_MAX

typedef struct {
    int adjacency[MAX_NODES][MAX_NODES];
    int node_count;
} Graph;

typedef int (*GraphOperation)(Graph*, int, int);

void initialize_graph(Graph* graph, int node_count) {
    if (graph == NULL || node_count <= 0 || node_count > MAX_NODES) {
        return;
    }
    graph->node_count = node_count;
    for (int i = 0; i < node_count; i++) {
        for (int j = 0; j < node_count; j++) {
            graph->adjacency[i][j] = (i == j) ? 0 : INF;
        }
    }
}

int add_edge(Graph* graph, int from, int to, int weight) {
    if (graph == NULL || from < 0 || from >= graph->node_count || 
        to < 0 || to >= graph->node_count || weight < 0) {
        return 0;
    }
    graph->adjacency[from][to] = weight;
    return 1;
}

int dijkstra_shortest_path(Graph* graph, int start, int end) {
    if (graph == NULL || start < 0 || start >= graph->node_count || 
        end < 0 || end >= graph->node_count) {
        return INF;
    }
    
    int distances[MAX_NODES];
    int visited[MAX_NODES];
    
    for (int i = 0; i < graph->node_count; i++) {
        distances[i] = INF;
        visited[i] = 0;
    }
    distances[start] = 0;
    
    for (int count = 0; count < graph->node_count - 1; count++) {
        int min_distance = INF;
        int min_index = -1;
        
        for (int v = 0; v < graph->node_count; v++) {
            if (!visited[v] && distances[v] <= min_distance) {
                min_distance = distances[v];
                min_index = v;
            }
        }
        
        if (min_index == -1 || min_index == end) {
            break;
        }
        
        visited[min_index] = 1;
        
        for (int v = 0; v < graph->node_count; v++) {
            if (!visited[v] && graph->adjacency[min_index][v] != INF && 
                distances[min_index] != INF && 
                distances[min_index] + graph->adjacency[min_index][v] < distances[v]) {
                distances[v] = distances[min_index] + graph->adjacency[min_index][v];
            }
        }
    }
    
    return distances[end];
}

int has_path_exists(Graph* graph, int start, int end) {
    if (graph == NULL || start < 0 || start >= graph->node_count || 
        end < 0 || end >= graph->node_count) {
        return 0;
    }
    
    int visited[MAX_NODES] = {0};
    int stack[MAX_NODES];
    int top = -1;
    
    stack[++top] = start;
    visited[start] = 1;
    
    while (top >= 0) {
        int current = stack[top--];
        
        if (current == end) {
            return 1;
        }
        
        for (int i = 0; i < graph->node_count; i++) {
            if (graph->adjacency[current][i] != INF && !visited[i]) {
                visited[i] = 1;
                stack[++top] = i;
            }
        }
    }
    
    return 0;
}

int execute_graph_operation(GraphOperation operation, Graph* graph, int start, int end) {
    if (operation == NULL || graph == NULL) {
        return -1;
    }
    return operation(graph, start, end);
}

int main() {
    Graph graph;
    int node_count, edge_count;
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &node_count) != 1 || node_count < 1 || node_count > MAX_NODES) {
        printf("Invalid number of nodes\n");
        return 1;
    }
    
    initialize_graph(&graph, node_count);
    
    printf("Enter number of edges: ");
    if (scanf("%d", &edge_count) != 1 || edge_count < 0 || 
        edge_count > node_count * (node_count - 1)) {
        printf("Invalid number of edges\n");
        return 1;
    }
    
    printf("Enter edges as 'from to weight' (0-based indices):\n");
    for (int i = 0; i < edge_count; i++) {
        int from, to, weight;
        if (scanf("%d %d %d", &from, &to, &weight) != 3 || 
            from < 0 || from >= node_count || 
            to < 0 || to >= node_count || weight < 0) {
            printf("Invalid edge data\n");
            return 1;
        }
        if (!add_edge(&graph, from, to, weight)) {
            printf("Failed to add edge\n");
            return 1;
        }
    }
    
    int start, end;
    printf("Enter start and end nodes for analysis: ");
    if (scanf("%d %d", &start, &end) !=