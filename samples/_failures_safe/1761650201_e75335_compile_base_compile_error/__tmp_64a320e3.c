//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

enum GraphType {
    ADJ_MATRIX,
    ADJ_LIST
};

enum Operation {
    BFS,
    DFS,
    SHORTEST_PATH,
    CYCLE_CHECK
};

struct Graph {
    enum GraphType type;
    int vertices;
    int **adjMatrix;
    int **adjList;
    int *listSizes;
};

struct Queue {
    int front, rear, size;
    unsigned capacity;
    int *array;
};

struct Queue *createQueue(unsigned capacity) {
    if (capacity == 0 || capacity > 1000) return NULL;
    struct Queue *queue = (struct Queue *)malloc(sizeof(struct Queue));
    if (!queue) return NULL;
    queue->capacity = capacity;
    queue->front = queue->size = 0;
    queue->rear = capacity - 1;
    queue->array = (int *)malloc(queue->capacity * sizeof(int));
    if (!queue->array) {
        free(queue);
        return NULL;
    }
    return queue;
}

int isQueueFull(struct Queue *queue) {
    if (!queue) return 1;
    return (queue->size == queue->capacity);
}

int isQueueEmpty(struct Queue *queue) {
    if (!queue) return 1;
    return (queue->size == 0);
}

void enqueue(struct Queue *queue, int item) {
    if (!queue || isQueueFull(queue)) return;
    queue->rear = (queue->rear + 1) % queue->capacity;
    queue->array[queue->rear] = item;
    queue->size = queue->size + 1;
}

int dequeue(struct Queue *queue) {
    if (!queue || isQueueEmpty(queue)) return INT_MIN;
    int item = queue->array[queue->front];
    queue->front = (queue->front + 1) % queue->capacity;
    queue->size = queue->size - 1;
    return item;
}

void freeQueue(struct Queue *queue) {
    if (!queue) return;
    free(queue->array);
    free(queue);
}

void freeGraph(struct Graph *graph) {
    if (!graph) return;
    if (graph->type == ADJ_MATRIX && graph->adjMatrix) {
        for (int i = 0; i < graph->vertices; i++) {
            free(graph->adjMatrix[i]);
        }
        free(graph->adjMatrix);
    } else if (graph->type == ADJ_LIST && graph->adjList) {
        for (int i = 0; i < graph->vertices; i++) {
            free(graph->adjList[i]);
        }
        free(graph->adjList);
        free(graph->listSizes);
    }
    free(graph);
}

struct Graph *createGraph(enum GraphType type, int vertices) {
    if (vertices <= 0 || vertices > 100) return NULL;
    struct Graph *graph = (struct Graph *)malloc(sizeof(struct Graph));
    if (!graph) return NULL;
    graph->type = type;
    graph->vertices = vertices;
    graph->adjMatrix = NULL;
    graph->adjList = NULL;
    graph->listSizes = NULL;
    if (type == ADJ_MATRIX) {
        graph->adjMatrix = (int **)malloc(vertices * sizeof(int *));
        if (!graph->adjMatrix) {
            free(graph);
            return NULL;
        }
        for (int i = 0; i < vertices; i++) {
            graph->adjMatrix[i] = (int *)calloc(vertices, sizeof(int));
            if (!graph->adjMatrix[i]) {
                for (int j = 0; j < i; j++) free(graph->adjMatrix[j]);
                free(graph->adjMatrix);
                free(graph);
                return NULL;
            }
        }
    } else if (type == ADJ_LIST) {
        graph->adjList = (int **)malloc(vertices * sizeof(int *));
        graph->listSizes = (int *)calloc(vertices, sizeof(int));
        if (!graph->adjList || !graph->listSizes) {
            free(graph->adjList);
            free(graph->listSizes);
            free(graph);
            return NULL;
        }
        for (int i = 0; i < vertices; i++) {
            graph->adjList[i] = NULL;
        }
    }
    return graph;
}

void addEdge(struct Graph *graph, int src, int dest) {
    if (!graph || src < 0 || src >= graph->vertices || dest < 0 || dest >= graph->vertices) return;
    if (graph->type == ADJ_MATRIX) {
        graph->adjMatrix[src][dest] = 1;
        graph->adjMatrix[dest][src] = 1;
    } else if (graph->type == ADJ_LIST) {
        int newSize = graph->listSizes[src] + 1;
        int *newList = (int *)realloc(graph->adjList[src], newSize * sizeof(int));
        if (!newList) return;
        graph->adjList[src] = newList;
        graph->adjList[src][graph->listSizes[src]] = dest;
        graph->listSizes[src]++;
        newSize = graph->listSizes[dest] + 1;
        newList = (int *)realloc(graph->adjList[dest], newSize * sizeof(int));
        if (!newList) return;
        graph->adjList[dest] = newList;