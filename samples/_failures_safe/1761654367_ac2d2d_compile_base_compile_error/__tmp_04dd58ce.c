//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

enum GraphType { UNDIRECTED, DIRECTED };
enum Algorithm { BFS, DFS, DIJKSTRA };

struct Graph {
    int vertices;
    int** matrix;
    enum GraphType type;
};

struct Graph* createGraph(int vertices, enum GraphType type) {
    if (vertices <= 0 || vertices > 100) return NULL;
    struct Graph* graph = malloc(sizeof(struct Graph));
    if (!graph) return NULL;
    graph->vertices = vertices;
    graph->type = type;
    graph->matrix = malloc(vertices * sizeof(int*));
    if (!graph->matrix) {
        free(graph);
        return NULL;
    }
    for (int i = 0; i < vertices; i++) {
        graph->matrix[i] = calloc(vertices, sizeof(int));
        if (!graph->matrix[i]) {
            for (int j = 0; j < i; j++) free(graph->matrix[j]);
            free(graph->matrix);
            free(graph);
            return NULL;
        }
    }
    return graph;
}

void freeGraph(struct Graph* graph) {
    if (!graph) return;
    for (int i = 0; i < graph->vertices; i++) free(graph->matrix[i]);
    free(graph->matrix);
    free(graph);
}

int addEdge(struct Graph* graph, int src, int dest, int weight) {
    if (!graph || src < 0 || src >= graph->vertices || dest < 0 || dest >= graph->vertices) return 0;
    if (weight < 0) return 0;
    graph->matrix[src][dest] = weight;
    if (graph->type == UNDIRECTED) graph->matrix[dest][src] = weight;
    return 1;
}

void bfs(struct Graph* graph, int start) {
    if (!graph || start < 0 || start >= graph->vertices) return;
    int* visited = calloc(graph->vertices, sizeof(int));
    if (!visited) return;
    int* queue = malloc(graph->vertices * sizeof(int));
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
            if (graph->matrix[current][i] && !visited[i]) {
                visited[i] = 1;
                if (rear < graph->vertices) queue[rear++] = i;
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
        if (graph->matrix[vertex][i] && !visited[i]) dfsUtil(graph, i, visited);
    }
}

void dfs(struct Graph* graph, int start) {
    if (!graph || start < 0 || start >= graph->vertices) return;
    int* visited = calloc(graph->vertices, sizeof(int));
    if (!visited) return;
    printf("DFS traversal: ");
    dfsUtil(graph, start, visited);
    printf("\n");
    free(visited);
}

int minDistance(int* dist, int* visited, int vertices) {
    int min = INT_MAX, min_index = -1;
    for (int v = 0; v < vertices; v++) {
        if (!visited[v] && dist[v] <= min) {
            min = dist[v];
            min_index = v;
        }
    }
    return min_index;
}

void dijkstra(struct Graph* graph, int start) {
    if (!graph || start < 0 || start >= graph->vertices) return;
    int* dist = malloc(graph->vertices * sizeof(int));
    if (!dist) return;
    int* visited = calloc(graph->vertices, sizeof(int));
    if (!visited) {
        free(dist);
        return;
    }
    for (int i = 0; i < graph->vertices; i++) dist[i] = INT_MAX;
    dist[start] = 0;
    for (int count = 0; count < graph->vertices - 1; count++) {
        int u = minDistance(dist, visited, graph->vertices);
        if (u == -1) break;
        visited[u] = 1;
        for (int v = 0; v < graph->vertices; v++) {
            if (!visited[v] && graph->matrix[u][v] && dist[u] != INT_MAX) {
                long new_dist = (long)dist[u] + graph->matrix[u][v];
                if (new_dist < dist[v] && new_dist <= INT_MAX) dist[v] = new_dist;
            }
        }
    }
    printf("Dijkstra distances from %d:\n", start);
    for (int i = 0; i < graph->vertices; i++) {
        if (dist[i] == INT_MAX) printf("Vertex %d: unreachable\n", i);
        else printf("Vertex %d: %d\n", i, dist[i]);
    }
    free(dist);
    free(visited);
}

void executeAlgorithm(struct Graph* graph, enum Algorithm algo, int start) {
    switch (algo