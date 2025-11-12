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
        int **adj_matrix;
        struct Node **adj_list;
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
    if (queue->size == queue->capacity) return -1;
    queue->rear = (queue->rear + 1) % queue->capacity;
    queue->array[queue->rear] = item;
    queue->size++;
    return 0;
}

int dequeue(struct Queue *queue) {
    if (is_empty(queue)) return INT_MIN;
    int item = queue->array[queue->front];
    queue->front = (queue->front + 1) % queue->capacity;
    queue->size--;
    return item;
}

void free_queue(struct Queue *queue) {
    if (queue) {
        free(queue->array);
        free(queue);
    }
}

struct Graph *create_graph(enum GraphType type, int vertex_count) {
    if (vertex_count <= 0 || vertex_count > 100) return NULL;
    struct Graph *graph = malloc(sizeof(struct Graph));
    if (!graph) return NULL;
    graph->type = type;
    graph->vertex_count = vertex_count;
    
    switch (type) {
        case ADJ_MATRIX:
            graph->data.adj_matrix = malloc(vertex_count * sizeof(int *));
            if (!graph->data.adj_matrix) {
                free(graph);
                return NULL;
            }
            for (int i = 0; i < vertex_count; i++) {
                graph->data.adj_matrix[i] = calloc(vertex_count, sizeof(int));
                if (!graph->data.adj_matrix[i]) {
                    for (int j = 0; j < i; j++) free(graph->data.adj_matrix[j]);
                    free(graph->data.adj_matrix);
                    free(graph);
                    return NULL;
                }
            }
            break;
        case ADJ_LIST:
            graph->data.adj_list = calloc(vertex_count, sizeof(struct Node *));
            if (!graph->data.adj_list) {
                free(graph);
                return NULL;
            }
            break;
        default:
            free(graph);
            return NULL;
    }
    return graph;
}

void free_graph(struct Graph *graph) {
    if (!graph) return;
    switch (graph->type) {
        case ADJ_MATRIX:
            for (int i = 0; i < graph->vertex_count; i++) free(graph->data.adj_matrix[i]);
            free(graph->data.adj_matrix);
            break;
        case ADJ_LIST:
            for (int i = 0; i < graph->vertex_count; i++) {
                struct Node *current = graph->data.adj_list[i];
                while (current) {
                    struct Node *temp = current;
                    current = current->next;
                    free(temp);
                }
            }
            free(graph->data.adj_list);
            break;
        default:
            break;
    }
    free(graph);
}

int add_edge(struct Graph *graph, int src, int dest) {
    if (!graph || src < 0 || src >= graph->vertex_count || dest < 0 || dest >= graph->vertex_count) return -1;
    switch (graph->type) {
        case ADJ_MATRIX:
            graph->data.adj_matrix[src][dest] = 1;
            graph->data.adj_matrix[dest][src] = 1;
            break;
        case ADJ_LIST:
            {
                struct Node *new_node = malloc(sizeof(struct Node));
                if (!new_node) return -1;
                new_node->vertex = dest;
                new_node->next = graph->data.adj_list[src];
                graph->data.adj_list[src] = new_node;
                
                new_node = malloc(sizeof(struct Node));
                if (!new_node) return -1;
                new_node->vertex = src;
                new_node->next = graph->data.adj_list[dest];
                graph->data.adj_list[dest] = new_node;
            }
            break;
        default:
            return -1;
    }
    return 0;
}

void bfs(struct Graph *graph, int start_vertex) {
    if (!graph || start_vertex < 0 || start_vertex >= graph->vertex_count) return;
    int *visited = calloc(graph->vertex_count, sizeof(int));
    if (!visited) return;
    struct Queue *queue = create_queue(graph->vertex_count);
    if (!queue) {
        free(visited