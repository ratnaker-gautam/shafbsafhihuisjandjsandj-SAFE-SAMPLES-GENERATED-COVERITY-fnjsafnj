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
    union {
        int **matrix;
        struct Node **list;
    } data;
};

struct Node {
    int vertex;
    int weight;
    struct Node *next;
};

struct Graph *createGraph(enum GraphType type, int vertices) {
    if (vertices <= 0 || vertices > 100) return NULL;
    struct Graph *graph = malloc(sizeof(struct Graph));
    if (!graph) return NULL;
    graph->type = type;
    graph->vertices = vertices;
    if (type == ADJ_MATRIX) {
        graph->data.matrix = malloc(vertices * sizeof(int *));
        if (!graph->data.matrix) { free(graph); return NULL; }
        for (int i = 0; i < vertices; i++) {
            graph->data.matrix[i] = calloc(vertices, sizeof(int));
            if (!graph->data.matrix[i]) {
                for (int j = 0; j < i; j++) free(graph->data.matrix[j]);
                free(graph->data.matrix);
                free(graph);
                return NULL;
            }
        }
    } else {
        graph->data.list = calloc(vertices, sizeof(struct Node *));
        if (!graph->data.list) { free(graph); return NULL; }
    }
    return graph;
}

void addEdge(struct Graph *graph, int src, int dest, int weight) {
    if (!graph || src < 0 || src >= graph->vertices || dest < 0 || dest >= graph->vertices) return;
    if (weight < 0 || weight > 1000) return;
    switch (graph->type) {
        case ADJ_MATRIX:
            graph->data.matrix[src][dest] = weight;
            graph->data.matrix[dest][src] = weight;
            break;
        case ADJ_LIST: {
            struct Node *newNode = malloc(sizeof(struct Node));
            if (!newNode) return;
            newNode->vertex = dest;
            newNode->weight = weight;
            newNode->next = graph->data.list[src];
            graph->data.list[src] = newNode;
            newNode = malloc(sizeof(struct Node));
            if (!newNode) return;
            newNode->vertex = src;
            newNode->weight = weight;
            newNode->next = graph->data.list[dest];
            graph->data.list[dest] = newNode;
            break;
        }
    }
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
    printf("BFS: ");
    while (front < rear) {
        int current = queue[front++];
        printf("%d ", current);
        switch (graph->type) {
            case ADJ_MATRIX:
                for (int i = 0; i < graph->vertices; i++) {
                    if (graph->data.matrix[current][i] && !visited[i]) {
                        visited[i] = 1;
                        queue[rear++] = i;
                    }
                }
                break;
            case ADJ_LIST: {
                struct Node *temp = graph->data.list[current];
                while (temp) {
                    if (!visited[temp->vertex]) {
                        visited[temp->vertex] = 1;
                        queue[rear++] = temp->vertex;
                    }
                    temp = temp->next;
                }
                break;
            }
        }
    }
    printf("\n");
    free(visited);
    free(queue);
}

void dfsUtil(struct Graph *graph, int vertex, int *visited) {
    visited[vertex] = 1;
    printf("%d ", vertex);
    switch (graph->type) {
        case ADJ_MATRIX:
            for (int i = 0; i < graph->vertices; i++) {
                if (graph->data.matrix[vertex][i] && !visited[i]) {
                    dfsUtil(graph, i, visited);
                }
            }
            break;
        case ADJ_LIST: {
            struct Node *temp = graph->data.list[vertex];
            while (temp) {
                if (!visited[temp->vertex]) {
                    dfsUtil(graph, temp->vertex, visited);
                }
                temp = temp->next;
            }
            break;
        }
    }
}

void dfs(struct Graph *graph, int start) {
    if (!graph || start < 0 || start >= graph->vertices) return;
    int *visited = calloc(graph->vertices, sizeof(int));
    if (!visited) return;
    printf("DFS: ");
    dfsUtil(graph, start, visited);
    printf("\n");
    free(visited);
}

void dijkstra(struct Graph *graph, int start) {
    if (!graph || start < 0 || start >= graph->vertices) return;
    int *dist = malloc(graph->vertices * sizeof(int));
    if (!dist) return;
    int *visited = calloc(graph->vertices, sizeof(int));
    if (!visited) { free(dist); return; }
    for (