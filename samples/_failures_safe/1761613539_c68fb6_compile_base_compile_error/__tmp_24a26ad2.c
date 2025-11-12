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
    
    struct Graph* graph = malloc(sizeof(struct Graph));
    if (!graph) {
        return NULL;
    }
    
    graph->vertices = vertices;
    graph->type = type;
    
    graph->adjMatrix = malloc(vertices * sizeof(int*));
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

void addEdge(struct Graph* graph, int src, int dest, int weight) {
    if (!graph || src < 0 || src >= graph->vertices || dest < 0 || dest >= graph->vertices) {
        return;
    }
    
    graph->adjMatrix[src][dest] = weight;
    if (graph->type == UNDIRECTED) {
        graph->adjMatrix[dest][src] = weight;
    }
}

void bfs(struct Graph* graph, int start) {
    if (!graph || start < 0 || start >= graph->vertices) {
        return;
    }
    
    int *visited = calloc(graph->vertices, sizeof(int));
    if (!visited) {
        return;
    }
    
    int *queue = malloc(graph->vertices * sizeof(int));
    if (!queue) {
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
                if (rear < graph->vertices) {
                    queue[rear++] = i;
                }
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
    if (!graph || start < 0 || start >= graph->vertices) {
        return;
    }
    
    int *visited = calloc(graph->vertices, sizeof(int));
    if (!visited) {
        return;
    }
    
    printf("DFS traversal: ");
    dfsUtil(graph, start, visited);
    printf("\n");
    
    free(visited);
}

void dijkstra(struct Graph* graph, int start) {
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
        int minVertex = -1;
        
        for (int v = 0; v < graph->vertices; v++) {
            if (!visited[v] && dist[v] <= minDist) {
                minDist = dist[v];
                minVertex = v;
            }
        }
        
        if (minVertex == -1) {
            break;
        }
        
        visited[minVertex] = 1;
        
        for (int v = 0; v < graph->vertices; v++) {
            if (!visited[v] && graph->adjMatrix[minVertex][v] != 0 && 
                dist[minVertex] != INT_MAX && 
                dist[minVertex] + graph->adjMatrix[minVertex][v] < dist[v]) {
                dist[v] = dist[minVertex] + graph->adjMatrix[minVertex][v];
            }
        }
    }
    
    printf("Dijkstra distances from vertex %d:\n", start);
    for (int i = 0; i < graph->vertices; i++) {
        if (dist[i] == INT_MAX) {
            printf("Vertex %d: unreachable\n", i);
        } else {
            printf("Vertex %d: %d\n", i, dist[i]);
        }
    }
    
    free(dist);
    free(visited);
}

void