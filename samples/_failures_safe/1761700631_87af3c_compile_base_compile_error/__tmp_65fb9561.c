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

void add_edge(struct Graph* graph, int src, int dest, int weight) {
    if (!graph || src < 0 || src >= graph->vertex_count || dest < 0 || dest >= graph->vertex_count || weight < 0) {
        return;
    }
    struct Edge* new_edge = malloc(sizeof(struct Edge));
    if (!new_edge) {
        return;
    }
    new_edge->dest = dest;
    new_edge->weight = weight;
    new_edge->next = graph->vertices[src].edges;
    graph->vertices[src].edges = new_edge;
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

void enqueue(struct Queue* queue, int vertex_id) {
    if (!queue) {
        return;
    }
    struct QueueNode* new_node = malloc(sizeof(struct QueueNode));
    if (!new_node) {
        return;
    }
    new_node->vertex_id = vertex_id;
    new_node->next = NULL;
    if (queue->rear) {
        queue->rear->next = new_node;
    } else {
        queue->front = new_node;
    }
    queue->rear = new_node;
}

int dequeue(struct Queue* queue) {
    if (!queue || !queue->front) {
        return -1;
    }
    struct QueueNode* temp = queue->front;
    int vertex_id = temp->vertex_id;
    queue->front = queue->front->next;
    if (!queue->front) {
        queue->rear = NULL;
    }
    free(temp);
    return vertex_id;
}

int is_empty(struct Queue* queue) {
    return queue && queue->front == NULL;
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
    graph->vertices[start].visited = 1;
    struct Queue* queue = create_queue();
    if (!queue) {
        return;
    }
    enqueue(queue, start);
    while (!is_empty(queue)) {
        int current = dequeue(queue);
        struct Edge* edge = graph->vertices[current].edges;
        while (edge) {
            int neighbor = edge->dest;
            if (!graph->vertices[neighbor].visited) {
                graph->vertices[neighbor].visited = 1;
                graph->vertices[neighbor].distance = graph->vertices[current].distance + 1;
                enqueue(queue, neighbor);
            }
            edge = edge->next;
        }
    }
    free(queue);
}

void print_shortest_paths(struct Graph* graph, int start) {
    if (!graph) {
        return;
    }
    printf("Shortest paths from vertex %d:\n", start);
    for (int i = 0; i < graph->vertex_count; i++) {
        if (graph->vertices[i].distance == INT_MAX) {
            printf("Vertex %d: unreachable\n", i);
        } else {
            printf("Vertex %d: distance %d\n", i, graph->vertices[i].distance);
        }
    }
}

void free_graph(struct Graph* graph) {
    if (!graph) {
        return;
    }
    for (int i = 0; i < graph->vertex_count; i++) {
        struct Edge* edge = graph->vertices[i].edges;
        while (edge) {
            struct Edge* next = edge->next;
            free(edge);
            edge = next;
        }
    }
    free(graph->vertices);
    free(graph);
}

int main() {
    int vertex_count = 6;
    struct Graph* graph = create_graph(vertex_count);
    if (!graph) {
        printf("Failed to create graph\n");
        return 1;
    }
    add_edge(graph, 0, 1, 1);
    add_edge(graph, 0, 2, 1);
    add_edge(graph, 1, 3, 1);