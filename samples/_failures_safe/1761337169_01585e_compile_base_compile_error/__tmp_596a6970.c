//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_VERTICES 100
#define MAX_EDGES 1000

struct Graph {
    int vertices;
    int edges;
    int adjacency[MAX_VERTICES][MAX_VERTICES];
};

struct Edge {
    int src;
    int dest;
    int weight;
};

void initializeGraph(struct Graph* graph, int vertices) {
    if (vertices <= 0 || vertices > MAX_VERTICES) {
        vertices = MAX_VERTICES;
    }
    graph->vertices = vertices;
    graph->edges = 0;
    for (int i = 0; i < vertices; i++) {
        for (int j = 0; j < vertices; j++) {
            graph->adjacency[i][j] = 0;
        }
    }
}

int addEdge(struct Graph* graph, int src, int dest, int weight) {
    if (graph == NULL) return 0;
    if (src < 0 || src >= graph->vertices) return 0;
    if (dest < 0 || dest >= graph->vertices) return 0;
    if (graph->edges >= MAX_EDGES) return 0;
    
    graph->adjacency[src][dest] = weight;
    graph->adjacency[dest][src] = weight;
    graph->edges++;
    return 1;
}

void printGraph(struct Graph* graph) {
    if (graph == NULL) return;
    
    printf("Graph with %d vertices and %d edges:\n", graph->vertices, graph->edges);
    for (int i = 0; i < graph->vertices; i++) {
        printf("Vertex %d: ", i);
        int hasNeighbors = 0;
        for (int j = 0; j < graph->vertices; j++) {
            if (graph->adjacency[i][j] != 0) {
                printf("%d(%d) ", j, graph->adjacency[i][j]);
                hasNeighbors = 1;
            }
        }
        if (!hasNeighbors) {
            printf("No neighbors");
        }
        printf("\n");
    }
}

int findMinDistance(int distances[], int visited[], int vertices) {
    int minDistance = INT_MAX;
    int minIndex = -1;
    
    for (int v = 0; v < vertices; v++) {
        if (!visited[v] && distances[v] <= minDistance) {
            minDistance = distances[v];
            minIndex = v;
        }
    }
    return minIndex;
}

void dijkstra(struct Graph* graph, int startVertex) {
    if (graph == NULL) return;
    if (startVertex < 0 || startVertex >= graph->vertices) return;
    
    int distances[MAX_VERTICES];
    int visited[MAX_VERTICES];
    
    for (int i = 0; i < graph->vertices; i++) {
        distances[i] = INT_MAX;
        visited[i] = 0;
    }
    
    distances[startVertex] = 0;
    
    for (int count = 0; count < graph->vertices - 1; count++) {
        int u = findMinDistance(distances, visited, graph->vertices);
        if (u == -1) break;
        
        visited[u] = 1;
        
        for (int v = 0; v < graph->vertices; v++) {
            if (!visited[v] && graph->adjacency[u][v] != 0 && 
                distances[u] != INT_MAX) {
                int newDistance = distances[u] + graph->adjacency[u][v];
                if (newDistance < distances[v]) {
                    distances[v] = newDistance;
                }
            }
        }
    }
    
    printf("Shortest distances from vertex %d:\n", startVertex);
    for (int i = 0; i < graph->vertices; i++) {
        if (distances[i] == INT_MAX) {
            printf("Vertex %d: Unreachable\n", i);
        } else {
            printf("Vertex %d: %d\n", i, distances[i]);
        }
    }
}

int main() {
    struct Graph graph;
    int vertices, edges;
    
    printf("Enter number of vertices (1-%d): ", MAX_VERTICES);
    if (scanf("%d", &vertices) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (vertices <= 0 || vertices > MAX_VERTICES) {
        printf("Invalid number of vertices\n");
        return 1;
    }
    
    initializeGraph(&graph, vertices);
    
    printf("Enter number of edges: ");
    if (scanf("%d", &edges) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (edges < 0 || edges > MAX_EDGES) {
        printf("Invalid number of edges\n");
        return 1;
    }
    
    printf("Enter edges as: source destination weight\n");
    for (int i = 0; i < edges; i++) {
        int src, dest, weight;
        printf("Edge %d: ", i + 1);
        if (scanf("%d %d %d", &src, &dest, &weight) != 3) {
            printf("Invalid edge input\n");
            return 1;
        }
        
        if (!addEdge(&graph, src, dest, weight)) {
            printf("Failed to add edge\n");
            return 1;
        }
    }
    
    printf("\n");
    printGraph(&