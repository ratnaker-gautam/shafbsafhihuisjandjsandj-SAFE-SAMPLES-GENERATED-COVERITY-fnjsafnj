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
    void *data;
};

struct Queue {
    int front, rear, size;
    unsigned capacity;
    int *array;
};

struct Queue *createQueue(unsigned capacity) {
    if (capacity == 0 || capacity > 1000) return NULL;
    struct Queue *queue = malloc(sizeof(struct Queue));
    if (!queue) return NULL;
    queue->capacity = capacity;
    queue->front = queue->size = 0;
    queue->rear = capacity - 1;
    queue->array = malloc(capacity * sizeof(int));
    if (!queue->array) {
        free(queue);
        return NULL;
    }
    return queue;
}

int isFull(struct Queue *queue) {
    if (!queue) return 1;
    return queue->size == queue->capacity;
}

int isEmpty(struct Queue *queue) {
    if (!queue) return 1;
    return queue->size == 0;
}

void enqueue(struct Queue *queue, int item) {
    if (!queue || isFull(queue)) return;
    queue->rear = (queue->rear + 1) % queue->capacity;
    queue->array[queue->rear] = item;
    queue->size++;
}

int dequeue(struct Queue *queue) {
    if (!queue || isEmpty(queue)) return -1;
    int item = queue->array[queue->front];
    queue->front = (queue->front + 1) % queue->capacity;
    queue->size--;
    return item;
}

void freeQueue(struct Queue *queue) {
    if (!queue) return;
    free(queue->array);
    free(queue);
}

int validateGraph(struct Graph *graph) {
    if (!graph) return 0;
    if (graph->vertices <= 0 || graph->vertices > 100) return 0;
    if (!graph->data) return 0;
    return 1;
}

void bfs(struct Graph *graph, int start) {
    if (!validateGraph(graph) || start < 0 || start >= graph->vertices) return;
    
    int visited[100] = {0};
    struct Queue *queue = createQueue(graph->vertices);
    if (!queue) return;
    
    visited[start] = 1;
    enqueue(queue, start);
    
    printf("BFS traversal: ");
    while (!isEmpty(queue)) {
        int current = dequeue(queue);
        printf("%d ", current);
        
        if (graph->type == ADJ_MATRIX) {
            int **matrix = (int **)graph->data;
            for (int i = 0; i < graph->vertices; i++) {
                if (matrix[current][i] && !visited[i]) {
                    visited[i] = 1;
                    enqueue(queue, i);
                }
            }
        }
    }
    printf("\n");
    freeQueue(queue);
}

void dfsUtil(struct Graph *graph, int v, int visited[]) {
    visited[v] = 1;
    printf("%d ", v);
    
    if (graph->type == ADJ_MATRIX) {
        int **matrix = (int **)graph->data;
        for (int i = 0; i < graph->vertices; i++) {
            if (matrix[v][i] && !visited[i]) {
                dfsUtil(graph, i, visited);
            }
        }
    }
}

void dfs(struct Graph *graph, int start) {
    if (!validateGraph(graph) || start < 0 || start >= graph->vertices) return;
    
    int visited[100] = {0};
    printf("DFS traversal: ");
    dfsUtil(graph, start, visited);
    printf("\n");
}

void shortestPath(struct Graph *graph, int start) {
    if (!validateGraph(graph) || start < 0 || start >= graph->vertices) return;
    
    int dist[100];
    for (int i = 0; i < graph->vertices; i++) {
        dist[i] = INT_MAX;
    }
    dist[start] = 0;
    
    struct Queue *queue = createQueue(graph->vertices);
    if (!queue) return;
    enqueue(queue, start);
    
    while (!isEmpty(queue)) {
        int u = dequeue(queue);
        
        if (graph->type == ADJ_MATRIX) {
            int **matrix = (int **)graph->data;
            for (int v = 0; v < graph->vertices; v++) {
                if (matrix[u][v] && dist[v] > dist[u] + 1) {
                    dist[v] = dist[u] + 1;
                    enqueue(queue, v);
                }
            }
        }
    }
    
    printf("Shortest paths from %d:\n", start);
    for (int i = 0; i < graph->vertices; i++) {
        if (dist[i] != INT_MAX) {
            printf("To %d: %d\n", i, dist[i]);
        } else {
            printf("To %d: unreachable\n", i);
        }
    }
    freeQueue(queue);
}

int cycleCheckUtil(struct Graph *graph, int v, int visited[], int parent) {
    visited[v] = 1;
    
    if (graph->type == ADJ_MATRIX) {
        int **matrix = (int **)graph