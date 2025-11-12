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
    struct GraphNode *next;
};

struct GraphEdge {
    struct GraphNode *src;
    struct GraphNode *dest;
    int weight;
};

struct Graph {
    int node_count;
    int edge_count;
    struct GraphNode **nodes;
    struct GraphEdge **edges;
};

struct QueueNode {
    struct GraphNode *data;
    struct QueueNode *next;
};

struct Queue {
    struct QueueNode *front;
    struct QueueNode *rear;
    int size;
};

struct GraphNode *create_graph_node(int id) {
    if (id < 0 || id > 1000) return NULL;
    struct GraphNode *node = malloc(sizeof(struct GraphNode));
    if (!node) return NULL;
    node->id = id;
    node->visited = 0;
    node->distance = INT_MAX;
    node->next = NULL;
    return node;
}

struct GraphEdge *create_graph_edge(struct GraphNode *src, struct GraphNode *dest, int weight) {
    if (!src || !dest || weight < 0 || weight > 1000) return NULL;
    struct GraphEdge *edge = malloc(sizeof(struct GraphEdge));
    if (!edge) return NULL;
    edge->src = src;
    edge->dest = dest;
    edge->weight = weight;
    return edge;
}

struct Graph *create_graph(int node_count) {
    if (node_count <= 0 || node_count > 100) return NULL;
    struct Graph *graph = malloc(sizeof(struct Graph));
    if (!graph) return NULL;
    graph->node_count = node_count;
    graph->edge_count = 0;
    graph->nodes = malloc(node_count * sizeof(struct GraphNode *));
    graph->edges = malloc(100 * sizeof(struct GraphEdge *));
    if (!graph->nodes || !graph->edges) {
        free(graph->nodes);
        free(graph->edges);
        free(graph);
        return NULL;
    }
    for (int i = 0; i < node_count; i++) {
        graph->nodes[i] = create_graph_node(i);
        if (!graph->nodes[i]) {
            for (int j = 0; j < i; j++) free(graph->nodes[j]);
            free(graph->nodes);
            free(graph->edges);
            free(graph);
            return NULL;
        }
    }
    return graph;
}

void free_graph(struct Graph *graph) {
    if (!graph) return;
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

int add_edge(struct Graph *graph, int src_id, int dest_id, int weight) {
    if (!graph || src_id < 0 || src_id >= graph->node_count || dest_id < 0 || dest_id >= graph->node_count) return 0;
    if (graph->edge_count >= 100) return 0;
    struct GraphEdge *edge = create_graph_edge(graph->nodes[src_id], graph->nodes[dest_id], weight);
    if (!edge) return 0;
    graph->edges[graph->edge_count] = edge;
    graph->edge_count++;
    return 1;
}

struct Queue *create_queue() {
    struct Queue *queue = malloc(sizeof(struct Queue));
    if (!queue) return NULL;
    queue->front = NULL;
    queue->rear = NULL;
    queue->size = 0;
    return queue;
}

int enqueue(struct Queue *queue, struct GraphNode *node) {
    if (!queue || !node) return 0;
    struct QueueNode *new_node = malloc(sizeof(struct QueueNode));
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

struct GraphNode *dequeue(struct Queue *queue) {
    if (!queue || !queue->front) return NULL;
    struct QueueNode *temp = queue->front;
    struct GraphNode *node = temp->data;
    queue->front = queue->front->next;
    if (!queue->front) queue->rear = NULL;
    free(temp);
    queue->size--;
    return node;
}

void free_queue(struct Queue *queue) {
    if (!queue) return;
    while (queue->front) {
        dequeue(queue);
    }
    free(queue);
}

void bfs(struct Graph *graph, int start_id) {
    if (!graph || start_id < 0 || start_id >= graph->node_count) return;
    for (int i = 0; i < graph->node_count; i++) {
        graph->nodes[i]->visited = 0;
        graph->nodes[i]->distance = INT_MAX;
    }
    struct Queue *queue = create_queue();
    if (!queue) return;
    graph->nodes[start_id]->visited = 1;
    graph->nodes[start_id]->distance = 0;
    enqueue(queue, graph->nodes[start_id]);
    while (queue->size > 0) {
        struct GraphNode *current = dequeue(queue);
        for