//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

struct Edge {
    int dest;
    int weight;
    struct Edge* next;
};

struct Vertex {
    int id;
    struct Edge* edges;
    int visited;
    int distance;
};

struct Graph {
    int vertex_count;
    struct Vertex* vertices;
};

struct QueueNode {
    int vertex_id;
    struct QueueNode* next;
};

struct Queue {
    struct QueueNode* front;
    struct QueueNode* rear;
};

struct Graph* create_graph(int vertex_count) {
    if (vertex_count <= 0 || vertex_count > 1000) {
        return NULL;
    }
    struct Graph* graph = malloc(sizeof(struct Graph));
    if (!graph) {
        return NULL;
    }
    graph->vertex_count = vertex_count;
    graph->vertices = malloc(vertex_count * sizeof(struct Vertex));
    if (!graph->vertices) {
        free(graph);
        return NULL;
    }
    for (int i = 0; i < vertex_count; i++) {
        graph->vertices[i].id = i;
        graph->vertices[i].edges = NULL;
        graph->vertices[i].visited = 0;
        graph->vertices[i].distance = INT_MAX;
    }
    return graph;
}

int add_edge(struct Graph* graph, int src, int dest, int weight) {
    if (!graph || src < 0 || src >= graph->vertex_count || dest < 0 || dest >= graph->vertex_count || src == dest) {
        return 0;
    }
    struct Edge* new_edge = malloc(sizeof(struct Edge));
    if (!new_edge) {
        return 0;
    }
    new_edge->dest = dest;
    new_edge->weight = weight;
    new_edge->next = graph->vertices[src].edges;
    graph->vertices[src].edges = new_edge;
    return 1;
}

struct Queue* create_queue() {
    struct Queue* queue = malloc(sizeof(struct Queue));
    if (!queue) {
        return NULL;
    }
    queue->front = NULL;
    queue->rear = NULL;
    return queue;
}

int enqueue(struct Queue* queue, int vertex_id) {
    if (!queue) {
        return 0;
    }
    struct QueueNode* new_node = malloc(sizeof(struct QueueNode));
    if (!new_node) {
        return 0;
    }
    new_node->vertex_id = vertex_id;
    new_node->next = NULL;
    if (queue->rear) {
        queue->rear->next = new_node;
    } else {
        queue->front = new_node;
    }
    queue->rear = new_node;
    return 1;
}

int dequeue(struct Queue* queue, int* vertex_id) {
    if (!queue || !queue->front) {
        return 0;
    }
    struct QueueNode* temp = queue->front;
    *vertex_id = temp->vertex_id;
    queue->front = queue->front->next;
    if (!queue->front) {
        queue->rear = NULL;
    }
    free(temp);
    return 1;
}

int is_queue_empty(struct Queue* queue) {
    return queue && queue->front == NULL;
}

void free_queue(struct Queue* queue) {
    if (!queue) {
        return;
    }
    while (queue->front) {
        struct QueueNode* temp = queue->front;
        queue->front = queue->front->next;
        free(temp);
    }
    free(queue);
}

void free_graph(struct Graph* graph) {
    if (!graph) {
        return;
    }
    for (int i = 0; i < graph->vertex_count; i++) {
        struct Edge* edge = graph->vertices[i].edges;
        while (edge) {
            struct Edge* temp = edge;
            edge = edge->next;
            free(temp);
        }
    }
    free(graph->vertices);
    free(graph);
}

void bfs_shortest_path(struct Graph* graph, int start) {
    if (!graph || start < 0 || start >= graph->vertex_count) {
        return;
    }
    for (int i = 0; i < graph->vertex_count; i++) {
        graph->vertices[i].visited = 0;
        graph->vertices[i].distance = INT_MAX;
    }
    graph->vertices[start].distance = 0;
    struct Queue* queue = create_queue();
    if (!queue) {
        return;
    }
    enqueue(queue, start);
    while (!is_queue_empty(queue)) {
        int current;
        if (!dequeue(queue, &current)) {
            break;
        }
        struct Edge* edge = graph->vertices[current].edges;
        while (edge) {
            int neighbor = edge->dest;
            if (graph->vertices[neighbor].distance == INT_MAX) {
                graph->vertices[neighbor].distance = graph->vertices[current].distance + 1;
                enqueue(queue, neighbor);
            }
            edge = edge->next;
        }
    }
    free_queue(queue);
}

int main() {
    int vertex_count;
    printf("Enter number of vertices (1-100): ");
    if (scanf("%d", &vertex_count) != 1 || vertex_count < 1 || vertex_count > 100) {
        printf("Invalid input.\n");
        return 1;
    }
    struct Graph* graph = create_graph(vertex_count);
    if (!graph) {
        printf("Graph creation failed.\n");
        return 1;
    }
    int edge_count;
    printf("Enter number of edges: ");
    if (scanf("%d", &edge_count) != 1 || edge_count