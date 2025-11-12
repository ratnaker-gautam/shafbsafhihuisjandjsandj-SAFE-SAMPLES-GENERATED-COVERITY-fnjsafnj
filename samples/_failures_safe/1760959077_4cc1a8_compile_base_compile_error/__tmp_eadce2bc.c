//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdint.h>

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

struct QueueNode {
    struct GraphNode* data;
    struct QueueNode* next;
};

struct Queue {
    struct QueueNode* front;
    struct QueueNode* rear;
    int size;
};

struct Graph {
    struct GraphNode** nodes;
    struct GraphEdge** edges;
    int node_count;
    int node_capacity;
    int edge_count;
    int edge_capacity;
};

struct Queue* queue_create() {
    struct Queue* queue = malloc(sizeof(struct Queue));
    if (!queue) return NULL;
    queue->front = NULL;
    queue->rear = NULL;
    queue->size = 0;
    return queue;
}

void queue_enqueue(struct Queue* queue, struct GraphNode* data) {
    if (!queue || !data) return;
    struct QueueNode* new_node = malloc(sizeof(struct QueueNode));
    if (!new_node) return;
    new_node->data = data;
    new_node->next = NULL;
    if (!queue->rear) {
        queue->front = new_node;
        queue->rear = new_node;
    } else {
        queue->rear->next = new_node;
        queue->rear = new_node;
    }
    queue->size++;
}

struct GraphNode* queue_dequeue(struct Queue* queue) {
    if (!queue || !queue->front) {
        return NULL;
    }
    struct QueueNode* temp = queue->front;
    struct GraphNode* data = temp->data;
    queue->front = queue->front->next;
    if (!queue->front) {
        queue->rear = NULL;
    }
    free(temp);
    queue->size--;
    return data;
}

int queue_is_empty(struct Queue* queue) {
    return !queue || !queue->front;
}

void queue_destroy(struct Queue* queue) {
    if (!queue) return;
    while (!queue_is_empty(queue)) {
        queue_dequeue(queue);
    }
    free(queue);
}

struct Graph* graph_create(int initial_capacity) {
    if (initial_capacity <= 0) return NULL;
    struct Graph* graph = malloc(sizeof(struct Graph));
    if (!graph) return NULL;
    graph->nodes = malloc(sizeof(struct GraphNode*) * initial_capacity);
    if (!graph->nodes) {
        free(graph);
        return NULL;
    }
    graph->edges = malloc(sizeof(struct GraphEdge*) * initial_capacity * 2);
    if (!graph->edges) {
        free(graph->nodes);
        free(graph);
        return NULL;
    }
    graph->node_count = 0;
    graph->node_capacity = initial_capacity;
    graph->edge_count = 0;
    graph->edge_capacity = initial_capacity * 2;
    return graph;
}

int graph_add_node(struct Graph* graph, int id) {
    if (!graph || id < 0) return 0;
    for (int i = 0; i < graph->node_count; i++) {
        if (graph->nodes[i]->id == id) return 0;
    }
    if (graph->node_count >= graph->node_capacity) {
        int new_capacity = graph->node_capacity * 2;
        struct GraphNode** new_nodes = realloc(graph->nodes, sizeof(struct GraphNode*) * new_capacity);
        if (!new_nodes) return 0;
        graph->nodes = new_nodes;
        graph->node_capacity = new_capacity;
    }
    struct GraphNode* node = malloc(sizeof(struct GraphNode));
    if (!node) return 0;
    node->id = id;
    node->visited = 0;
    node->distance = INT_MAX;
    node->parent = NULL;
    graph->nodes[graph->node_count++] = node;
    return 1;
}

int graph_add_edge(struct Graph* graph, int source_id, int target_id, int weight) {
    if (!graph || weight <= 0) return 0;
    struct GraphNode* source = NULL;
    struct GraphNode* target = NULL;
    for (int i = 0; i < graph->node_count; i++) {
        if (graph->nodes[i]->id == source_id) source = graph->nodes[i];
        if (graph->nodes[i]->id == target_id) target = graph->nodes[i];
    }
    if (!source || !target) return 0;
    if (graph->edge_count >= graph->edge_capacity) {
        int new_capacity = graph->edge_capacity * 2;
        struct GraphEdge** new_edges = realloc(graph->edges, sizeof(struct GraphEdge*) * new_capacity);
        if (!new_edges) return 0;
        graph->edges = new_edges;
        graph->edge_capacity = new_capacity;
    }
    struct GraphEdge* edge = malloc(sizeof(struct GraphEdge));
    if (!edge) return 0;
    edge->source = source;
    edge->target = target;
    edge->weight = weight;
    graph->edges[graph->edge_count++] = edge;
    return 1;
}

void graph_reset(struct Graph* graph) {
    if (!graph) return;
    for (int i = 0; i < graph->node_count; i++) {
        graph->nodes[i]->visited = 0;
        graph->nodes[i]->distance = INT_MAX