//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

struct GraphNode {
    int id;
    int visited;
    int distance;
    struct GraphNode *parent;
};

struct GraphEdge {
    struct GraphNode *source;
    struct GraphNode *target;
    int weight;
};

struct Graph {
    struct GraphNode **nodes;
    struct GraphEdge **edges;
    int node_count;
    int edge_count;
    int max_nodes;
    int max_edges;
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

struct Graph *graph_create(int max_nodes, int max_edges) {
    if (max_nodes <= 0 || max_edges <= 0) return NULL;
    struct Graph *g = malloc(sizeof(struct Graph));
    if (!g) return NULL;
    g->nodes = malloc(max_nodes * sizeof(struct GraphNode*));
    if (!g->nodes) { free(g); return NULL; }
    g->edges = malloc(max_edges * sizeof(struct GraphEdge*));
    if (!g->edges) { free(g->nodes); free(g); return NULL; }
    g->node_count = 0;
    g->edge_count = 0;
    g->max_nodes = max_nodes;
    g->max_edges = max_edges;
    return g;
}

struct GraphNode *graph_add_node(struct Graph *g, int id) {
    if (!g || g->node_count >= g->max_nodes) return NULL;
    for (int i = 0; i < g->node_count; i++) {
        if (g->nodes[i]->id == id) return NULL;
    }
    struct GraphNode *node = malloc(sizeof(struct GraphNode));
    if (!node) return NULL;
    node->id = id;
    node->visited = 0;
    node->distance = INT_MAX;
    node->parent = NULL;
    g->nodes[g->node_count++] = node;
    return node;
}

int graph_add_edge(struct Graph *g, int source_id, int target_id, int weight) {
    if (!g || g->edge_count >= g->max_edges || weight < 0) return 0;
    struct GraphNode *source = NULL, *target = NULL;
    for (int i = 0; i < g->node_count; i++) {
        if (g->nodes[i]->id == source_id) source = g->nodes[i];
        if (g->nodes[i]->id == target_id) target = g->nodes[i];
    }
    if (!source || !target) return 0;
    struct GraphEdge *edge = malloc(sizeof(struct GraphEdge));
    if (!edge) return 0;
    edge->source = source;
    edge->target = target;
    edge->weight = weight;
    g->edges[g->edge_count++] = edge;
    return 1;
}

void graph_reset(struct Graph *g) {
    if (!g) return;
    for (int i = 0; i < g->node_count; i++) {
        g->nodes[i]->visited = 0;
        g->nodes[i]->distance = INT_MAX;
        g->nodes[i]->parent = NULL;
    }
}

struct Queue *queue_create() {
    struct Queue *q = malloc(sizeof(struct Queue));
    if (!q) return NULL;
    q->front = q->rear = NULL;
    q->size = 0;
    return q;
}

int queue_enqueue(struct Queue *q, struct GraphNode *node) {
    if (!q || !node) return 0;
    struct QueueNode *new_node = malloc(sizeof(struct QueueNode));
    if (!new_node) return 0;
    new_node->data = node;
    new_node->next = NULL;
    if (q->rear) q->rear->next = new_node;
    else q->front = new_node;
    q->rear = new_node;
    q->size++;
    return 1;
}

struct GraphNode *queue_dequeue(struct Queue *q) {
    if (!q || !q->front) return NULL;
    struct QueueNode *temp = q->front;
    struct GraphNode *data = temp->data;
    q->front = q->front->next;
    if (!q->front) q->rear = NULL;
    free(temp);
    q->size--;
    return data;
}

void queue_destroy(struct Queue *q) {
    if (!q) return;
    while (q->front) queue_dequeue(q);
    free(q);
}

void bfs_shortest_path(struct Graph *g, int start_id) {
    if (!g) return;
    graph_reset(g);
    struct GraphNode *start = NULL;
    for (int i = 0; i < g->node_count; i++) {
        if (g->nodes[i]->id == start_id) {
            start = g->nodes[i];
            break;
        }
    }
    if (!start) return;
    start->distance = 0;
    struct Queue *q = queue_create();
    if (!q) return;
    queue_enqueue(q, start);
    while (q->size > 0) {
        struct GraphNode *current = queue_dequeue(q);
        if (!current) continue;
        for (int i = 0; i < g->edge_count; i++) {
            if (g->edges[i]->source == current) {
                struct GraphNode *neighbor = g->edges