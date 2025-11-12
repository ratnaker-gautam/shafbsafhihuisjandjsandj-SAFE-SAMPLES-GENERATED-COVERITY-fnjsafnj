//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: monolithic_main ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_VERTICES 100
#define INF INT_MAX

struct Graph {
    int vertices;
    int adjMatrix[MAX_VERTICES][MAX_VERTICES];
};

void initializeGraph(struct Graph* graph, int vertices) {
    if (vertices <= 0 || vertices > MAX_VERTICES) {
        fprintf(stderr, "Invalid number of vertices\n");
        exit(1);
    }
    
    graph->vertices = vertices;
    for (int i = 0; i < vertices; i++) {
        for (int j = 0; j < vertices; j++) {
            if (i == j) {
                graph->adjMatrix[i][j] = 0;
            } else {
                graph->adjMatrix[i][j] = INF;
            }
        }
    }
}

void addEdge(struct Graph* graph, int src, int dest, int weight) {
    if (src < 0 || src >= graph->vertices || dest < 0 || dest >= graph->vertices) {
        fprintf(stderr, "Invalid vertex index\n");
        exit(1);
    }
    if (weight < 0) {
        fprintf(stderr, "Negative weights not allowed\n");
        exit(1);
    }
    graph->adjMatrix[src][dest] = weight;
    graph->adjMatrix[dest][src] = weight;
}

int findMinDistance(int dist[], int visited[], int vertices) {
    int min = INF;
    int minIndex = -1;
    
    for (int v = 0; v < vertices; v++) {
        if (visited[v] == 0 && dist[v] <= min) {
            min = dist[v];
            minIndex = v;
        }
    }
    return minIndex;
}

void dijkstra(struct Graph* graph, int start) {
    int dist[MAX_VERTICES];
    int visited[MAX_VERTICES];
    
    for (int i = 0; i < graph->vertices; i++) {
        dist[i] = INF;
        visited[i] = 0;
    }
    dist[start] = 0;
    
    for (int count = 0; count < graph->vertices - 1; count++) {
        int u = findMinDistance(dist, visited, graph->vertices);
        if (u == -1) break;
        
        visited[u] = 1;
        
        for (int v = 0; v < graph->vertices; v++) {
            if (!visited[v] && graph->adjMatrix[u][v] != INF && dist[u] != INF) {
                long newDist = (long)dist[u] + graph->adjMatrix[u][v];
                if (newDist < dist[v] && newDist <= INT_MAX) {
                    dist[v] = (int)newDist;
                }
            }
        }
    }
    
    printf("Vertex\tDistance from source %d\n", start);
    for (int i = 0; i < graph->vertices; i++) {
        if (dist[i] == INF) {
            printf("%d\tINF\n", i);
        } else {
            printf("%d\t%d\n", i, dist[i]);
        }
    }
}

int main() {
    struct Graph graph;
    int vertices, edges;
    
    printf("Enter number of vertices (1-100): ");
    if (scanf("%d", &vertices) != 1 || vertices < 1 || vertices > MAX_VERTICES) {
        fprintf(stderr, "Invalid input for vertices\n");
        return 1;
    }
    
    initializeGraph(&graph, vertices);
    
    printf("Enter number of edges: ");
    if (scanf("%d", &edges) != 1 || edges < 0 || edges > vertices * (vertices - 1) / 2) {
        fprintf(stderr, "Invalid input for edges\n");
        return 1;
    }
    
    printf("Enter edges (source destination weight):\n");
    for (int i = 0; i < edges; i++) {
        int src, dest, weight;
        if (scanf("%d %d %d", &src, &dest, &weight) != 3) {
            fprintf(stderr, "Invalid edge input\n");
            return 1;
        }
        if (src < 0 || src >= vertices || dest < 0 || dest >= vertices) {
            fprintf(stderr, "Invalid vertex in edge\n");
            return 1;
        }
        if (weight < 0) {
            fprintf(stderr, "Negative weight not allowed\n");
            return 1;
        }
        addEdge(&graph, src, dest, weight);
    }
    
    int startVertex;
    printf("Enter starting vertex for Dijkstra: ");
    if (scanf("%d", &startVertex) != 1 || startVertex < 0 || startVertex >= vertices) {
        fprintf(stderr, "Invalid starting vertex\n");
        return 1;
    }
    
    dijkstra(&graph, startVertex);
    
    return 0;
}