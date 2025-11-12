//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

enum GraphType { UNDIRECTED, DIRECTED };
enum Algorithm { BFS, DFS, DIJKSTRA, TOPOLOGICAL };

struct Graph {
    int vertices;
    int **adjMatrix;
    enum GraphType type;
};

struct Graph* createGraph(int vertices, enum GraphType type) {
    if (vertices <= 0 || vertices > 100) return NULL;
    struct Graph* graph = malloc(sizeof(struct Graph));
    if (!graph) return NULL;
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
            for (int j = 0; j < i; j++) free(graph->adjMatrix[j]);
            free(graph->adjMatrix);
            free(graph);
            return NULL;
        }
    }
    return graph;
}

void addEdge(struct Graph* graph, int src, int dest, int weight) {
    if (!graph || src < 0 || src >= graph->vertices || dest < 0 || dest >= graph->vertices) return;
    graph->adjMatrix[src][dest] = weight;
    if (graph->type == UNDIRECTED) graph->adjMatrix[dest][src] = weight;
}

void freeGraph(struct Graph* graph) {
    if (!graph) return;
    for (int i = 0; i < graph->vertices; i++) free(graph->adjMatrix[i]);
    free(graph->adjMatrix);
    free(graph);
}

void bfs(struct Graph* graph, int start) {
    if (!graph || start < 0 || start >= graph->vertices) return;
    int *visited = calloc(graph->vertices, sizeof(int));
    if (!visited) return;
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
            if (graph->adjMatrix[current][i] && !visited[i]) {
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
        if (graph->adjMatrix[vertex][i] && !visited[i]) dfsUtil(graph, i, visited);
    }
}

void dfs(struct Graph* graph, int start) {
    if (!graph || start < 0 || start >= graph->vertices) return;
    int *visited = calloc(graph->vertices, sizeof(int));
    if (!visited) return;
    printf("DFS traversal: ");
    dfsUtil(graph, start, visited);
    printf("\n");
    free(visited);
}

void dijkstra(struct Graph* graph, int start) {
    if (!graph || start < 0 || start >= graph->vertices) return;
    int *dist = malloc(graph->vertices * sizeof(int));
    if (!dist) return;
    int *visited = calloc(graph->vertices, sizeof(int));
    if (!visited) {
        free(dist);
        return;
    }
    for (int i = 0; i < graph->vertices; i++) dist[i] = INT_MAX;
    dist[start] = 0;
    for (int count = 0; count < graph->vertices - 1; count++) {
        int min = INT_MAX, minIndex = -1;
        for (int v = 0; v < graph->vertices; v++) {
            if (!visited[v] && dist[v] <= min) {
                min = dist[v];
                minIndex = v;
            }
        }
        if (minIndex == -1) break;
        visited[minIndex] = 1;
        for (int v = 0; v < graph->vertices; v++) {
            if (!visited[v] && graph->adjMatrix[minIndex][v] && dist[minIndex] != INT_MAX && dist[minIndex] + graph->adjMatrix[minIndex][v] < dist[v]) {
                dist[v] = dist[minIndex] + graph->adjMatrix[minIndex][v];
            }
        }
    }
    printf("Dijkstra distances from %d: ", start);
    for (int i = 0; i < graph->vertices; i++) {
        if (dist[i] == INT_MAX) printf("INF ");
        else printf("%d ", dist[i]);
    }
    printf("\n");
    free(dist);
    free(visited);
}

void topologicalSortUtil(struct Graph* graph, int vertex, int* visited, int* stack, int* stackIndex) {
    visited[vertex] = 1;
    for (int i = 0; i < graph->vertices; i++) {
        if (graph->adjMatrix[vertex][i] && !visited[i])