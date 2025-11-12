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

typedef void (*GraphOperation)(Graph*, int, int);

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

void add_edge(Graph* graph, int from, int to, int weight) {
    if (graph == NULL || from < 0 || from >= graph->node_count || 
        to < 0 || to >= graph->node_count || weight < 0) {
        return;
    }
    graph->adjacency[from][to] = weight;
}

void print_shortest_paths(Graph* graph, int source, int target) {
    if (graph == NULL || source < 0 || source >= graph->node_count || 
        target < 0 || target >= graph->node_count) {
        return;
    }
    
    int dist[MAX_NODES];
    int visited[MAX_NODES];
    
    for (int i = 0; i < graph->node_count; i++) {
        dist[i] = INF;
        visited[i] = 0;
    }
    dist[source] = 0;
    
    for (int count = 0; count < graph->node_count - 1; count++) {
        int min_dist = INF;
        int min_index = -1;
        
        for (int v = 0; v < graph->node_count; v++) {
            if (!visited[v] && dist[v] <= min_dist) {
                min_dist = dist[v];
                min_index = v;
            }
        }
        
        if (min_index == -1 || min_dist == INF) {
            break;
        }
        
        visited[min_index] = 1;
        
        for (int v = 0; v < graph->node_count; v++) {
            if (!visited[v] && graph->adjacency[min_index][v] != INF && 
                dist[min_index] != INF && 
                dist[min_index] + graph->adjacency[min_index][v] < dist[v]) {
                dist[v] = dist[min_index] + graph->adjacency[min_index][v];
            }
        }
    }
    
    printf("Shortest distance from node %d to node %d: ", source, target);
    if (dist[target] == INF) {
        printf("No path exists\n");
    } else {
        printf("%d\n", dist[target]);
    }
}

void print_graph_structure(Graph* graph, int source, int target) {
    if (graph == NULL || source < 0 || source >= graph->node_count || 
        target < 0 || target >= graph->node_count) {
        return;
    }
    
    printf("Graph structure (first 5 nodes):\n");
    int limit = (graph->node_count > 5) ? 5 : graph->node_count;
    for (int i = 0; i < limit; i++) {
        printf("Node %d connects to: ", i);
        int connections = 0;
        for (int j = 0; j < graph->node_count; j++) {
            if (graph->adjacency[i][j] != INF && graph->adjacency[i][j] != 0) {
                printf("%d(w:%d) ", j, graph->adjacency[i][j]);
                connections++;
            }
        }
        if (connections == 0) {
            printf("no outgoing edges");
        }
        printf("\n");
    }
}

void execute_graph_operation(Graph* graph, int source, int target, GraphOperation operation) {
    if (graph == NULL || operation == NULL) {
        return;
    }
    operation(graph, source, target);
}

int main() {
    Graph graph;
    int node_count, edge_count;
    
    printf("Enter number of nodes (1-100): ");
    if (scanf("%d", &node_count) != 1 || node_count < 1 || node_count > MAX_NODES) {
        printf("Invalid number of nodes\n");
        return 1;
    }
    
    initialize_graph(&graph, node_count);
    
    printf("Enter number of edges: ");
    if (scanf("%d", &edge_count) != 1 || edge_count < 0 || edge_count > node_count * node_count) {
        printf("Invalid number of edges\n");
        return 1;
    }
    
    printf("Enter edges as 'from to weight' (0-indexed):\n");
    for (int i = 0; i < edge_count; i++) {
        int from, to, weight;
        if (scanf("%d %d %d", &from, &to, &weight) != 3) {
            printf("Invalid edge input\n");
            return 1;
        }
        if (from < 0 || from >= node_count || to < 0 || to >= node_count || weight < 0) {
            printf("Invalid edge parameters\n");
            return 1;
        }
        add_edge(&graph, from