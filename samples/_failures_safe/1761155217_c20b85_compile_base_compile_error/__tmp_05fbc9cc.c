//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

enum GraphType { ADJ_MATRIX, ADJ_LIST };
enum Algorithm { BFS, DFS, DIJKSTRA };

struct Graph {
    enum GraphType type;
    int vertices;
    int **adjMatrix;
    int **adjList;
    int *listSizes;
};

struct Graph* createGraph(enum GraphType type, int vertices) {
    if (vertices <= 0 || vertices > 100) return NULL;
    
    struct Graph *graph = malloc(sizeof(struct Graph));
    if (!graph) return NULL;
    
    graph->type = type;
    graph->vertices = vertices;
    graph->adjMatrix = NULL;
    graph->adjList = NULL;
    graph->listSizes = NULL;
    
    if (type == ADJ_MATRIX) {
        graph->adjMatrix = malloc(vertices * sizeof(int*));
        if (!graph->adjMatrix) { free(graph); return NULL; }
        
        for (int i = 0; i < vertices; i++) {
            graph->adjMatrix[i] = calloc(vertices, sizeof(int));
            if (!graph->adjMatrix[i]) {
                for (int j = 0; j < i; j++) free(graph->adjMatrix[j]);
                free(graph->adjMatrix);
                free(graph);
                return NULL;
            }
        }
    } else {
        graph->adjList = malloc(vertices * sizeof(int*));
        graph->listSizes = calloc(vertices, sizeof(int));
        if (!graph->adjList || !graph->listSizes) {
            free(graph->adjList);
            free(graph->listSizes);
            free(graph);
            return NULL;
        }
        
        for (int i = 0; i < vertices; i++) {
            graph->adjList[i] = malloc(vertices * sizeof(int));
            if (!graph->adjList[i]) {
                for (int j = 0; j < i; j++) free(graph->adjList[j]);
                free(graph->adjList);
                free(graph->listSizes);
                free(graph);
                return NULL;
            }
        }
    }
    
    return graph;
}

void freeGraph(struct Graph *graph) {
    if (!graph) return;
    
    if (graph->type == ADJ_MATRIX && graph->adjMatrix) {
        for (int i = 0; i < graph->vertices; i++) free(graph->adjMatrix[i]);
        free(graph->adjMatrix);
    } else if (graph->adjList) {
        for (int i = 0; i < graph->vertices; i++) free(graph->adjList[i]);
        free(graph->adjList);
        free(graph->listSizes);
    }
    free(graph);
}

int addEdge(struct Graph *graph, int src, int dest, int weight) {
    if (!graph || src < 0 || src >= graph->vertices || dest < 0 || dest >= graph->vertices) return 0;
    if (weight < 0 || weight > 1000) return 0;
    
    switch (graph->type) {
        case ADJ_MATRIX:
            graph->adjMatrix[src][dest] = weight;
            graph->adjMatrix[dest][src] = weight;
            break;
        case ADJ_LIST:
            if (graph->listSizes[src] < graph->vertices) {
                graph->adjList[src][graph->listSizes[src]++] = dest;
            }
            if (graph->listSizes[dest] < graph->vertices) {
                graph->adjList[dest][graph->listSizes[dest]++] = src;
            }
            break;
    }
    return 1;
}

void dfsUtil(struct Graph *graph, int vertex, int *visited) {
    visited[vertex] = 1;
    printf("%d ", vertex);
    
    switch (graph->type) {
        case ADJ_MATRIX:
            for (int i = 0; i < graph->vertices; i++) {
                if (graph->adjMatrix[vertex][i] && !visited[i]) {
                    dfsUtil(graph, i, visited);
                }
            }
            break;
        case ADJ_LIST:
            for (int i = 0; i < graph->listSizes[vertex]; i++) {
                int neighbor = graph->adjList[vertex][i];
                if (!visited[neighbor]) {
                    dfsUtil(graph, neighbor, visited);
                }
            }
            break;
    }
}

void dfs(struct Graph *graph, int start) {
    if (!graph || start < 0 || start >= graph->vertices) return;
    
    int *visited = calloc(graph->vertices, sizeof(int));
    if (!visited) return;
    
    printf("DFS traversal: ");
    dfsUtil(graph, start, visited);
    printf("\n");
    
    free(visited);
}

void bfs(struct Graph *graph, int start) {
    if (!graph || start < 0 || start >= graph->vertices) return;
    
    int *visited = calloc(graph->vertices, sizeof(int));
    if (!visited) return;
    
    int *queue = malloc(graph->vertices * sizeof(int));
    if (!queue) { free(visited); return; }
    
    int front = 0, rear = 0;
    visited[start] = 1;
    queue[rear++] = start;
    
    printf("BFS traversal: ");
    while (front < rear) {
        int current = queue[front++];
        printf("%d ", current);