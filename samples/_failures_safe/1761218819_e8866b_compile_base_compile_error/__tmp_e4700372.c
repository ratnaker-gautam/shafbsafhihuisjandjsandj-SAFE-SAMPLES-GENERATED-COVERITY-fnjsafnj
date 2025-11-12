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
    struct GraphNode* destination;
    int weight;
};

struct Graph {
    struct GraphNode** nodes;
    struct GraphEdge** edges;
    int node_count;
    int edge_count;
    int max_nodes;
    int max_edges;
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

struct Graph* create_graph(int max_nodes, int max_edges) {
    if (max_nodes <= 0 || max_edges <= 0) return NULL;
    struct Graph* graph = malloc(sizeof(struct Graph));
    if (!graph) return NULL;
    graph->nodes = malloc(max_nodes * sizeof(struct GraphNode*));
    graph->edges = malloc(max_edges * sizeof(struct GraphEdge*));
    if (!graph->nodes || !graph->edges) {
        free(graph->nodes);
        free(graph->edges);
        free(graph);
        return NULL;
    }
    graph->node_count = 0;
    graph->edge_count = 0;
    graph->max_nodes = max_nodes;
    graph->max_edges = max_edges;
    return graph;
}

struct GraphNode* create_node(int id) {
    if (id < 0) return NULL;
    struct GraphNode* node = malloc(sizeof(struct GraphNode));
    if (!node) return NULL;
    node->id = id;
    node->visited = 0;
    node->distance = INT_MAX;
    node->parent = NULL;
    return node;
}

struct GraphEdge* create_edge(struct GraphNode* src, struct GraphNode* dst, int weight) {
    if (!src || !dst || weight < 0) return NULL;
    struct GraphEdge* edge = malloc(sizeof(struct GraphEdge));
    if (!edge) return NULL;
    edge->source = src;
    edge->destination = dst;
    edge->weight = weight;
    return edge;
}

int add_node(struct Graph* graph, int id) {
    if (!graph || id < 0 || graph->node_count >= graph->max_nodes) return 0;
    for (int i = 0; i < graph->node_count; i++) {
        if (graph->nodes[i]->id == id) return 0;
    }
    struct GraphNode* node = create_node(id);
    if (!node) return 0;
    graph->nodes[graph->node_count++] = node;
    return 1;
}

int add_edge(struct Graph* graph, int src_id, int dst_id, int weight) {
    if (!graph || src_id < 0 || dst_id < 0 || weight < 0) return 0;
    if (graph->edge_count >= graph->max_edges) return 0;
    struct GraphNode* src = NULL;
    struct GraphNode* dst = NULL;
    for (int i = 0; i < graph->node_count; i++) {
        if (graph->nodes[i]->id == src_id) src = graph->nodes[i];
        if (graph->nodes[i]->id == dst_id) dst = graph->nodes[i];
    }
    if (!src || !dst) return 0;
    struct GraphEdge* edge = create_edge(src, dst, weight);
    if (!edge) return 0;
    graph->edges[graph->edge_count++] = edge;
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

void reset_graph(struct Graph* graph) {
    if (!graph) return;
    for (int i = 0; i < graph->node_count; i++) {
        graph->nodes[i]->visited = 0;
        graph->nodes[i]->distance = INT_MAX;
        graph->nodes[i]->parent = NULL;
    }
}

void bfs_shortest_path(struct Graph* graph, int start_id) {
    if (!graph) return;
    reset_graph(graph);
    struct GraphNode* start = NULL;
    for (int i = 0; i < graph->node_count; i++) {
        if (graph->nodes[i]->id == start_id) {
            start = graph->nodes[i];
            break;
        }
    }
    if