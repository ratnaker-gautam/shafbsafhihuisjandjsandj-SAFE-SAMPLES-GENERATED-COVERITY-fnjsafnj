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
    if (!graph) return NULL;
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
    if (!graph) return;
    printf("Adjacency Matrix:\n");
    for (int i = 0; i < graph->vertices; i++) {
        for (int j = 0; j < graph->vertices; j++) {
            printf("%d ", graph->adjMatrix[i][j]);
        }
        printf("\n");
    }
}

enum Algorithm { BFS, DFS, DIJKSTRA };

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

void dfsUtil(struct Graph* graph, int v, int* visited) {
    visited[v] = 1;
    printf("%d ", v);
    for (int i = 0; i < graph->vertices; i++) {
        if (graph->adjMatrix[v][i] && !visited[i]) {
            dfsUtil(graph, i, visited);
        }
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

void dijkstra(struct Graph* graph, int src) {
    if (!graph || src < 0 || src >= graph->vertices) return;
    int* dist = malloc(graph->vertices * sizeof(int));
    if (!dist) return;
    int* visited = calloc(graph->vertices, sizeof(int));
    if (!visited) {
        free(dist);
        return;
    }
    for (int i = 0; i < graph->vertices; i++) {
        dist[i] = INT_MAX;
    }
    dist[src] = 0;
    for (int count = 0; count < graph->vertices - 1; count++) {
        int u = minDistance(dist, visited, graph->vertices);
        if (u == -1) break;
        visited[u] = 1;
        for (int v = 0; v < graph->vertices; v++) {
            if (!visited[v] && graph->adjMatrix[u][v] && dist[u] != INT_MAX && dist[u] + graph->adjMatrix[u][v] < dist[v]) {
                dist[v] = dist[u] + graph->adjMatrix[u][v];
            }
        }
    }
    printf("Dijkstra shortest paths from %d:\n", src);
    for (int i = 0; i < graph->vertices; i++) {
        if (dist[i] == INT_MAX) {
            printf("Vertex %