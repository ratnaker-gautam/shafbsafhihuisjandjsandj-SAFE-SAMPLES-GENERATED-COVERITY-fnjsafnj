//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: graph_algorithms
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
    if (!graph) {
        return NULL;
    }
    graph->vertices = vertices;
    graph->edges = 0;
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
    if (!graph || src < 0 || src >= graph->vertices || dest < 0 || dest >= graph->vertices || weight < 0) {
        return;
    }
    graph->adjMatrix[src][dest] = weight;
    graph->edges++;
    if (graph->type == UNDIRECTED) {
        graph->adjMatrix[dest][src] = weight;
    }
}

void printGraph(struct Graph* graph) {
    if (!graph) {
        return;
    }
    printf("Adjacency Matrix:\n");
    for (int i = 0; i < graph->vertices; i++) {
        for (int j = 0; j < graph->vertices; j++) {
            printf("%d ", graph->adjMatrix[i][j]);
        }
        printf("\n");
    }
}

int minDistance(int dist[], int visited[], int vertices) {
    int min = INT_MAX, min_index = -1;
    for (int v = 0; v < vertices; v++) {
        if (!visited[v] && dist[v] <= min) {
            min = dist[v];
            min_index = v;
        }
    }
    return min_index;
}

void dijkstra(struct Graph* graph, int src) {
    if (!graph || src < 0 || src >= graph->vertices) {
        return;
    }
    int vertices = graph->vertices;
    int dist[vertices];
    int visited[vertices];
    for (int i = 0; i < vertices; i++) {
        dist[i] = INT_MAX;
        visited[i] = 0;
    }
    dist[src] = 0;
    for (int count = 0; count < vertices - 1; count++) {
        int u = minDistance(dist, visited, vertices);
        if (u == -1) {
            break;
        }
        visited[u] = 1;
        for (int v = 0; v < vertices; v++) {
            if (!visited[v] && graph->adjMatrix[u][v] && dist[u] != INT_MAX && dist[u] + graph->adjMatrix[u][v] < dist[v]) {
                dist[v] = dist[u] + graph->adjMatrix[u][v];
            }
        }
    }
    printf("Dijkstra from vertex %d:\n", src);
    for (int i = 0; i < vertices; i++) {
        if (dist[i] == INT_MAX) {
            printf("Vertex %d: unreachable\n", i);
        } else {
            printf("Vertex %d: distance %d\n", i, dist[i]);
        }
    }
}

void dfsUtil(struct Graph* graph, int vertex, int visited[]) {
    if (!graph || vertex < 0 || vertex >= graph->vertices || visited[vertex]) {
        return;
    }
    visited[vertex] = 1;
    printf("%d ", vertex);
    for (int i = 0; i < graph->vertices; i++) {
        if (graph->adjMatrix[vertex][i] && !visited[i]) {
            dfsUtil(graph, i, visited);
        }
    }
}

void dfs(struct Graph* graph, int start) {
    if (!graph || start < 0 || start >= graph->vertices) {
        return;
    }
    int visited[graph->vertices];
    for (int i = 0; i < graph->vertices; i++) {
        visited[i] = 0;
    }
    printf("DFS from vertex %d: ", start);
    dfsUtil(graph, start, visited);
    printf("\n");
}

enum Algorithm { DIJKSTRA, DFS, PRINT };

void executeAlgorithm(struct Graph* graph, enum Algorithm algo, int start) {
    switch (algo) {
        case DIJKSTRA:
            dijkstra(graph, start);
            break;
        case DFS:
            dfs(graph, start);
            break;
        case PRINT:
            printGraph(graph);
            break;
        default:
            printf("Invalid algorithm\n");
    }
}

void freeGraph(struct Graph* graph) {
    if (!graph) {
        return;
    }
    if (graph->adjMatrix) {
        for (int i = 0; i < graph->vertices; i++) {
            free(graph->adjMatrix[i]);
        }
        free(graph->adjMatrix);
    }
    free(graph);
}