//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 100
#define INF INT_MAX

struct Graph {
    int nodes;
    int matrix[MAX_NODES][MAX_NODES];
};

struct Graph* createGraph(int nodes) {
    if (nodes <= 0 || nodes > MAX_NODES) {
        return NULL;
    }
    struct Graph* graph = malloc(sizeof(struct Graph));
    if (graph == NULL) {
        return NULL;
    }
    graph->nodes = nodes;
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            graph->matrix[i][j] = (i == j) ? 0 : INF;
        }
    }
    return graph;
}

int addEdge(struct Graph* graph, int src, int dest, int weight) {
    if (graph == NULL) return 0;
    if (src < 0 || src >= graph->nodes) return 0;
    if (dest < 0 || dest >= graph->nodes) return 0;
    if (weight < 0) return 0;
    graph->matrix[src][dest] = weight;
    graph->matrix[dest][src] = weight;
    return 1;
}

void dijkstra(struct Graph* graph, int start) {
    if (graph == NULL) return;
    if (start < 0 || start >= graph->nodes) return;
    
    int dist[MAX_NODES];
    int visited[MAX_NODES];
    
    for (int i = 0; i < graph->nodes; i++) {
        dist[i] = INF;
        visited[i] = 0;
    }
    dist[start] = 0;
    
    for (int count = 0; count < graph->nodes - 1; count++) {
        int min = INF;
        int min_index = -1;
        
        for (int v = 0; v < graph->nodes; v++) {
            if (!visited[v] && dist[v] <= min) {
                min = dist[v];
                min_index = v;
            }
        }
        
        if (min_index == -1) break;
        visited[min_index] = 1;
        
        for (int v = 0; v < graph->nodes; v++) {
            if (!visited[v] && graph->matrix[min_index][v] != INF && 
                dist[min_index] != INF && 
                dist[min_index] + graph->matrix[min_index][v] < dist[v]) {
                dist[v] = dist[min_index] + graph->matrix[min_index][v];
            }
        }
    }
    
    printf("Shortest distances from node %d:\n", start);
    for (int i = 0; i < graph->nodes; i++) {
        if (dist[i] == INF) {
            printf("Node %d: unreachable\n", i);
        } else {
            printf("Node %d: %d\n", i, dist[i]);
        }
    }
}

int main() {
    struct Graph* graph = createGraph(6);
    if (graph == NULL) {
        printf("Failed to create graph\n");
        return 1;
    }
    
    addEdge(graph, 0, 1, 4);
    addEdge(graph, 0, 2, 2);
    addEdge(graph, 1, 2, 1);
    addEdge(graph, 1, 3, 5);
    addEdge(graph, 2, 3, 8);
    addEdge(graph, 2, 4, 10);
    addEdge(graph, 3, 4, 2);
    addEdge(graph, 3, 5, 6);
    addEdge(graph, 4, 5, 3);
    
    printf("Graph with 6 nodes created\n");
    printf("Running Dijkstra's algorithm from node 0:\n");
    dijkstra(graph, 0);
    
    free(graph);
    return 0;
}