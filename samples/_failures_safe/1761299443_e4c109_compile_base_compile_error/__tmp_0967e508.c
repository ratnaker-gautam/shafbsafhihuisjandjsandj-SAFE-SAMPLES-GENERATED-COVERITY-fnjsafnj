//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

enum GraphType { DIRECTED, UNDIRECTED };
enum Algorithm { BFS, DFS, DIJKSTRA };

struct Graph {
    int vertices;
    int **adjMatrix;
    enum GraphType type;
};

struct Graph* createGraph(int vertices, enum GraphType type) {
    if (vertices <= 0 || vertices > 100) {
        return NULL;
    }
    
    struct Graph* graph = (struct Graph*)malloc(sizeof(struct Graph));
    if (graph == NULL) {
        return NULL;
    }
    
    graph->vertices = vertices;
    graph->type = type;
    
    graph->adjMatrix = (int**)malloc(vertices * sizeof(int*));
    if (graph->adjMatrix == NULL) {
        free(graph);
        return NULL;
    }
    
    for (int i = 0; i < vertices; i++) {
        graph->adjMatrix[i] = (int*)calloc(vertices, sizeof(int));
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

void addEdge(struct Graph* graph, int src, int dest, int weight) {
    if (graph == NULL || src < 0 || src >= graph->vertices || 
        dest < 0 || dest >= graph->vertices || weight < 0) {
        return;
    }
    
    graph->adjMatrix[src][dest] = weight;
    if (graph->type == UNDIRECTED) {
        graph->adjMatrix[dest][src] = weight;
    }
}

void bfs(struct Graph* graph, int start) {
    if (graph == NULL || start < 0 || start >= graph->vertices) {
        return;
    }
    
    int *visited = (int*)calloc(graph->vertices, sizeof(int));
    if (visited == NULL) {
        return;
    }
    
    int *queue = (int*)malloc(graph->vertices * sizeof(int));
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
    if (graph == NULL || start < 0 || start >= graph->vertices) {
        return;
    }
    
    int *visited = (int*)calloc(graph->vertices, sizeof(int));
    if (visited == NULL) {
        return;
    }
    
    printf("DFS traversal: ");
    dfsUtil(graph, start, visited);
    printf("\n");
    
    free(visited);
}

void dijkstra(struct Graph* graph, int start) {
    if (graph == NULL || start < 0 || start >= graph->vertices) {
        return;
    }
    
    int *dist = (int*)malloc(graph->vertices * sizeof(int));
    int *visited = (int*)calloc(graph->vertices, sizeof(int));
    
    if (dist == NULL || visited == NULL) {
        free(dist);
        free(visited);
        return;
    }
    
    for (int i = 0; i < graph->vertices; i++) {
        dist[i] = INT_MAX;
    }
    dist[start] = 0;
    
    for (int count = 0; count < graph->vertices - 1; count++) {
        int min = INT_MAX, min_index = -1;
        
        for (int v = 0; v < graph->vertices; v++) {
            if (!visited[v] && dist[v] <= min) {
                min = dist[v];
                min_index = v;
            }
        }
        
        if (min_index == -1) break;
        
        visited[min_index] = 1;
        
        for (int v = 0; v < graph->vertices; v++) {
            if (!visited[v] && graph->adjMatrix[min_index][v] != 0 && 
                dist[min_index] != INT_MAX && 
                dist[min_index] + graph->adjMatrix[min_index][v] < dist[v]) {
                dist[v] = dist[min_index] + graph->adjMatrix[min_index][v];
            }
        }
    }
    
    printf("Dijkstra distances from vertex %d:\n", start);
    for (int i = 0; i < graph->vertices; i++) {
        if (dist[i] == INT_MAX) {
            printf("Vertex %d: INF\n", i);
        }