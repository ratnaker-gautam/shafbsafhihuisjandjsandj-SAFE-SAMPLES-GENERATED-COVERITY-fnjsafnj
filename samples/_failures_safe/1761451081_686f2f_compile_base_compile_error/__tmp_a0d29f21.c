//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

struct GraphNode {
    int id;
    int visited;
    struct GraphNode** neighbors;
    int neighbor_count;
    int capacity;
};

struct Graph {
    struct GraphNode** nodes;
    int node_count;
    int capacity;
};

struct QueueNode {
    struct GraphNode* data;
    struct QueueNode* next;
};

struct Queue {
    struct QueueNode* front;
    struct QueueNode* rear;
    int size;
};

struct Graph* create_graph(int initial_capacity) {
    if (initial_capacity <= 0) return NULL;
    struct Graph* graph = malloc(sizeof(struct Graph));
    if (!graph) return NULL;
    graph->nodes = malloc(sizeof(struct GraphNode*) * initial_capacity);
    if (!graph->nodes) {
        free(graph);
        return NULL;
    }
    graph->node_count = 0;
    graph->capacity = initial_capacity;
    return graph;
}

struct GraphNode* create_node(int id) {
    if (id < 0) return NULL;
    struct GraphNode* node = malloc(sizeof(struct GraphNode));
    if (!node) return NULL;
    node->id = id;
    node->visited = 0;
    node->neighbors = malloc(sizeof(struct GraphNode*) * 4);
    if (!node->neighbors) {
        free(node);
        return NULL;
    }
    node->neighbor_count = 0;
    node->capacity = 4;
    return node;
}

void destroy_graph(struct Graph* graph) {
    if (!graph) return;
    for (int i = 0; i < graph->node_count; i++) {
        if (graph->nodes[i]) {
            free(graph->nodes[i]->neighbors);
            free(graph->nodes[i]);
        }
    }
    free(graph->nodes);
    free(graph);
}

int add_node(struct Graph* graph, int id) {
    if (!graph || id < 0) return 0;
    for (int i = 0; i < graph->node_count; i++) {
        if (graph->nodes[i]->id == id) return 0;
    }
    if (graph->node_count >= graph->capacity) {
        int new_capacity = graph->capacity * 2;
        if (new_capacity <= graph->capacity) return 0;
        struct GraphNode** new_nodes = realloc(graph->nodes, sizeof(struct GraphNode*) * new_capacity);
        if (!new_nodes) return 0;
        graph->nodes = new_nodes;
        graph->capacity = new_capacity;
    }
    struct GraphNode* node = create_node(id);
    if (!node) return 0;
    graph->nodes[graph->node_count++] = node;
    return 1;
}

int add_edge(struct Graph* graph, int from_id, int to_id) {
    if (!graph || from_id < 0 || to_id < 0) return 0;
    struct GraphNode* from = NULL;
    struct GraphNode* to = NULL;
    for (int i = 0; i < graph->node_count; i++) {
        if (graph->nodes[i]->id == from_id) from = graph->nodes[i];
        if (graph->nodes[i]->id == to_id) to = graph->nodes[i];
    }
    if (!from || !to) return 0;
    if (from->neighbor_count >= from->capacity) {
        int new_capacity = from->capacity * 2;
        if (new_capacity <= from->capacity) return 0;
        struct GraphNode** new_neighbors = realloc(from->neighbors, sizeof(struct GraphNode*) * new_capacity);
        if (!new_neighbors) return 0;
        from->neighbors = new_neighbors;
        from->capacity = new_capacity;
    }
    from->neighbors[from->neighbor_count++] = to;
    return 1;
}

struct Queue* create_queue() {
    struct Queue* queue = malloc(sizeof(struct Queue));
    if (!queue) return NULL;
    queue->front = NULL;
    queue->rear = NULL;
    queue->size = 0;
    return queue;
}

void destroy_queue(struct Queue* queue) {
    if (!queue) return;
    struct QueueNode* current = queue->front;
    while (current) {
        struct QueueNode* next = current->next;
        free(current);
        current = next;
    }
    free(queue);
}

int enqueue(struct Queue* queue, struct GraphNode* node) {
    if (!queue || !node) return 0;
    struct QueueNode* new_node = malloc(sizeof(struct QueueNode));
    if (!new_node) return 0;
    new_node->data = node;
    new_node->next = NULL;
    if (queue->rear) {
        queue->rear->next = new_node;
    } else {
        queue->front = new_node;
    }
    queue->rear = new_node;
    queue->size++;
    return 1;
}

struct GraphNode* dequeue(struct Queue* queue) {
    if (!queue || !queue->front) return NULL;
    struct QueueNode* temp = queue->front;
    struct GraphNode* data = temp->data;
    queue->front = queue->front->next;
    if (!queue->front) queue->rear = NULL;
    free(temp);
    queue->size--;
    return data;
}

void reset_visited(struct Graph* graph) {
    if (!graph) return;
    for (int i = 0; i < graph->node