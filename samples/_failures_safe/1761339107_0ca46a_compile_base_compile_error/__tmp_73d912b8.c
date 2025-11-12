//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

enum GraphType {
    ADJ_MATRIX,
    ADJ_LIST,
    GRAPH_TYPE_COUNT
};

enum Operation {
    BFS,
    DFS,
    SHORTEST_PATH,
    OPERATION_COUNT
};

struct Graph {
    enum GraphType type;
    int vertex_count;
    union {
        int **matrix;
        struct Node **list;
    } data;
};

struct Node {
    int vertex;
    struct Node *next;
};

struct Queue {
    int front, rear, size;
    unsigned capacity;
    int *array;
};

struct Queue *create_queue(unsigned capacity) {
    if (capacity == 0 || capacity > 1000) return NULL;
    struct Queue *queue = malloc(sizeof(struct Queue));
    if (!queue) return NULL;
    queue->array = malloc(capacity * sizeof(int));
    if (!queue->array) {
        free(queue);
        return NULL;
    }
    queue->capacity = capacity;
    queue->front = queue->size = 0;
    queue->rear = capacity - 1;
    return queue;
}

int is_empty(struct Queue *queue) {
    return queue->size == 0;
}

int enqueue(struct Queue *queue, int item) {
    if (queue->size == queue->capacity) return 0;
    queue->rear = (queue->rear + 1) % queue->capacity;
    queue->array[queue->rear] = item;
    queue->size++;
    return 1;
}

int dequeue(struct Queue *queue) {
    if (is_empty(queue)) return -1;
    int item = queue->array[queue->front];
    queue->front = (queue->front + 1) % queue->capacity;
    queue->size--;
    return item;
}

void free_graph(struct Graph *graph) {
    if (!graph) return;
    switch (graph->type) {
        case ADJ_MATRIX:
            if (graph->data.matrix) {
                for (int i = 0; i < graph->vertex_count; i++) {
                    free(graph->data.matrix[i]);
                }
                free(graph->data.matrix);
            }
            break;
        case ADJ_LIST:
            if (graph->data.list) {
                for (int i = 0; i < graph->vertex_count; i++) {
                    struct Node *current = graph->data.list[i];
                    while (current) {
                        struct Node *temp = current;
                        current = current->next;
                        free(temp);
                    }
                }
                free(graph->data.list);
            }
            break;
        default:
            break;
    }
    free(graph);
}

struct Graph *create_sample_graph(enum GraphType type) {
    struct Graph *graph = malloc(sizeof(struct Graph));
    if (!graph) return NULL;
    graph->type = type;
    graph->vertex_count = 5;
    
    switch (type) {
        case ADJ_MATRIX:
            graph->data.matrix = malloc(5 * sizeof(int *));
            if (!graph->data.matrix) {
                free(graph);
                return NULL;
            }
            for (int i = 0; i < 5; i++) {
                graph->data.matrix[i] = calloc(5, sizeof(int));
                if (!graph->data.matrix[i]) {
                    for (int j = 0; j < i; j++) free(graph->data.matrix[j]);
                    free(graph->data.matrix);
                    free(graph);
                    return NULL;
                }
            }
            graph->data.matrix[0][1] = 1;
            graph->data.matrix[0][2] = 1;
            graph->data.matrix[1][3] = 1;
            graph->data.matrix[2][4] = 1;
            graph->data.matrix[3][4] = 1;
            break;
            
        case ADJ_LIST:
            graph->data.list = calloc(5, sizeof(struct Node *));
            if (!graph->data.list) {
                free(graph);
                return NULL;
            }
            for (int i = 0; i < 5; i++) {
                graph->data.list[i] = NULL;
            }
            int edges[5][2] = {{0,1}, {0,2}, {1,3}, {2,4}, {3,4}};
            for (int i = 0; i < 5; i++) {
                int u = edges[i][0];
                int v = edges[i][1];
                if (u < 0 || u >= 5 || v < 0 || v >= 5) continue;
                
                struct Node *new_node = malloc(sizeof(struct Node));
                if (!new_node) {
                    free_graph(graph);
                    return NULL;
                }
                new_node->vertex = v;
                new_node->next = graph->data.list[u];
                graph->data.list[u] = new_node;
            }
            break;
            
        default:
            free(graph);
            return NULL;
    }
    return graph;
}

void bfs_traversal(struct Graph *graph, int start_vertex) {
    if (!graph || start_vertex < 0 || start_vertex >= graph->vertex_count) return;
    
    int *visited = calloc(graph->vertex_count, sizeof(int));
    if (!visited) return;
    
    struct Queue *queue = create_queue(graph->vertex_count);
    if (!queue) {
        free(visited);
        return;
    }
    
    visited[start_vertex] = 1;