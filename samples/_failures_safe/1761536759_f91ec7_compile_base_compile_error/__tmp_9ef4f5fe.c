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
    if (initial_capacity <= 0) {
        return NULL;
    }
    struct Graph* graph = malloc(sizeof(struct Graph));
    if (!graph) {
        return NULL;
    }
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

void graph_destroy(struct Graph* graph) {
    if (!graph) {
        return;
    }
    for (int i = 0; i < graph->node_count; i++) {
        free(graph->nodes[i]);
    }
    for (int i = 0; i < graph->edge_count; i++) {
        free(graph->edges[i]);
    }
    free(graph->nodes);
    free(graph->edges);
    free(graph);
}

struct GraphNode* graph_add_node(struct Graph* graph, int id) {
    if (!graph || id < 0) {
        return NULL;
    }
    for (int i = 0; i < graph->node_count; i++) {
        if (graph->nodes[i]->id == id) {
            return NULL;
        }
    }
    if (graph->node_count >= graph->capacity) {
        int new_capacity = graph->capacity * 2;
        if (new_capacity <= graph->capacity) {
            return NULL;
        }
        struct GraphNode** new_nodes = realloc(graph->nodes, sizeof(struct GraphNode*) * new_capacity);
        if (!new_nodes) {
            return NULL;
        }
        graph->nodes = new_nodes;
        graph->capacity = new_capacity;
    }
    struct GraphNode* node = malloc(sizeof(struct GraphNode));
    if (!node) {
        return NULL;
    }
    node->id = id;
    node->visited = 0;
    node->distance = INT_MAX;
    node->parent = NULL;
    graph->nodes[graph->node_count] = node;
    graph->node_count++;
    return node;
}

int graph_add_edge(struct Graph* graph, int source_id, int target_id, int weight) {
    if (!graph || weight < 0) {
        return 0;
    }
    struct GraphNode* source = NULL;
    struct GraphNode* target = NULL;
    for (int i = 0; i < graph->node_count; i++) {
        if (graph->nodes[i]->id == source_id) {
            source = graph->nodes[i];
        }
        if (graph->nodes[i]->id == target_id) {
            target = graph->nodes[i];
        }
    }
    if (!source || !target) {
        return 0;
    }
    if (graph->edge_count >= graph->capacity * 2) {
        int new_capacity = graph->capacity * 2;
        if (new_capacity <= graph->capacity) {
            return 0;
        }
        struct GraphEdge** new_edges = realloc(graph->edges, sizeof(struct GraphEdge*) * new_capacity * 2);
        if (!new_edges) {
            return 0;
        }
        graph->edges = new_edges;
        graph->capacity = new_capacity;
    }
    struct GraphEdge* edge = malloc(sizeof(struct GraphEdge));
    if (!edge) {
        return 0;
    }
    edge->source = source;
    edge->target = target;
    edge->weight = weight;
    graph->edges[graph->edge_count] = edge;
    graph->edge_count++;
    return 1;
}

struct Queue* queue_create() {
    struct Queue* queue = malloc(sizeof(struct Queue));
    if (!queue) {
        return NULL;
    }
    queue->front = NULL;
    queue->rear = NULL;
    queue->size = 0;
    return queue;
}

void queue_destroy(struct Queue* queue) {
    if (!queue) {
        return;
    }
    struct QueueNode* current = queue->front;
    while (current) {
        struct QueueNode* next = current->next;
        free(current);
        current = next;
    }
    free(queue);
}

int queue_enqueue(struct Queue* queue, struct GraphNode* data) {
    if (!queue || !data) {
        return 0;
    }
    struct QueueNode* new_node = malloc(sizeof(struct QueueNode));
    if (!new_node) {
        return 0;
    }
    new_node->data = data;
    new_node->next = NULL;
    if (queue->rear) {
        queue->rear->next = new_node;
    } else {
        queue->front = new_node;
    }