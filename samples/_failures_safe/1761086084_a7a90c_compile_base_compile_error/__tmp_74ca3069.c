//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: enum_switch ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

enum GraphType { UNDIRECTED, DIRECTED };

struct Graph {
    int vertices;
    int edges;
    int** adjMatrix;
    enum GraphType type;
};

struct Graph* createGraph(int vertices, enum GraphType type) {
    if (vertices <= 0 || vertices > 100) {
        return NULL;
    }
    
    struct Graph* graph = malloc(sizeof(struct Graph));
    if (graph == NULL) {
        return NULL;
    }
    
    graph->vertices = vertices;
    graph->edges = 0;
    graph->type = type;
    
    graph->adjMatrix = malloc(vertices * sizeof(int*));
    if (graph->adjMatrix == NULL) {
        free(graph);
        return NULL;
    }
    
    for (int i = 0; i < vertices; i++) {
        graph->adjMatrix[i] = calloc(vertices, sizeof(int));
        if (graph->adjMatrix[i] == NULL) {
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

void freeGraph(struct Graph* graph) {
    if (graph == NULL) return;
    
    for (int i = 0; i < graph->vertices; i++) {
        free(graph->adjMatrix[i]);
    }
    free(graph->adjMatrix);
    free(graph);
}

int addEdge(struct Graph* graph, int src, int dest, int weight) {
    if (graph == NULL) return 0;
    if (src < 0 || src >= graph->vertices) return 0;
    if (dest < 0 || dest >= graph->vertices) return 0;
    if (weight < 0) return 0;
    
    graph->adjMatrix[src][dest] = weight;
    graph->edges++;
    
    if (graph->type == UNDIRECTED) {
        graph->adjMatrix[dest][src] = weight;
    }
    
    return 1;
}

enum Algorithm { BFS, DFS, DIJKSTRA };

void bfs(struct Graph* graph, int start) {
    if (graph == NULL) return;
    if (start < 0 || start >= graph->vertices) return;
    
    int* visited = calloc(graph->vertices, sizeof(int));
    if (visited == NULL) return;
    
    int* queue = malloc(graph->vertices * sizeof(int));
    if (queue == NULL) {
        free(visited);
        return;
    }
    
    int front = 0, rear = 0;
    
    visited[start] = 1;
    queue[rear++] = start;
    
    printf("BFS traversal: ");
    
    while (front < rear) {
        int current = queue[front++];
        printf("%d ", current);
        
        for (int i = 0; i < graph->vertices; i++) {
            if (graph->adjMatrix[current][i] != 0 && !visited[i]) {
                visited[i] = 1;
                queue[rear++] = i;
            }
        }
    }
    printf("\n");
    
    free(visited);
    free(queue);
}

void dfsUtil(struct Graph* graph, int vertex, int* visited) {
    visited[vertex] = 1;
    printf("%d ", vertex);
    
    for (int i = 0; i < graph->vertices; i++) {
        if (graph->adjMatrix[vertex][i] != 0 && !visited[i]) {
            dfsUtil(graph, i, visited);
        }
    }
}

void dfs(struct Graph* graph, int start) {
    if (graph == NULL) return;
    if (start < 0 || start >= graph->vertices) return;
    
    int* visited = calloc(graph->vertices, sizeof(int));
    if (visited == NULL) return;
    
    printf("DFS traversal: ");
    dfsUtil(graph, start, visited);
    printf("\n");
    
    free(visited);
}

void dijkstra(struct Graph* graph, int start) {
    if (graph == NULL) return;
    if (start < 0 || start >= graph->vertices) return;
    
    int* dist = malloc(graph->vertices * sizeof(int));
    if (dist == NULL) return;
    
    int* visited = calloc(graph->vertices, sizeof(int));
    if (visited == NULL) {
        free(dist);
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
            if (!visited[v] && graph->adjMatrix[minIndex][v] != 0 && 
                dist[minIndex] != INT_MAX && 
                dist[minIndex] + graph->adjMatrix[minIndex][v] < dist[v]) {
                dist[v] = dist[minIndex] +