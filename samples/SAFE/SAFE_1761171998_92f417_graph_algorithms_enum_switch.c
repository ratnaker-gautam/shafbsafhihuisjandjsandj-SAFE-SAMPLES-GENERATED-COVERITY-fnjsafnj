//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

enum GraphType { DIRECTED, UNDIRECTED };

struct Graph {
    int vertices;
    int edges;
    int **adjMatrix;
    enum GraphType type;
};

struct Graph* createGraph(int vertices, enum GraphType type) {
    if (vertices <= 0 || vertices > 100) {
        return NULL;
    }
    
    struct Graph *graph = malloc(sizeof(struct Graph));
    if (!graph) return NULL;
    
    graph->vertices = vertices;
    graph->edges = 0;
    graph->type = type;
    
    graph->adjMatrix = malloc(vertices * sizeof(int *));
    if (!graph->adjMatrix) {
        free(graph);
        return NULL;
    }
    
    for (int i = 0; i < vertices; i++) {
        graph->adjMatrix[i] = calloc(vertices, sizeof(int));
        if (!graph->adjMatrix[i]) {
            for (int j = 0; j < i; j++) {
                free(graph->adjMatrix[j]);
            }
            free(graph->adjMatrix);
            free(graph);
            return NULL;
        }
    }
    
    return graph;
}

void addEdge(struct Graph *graph, int src, int dest, int weight) {
    if (!graph || src < 0 || src >= graph->vertices || dest < 0 || dest >= graph->vertices) {
        return;
    }
    
    if (weight < 0 || weight > 1000) {
        return;
    }
    
    graph->adjMatrix[src][dest] = weight;
    graph->edges++;
    
    if (graph->type == UNDIRECTED) {
        graph->adjMatrix[dest][src] = weight;
    }
}

void dijkstra(struct Graph *graph, int start) {
    if (!graph || start < 0 || start >= graph->vertices) {
        return;
    }
    
    int *dist = malloc(graph->vertices * sizeof(int));
    int *visited = calloc(graph->vertices, sizeof(int));
    
    if (!dist || !visited) {
        free(dist);
        free(visited);
        return;
    }
    
    for (int i = 0; i < graph->vertices; i++) {
        dist[i] = INT_MAX;
    }
    dist[start] = 0;
    
    for (int count = 0; count < graph->vertices - 1; count++) {
        int minDist = INT_MAX;
        int minIndex = -1;
        
        for (int v = 0; v < graph->vertices; v++) {
            if (!visited[v] && dist[v] <= minDist) {
                minDist = dist[v];
                minIndex = v;
            }
        }
        
        if (minIndex == -1) break;
        
        visited[minIndex] = 1;
        
        for (int v = 0; v < graph->vertices; v++) {
            if (!visited[v] && graph->adjMatrix[minIndex][v] && 
                dist[minIndex] != INT_MAX && 
                dist[minIndex] + graph->adjMatrix[minIndex][v] < dist[v]) {
                dist[v] = dist[minIndex] + graph->adjMatrix[minIndex][v];
            }
        }
    }
    
    printf("Vertex\tDistance from %d\n", start);
    for (int i = 0; i < graph->vertices; i++) {
        if (dist[i] == INT_MAX) {
            printf("%d\tINF\n", i);
        } else {
            printf("%d\t%d\n", i, dist[i]);
        }
    }
    
    free(dist);
    free(visited);
}

void freeGraph(struct Graph *graph) {
    if (!graph) return;
    
    for (int i = 0; i < graph->vertices; i++) {
        free(graph->adjMatrix[i]);
    }
    free(graph->adjMatrix);
    free(graph);
}

enum Algorithm { DIJKSTRA, BFS, DFS };

void executeAlgorithm(struct Graph *graph, enum Algorithm algo, int start) {
    if (!graph) return;
    
    switch (algo) {
        case DIJKSTRA:
            if (start >= 0 && start < graph->vertices) {
                dijkstra(graph, start);
            }
            break;
        case BFS:
            printf("BFS algorithm selected (not implemented)\n");
            break;
        case DFS:
            printf("DFS algorithm selected (not implemented)\n");
            break;
        default:
            printf("Unknown algorithm\n");
    }
}

int main() {
    struct Graph *graph = createGraph(5, UNDIRECTED);
    if (!graph) {
        printf("Failed to create graph\n");
        return 1;
    }
    
    addEdge(graph, 0, 1, 4);
    addEdge(graph, 0, 2, 1);
    addEdge(graph, 1, 2, 2);
    addEdge(graph, 1, 3, 5);
    addEdge(graph, 2, 3, 8);
    addEdge(graph, 2, 4, 10);
    addEdge(graph, 3, 4, 2);
    
    printf("Graph created with %d vertices and %d edges\n", graph->vertices, graph->edges);
    
    enum Algorithm selected = DIJKSTRA;
    int startVertex = 0;
    
    printf("\nRunning algorithm on graph:\n");
    executeAlgorithm(graph, selected, startVertex);
    
    freeGraph(graph);
    return 0;
}