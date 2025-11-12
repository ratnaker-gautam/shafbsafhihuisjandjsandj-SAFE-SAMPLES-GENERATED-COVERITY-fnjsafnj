//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

struct GraphNode {
    int id;
    int visited;
    int distance;
    struct GraphNode* parent;
};

struct GraphEdge {
    struct GraphNode* source;
    struct GraphNode* target;
    int weight;
};

struct Graph {
    struct GraphNode** nodes;
    struct GraphEdge** edges;
    int node_count;
    int edge_count;
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

struct Graph* graph_create(int initial_capacity) {
    if (initial_capacity <= 0) return NULL;
    struct Graph* graph = malloc(sizeof(struct Graph));
    if (!graph) return NULL;
    graph->nodes = malloc(sizeof(struct GraphNode*) * initial_capacity);
    graph->edges = malloc(sizeof(struct GraphEdge*) * initial_capacity * 2);
    if (!graph->nodes || !graph->edges) {
        free(graph->nodes);
        free(graph->edges);
        free(graph);
        return NULL;
    }
    graph->node_count = 0;
    graph->edge_count = 0;
    graph->capacity = initial_capacity;
    return graph;
}

struct GraphNode* graph_add_node(struct Graph* graph, int id) {
    if (!graph || id < 0) return NULL;
    for (int i = 0; i < graph->node_count; i++) {
        if (graph->nodes[i]->id == id) return NULL;
    }
    if (graph->node_count >= graph->capacity) return NULL;
    struct GraphNode* node = malloc(sizeof(struct GraphNode));
    if (!node) return NULL;
    node->id = id;
    node->visited = 0;
    node->distance = INT_MAX;
    node->parent = NULL;
    graph->nodes[graph->node_count++] = node;
    return node;
}

int graph_add_edge(struct Graph* graph, int source_id, int target_id, int weight) {
    if (!graph || weight < 0) return 0;
    struct GraphNode* source = NULL;
    struct GraphNode* target = NULL;
    for (int i = 0; i < graph->node_count; i++) {
        if (graph->nodes[i]->id == source_id) source = graph->nodes[i];
        if (graph->nodes[i]->id == target_id) target = graph->nodes[i];
    }
    if (!source || !target) return 0;
    if (graph->edge_count >= graph->capacity * 2) return 0;
    struct GraphEdge* edge = malloc(sizeof(struct GraphEdge));
    if (!edge) return 0;
    edge->source = source;
    edge->target = target;
    edge->weight = weight;
    graph->edges[graph->edge_count++] = edge;
    return 1;
}

struct Queue* queue_create() {
    struct Queue* queue = malloc(sizeof(struct Queue));
    if (!queue) return NULL;
    queue->front = NULL;
    queue->rear = NULL;
    queue->size = 0;
    return queue;
}

int queue_enqueue(struct Queue* queue, struct GraphNode* data) {
    if (!queue || !data) return 0;
    struct QueueNode* new_node = malloc(sizeof(struct QueueNode));
    if (!new_node) return 0;
    new_node->data = data;
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

struct GraphNode* queue_dequeue(struct Queue* queue) {
    if (!queue || !queue->front) return NULL;
    struct QueueNode* temp = queue->front;
    struct GraphNode* data = temp->data;
    queue->front = queue->front->next;
    if (!queue->front) queue->rear = NULL;
    free(temp);
    queue->size--;
    return data;
}

void queue_free(struct Queue* queue) {
    if (!queue) return;
    while (queue->front) {
        struct QueueNode* temp = queue->front;
        queue->front = queue->front->next;
        free(temp);
    }
    free(queue);
}

void graph_reset(struct Graph* graph) {
    if (!graph) return;
    for (int i = 0; i < graph->node_count; i++) {
        graph->nodes[i]->visited = 0;
        graph->nodes[i]->distance = INT_MAX;
        graph->nodes[i]->parent = NULL;
    }
}

void bfs_shortest_path(struct Graph* graph, int start_id) {
    if (!graph) return;
    graph_reset(graph);
    struct GraphNode* start = NULL;
    for (int i = 0; i < graph->node_count; i++) {
        if (graph->nodes[i]->id == start_id) {
            start = graph->nodes[i];
            break;
        }
    }
    if (!start) return;
    start->distance = 0;
    start->visited = 1;
    struct Queue* queue = queue_create();
    if (!queue) return;
    queue_enqueue(queue, start);
    while (queue->size > 0) {
        struct GraphNode* current = queue_dequeue(queue);
        for (int i = 0; i <