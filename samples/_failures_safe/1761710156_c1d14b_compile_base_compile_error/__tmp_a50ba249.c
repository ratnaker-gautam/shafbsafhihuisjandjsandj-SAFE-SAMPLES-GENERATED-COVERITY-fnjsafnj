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
    struct GraphNode* nodes;
    struct GraphEdge* edges;
    int node_count;
    int edge_count;
    int capacity_nodes;
    int capacity_edges;
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

void queue_init(struct Queue* q) {
    q->front = NULL;
    q->rear = NULL;
    q->size = 0;
}

int queue_empty(struct Queue* q) {
    return q->size == 0;
}

void queue_enqueue(struct Queue* q, struct GraphNode* node) {
    if (node == NULL) return;
    struct QueueNode* new_node = malloc(sizeof(struct QueueNode));
    if (new_node == NULL) return;
    new_node->data = node;
    new_node->next = NULL;
    if (q->rear == NULL) {
        q->front = new_node;
        q->rear = new_node;
    } else {
        q->rear->next = new_node;
        q->rear = new_node;
    }
    q->size++;
}

struct GraphNode* queue_dequeue(struct Queue* q) {
    if (queue_empty(q)) return NULL;
    struct QueueNode* temp = q->front;
    struct GraphNode* node = temp->data;
    q->front = q->front->next;
    if (q->front == NULL) q->rear = NULL;
    free(temp);
    q->size--;
    return node;
}

void graph_init(struct Graph* g, int initial_nodes, int initial_edges) {
    if (initial_nodes <= 0) initial_nodes = 10;
    if (initial_edges <= 0) initial_edges = 20;
    g->nodes = malloc(initial_nodes * sizeof(struct GraphNode));
    g->edges = malloc(initial_edges * sizeof(struct GraphEdge));
    g->node_count = 0;
    g->edge_count = 0;
    g->capacity_nodes = initial_nodes;
    g->capacity_edges = initial_edges;
}

void graph_add_node(struct Graph* g, int id) {
    if (g->node_count >= g->capacity_nodes) {
        int new_capacity = g->capacity_nodes * 2;
        if (new_capacity <= g->capacity_nodes) return;
        struct GraphNode* new_nodes = realloc(g->nodes, new_capacity * sizeof(struct GraphNode));
        if (new_nodes == NULL) return;
        g->nodes = new_nodes;
        g->capacity_nodes = new_capacity;
    }
    g->nodes[g->node_count].id = id;
    g->nodes[g->node_count].visited = 0;
    g->nodes[g->node_count].distance = INT_MAX;
    g->nodes[g->node_count].parent = NULL;
    g->node_count++;
}

void graph_add_edge(struct Graph* g, int source_id, int target_id, int weight) {
    if (g->edge_count >= g->capacity_edges) {
        int new_capacity = g->capacity_edges * 2;
        if (new_capacity <= g->capacity_edges) return;
        struct GraphEdge* new_edges = realloc(g->edges, new_capacity * sizeof(struct GraphEdge));
        if (new_edges == NULL) return;
        g->edges = new_edges;
        g->capacity_edges = new_capacity;
    }
    struct GraphNode* source = NULL;
    struct GraphNode* target = NULL;
    for (int i = 0; i < g->node_count; i++) {
        if (g->nodes[i].id == source_id) source = &g->nodes[i];
        if (g->nodes[i].id == target_id) target = &g->nodes[i];
    }
    if (source == NULL || target == NULL) return;
    g->edges[g->edge_count].source = source;
    g->edges[g->edge_count].target = target;
    g->edges[g->edge_count].weight = weight;
    g->edge_count++;
}

struct GraphNode* graph_find_node(struct Graph* g, int id) {
    for (int i = 0; i < g->node_count; i++) {
        if (g->nodes[i].id == id) return &g->nodes[i];
    }
    return NULL;
}

void graph_bfs(struct Graph* g, int start_id) {
    struct GraphNode* start = graph_find_node(g, start_id);
    if (start == NULL) return;
    for (int i = 0; i < g->node_count; i++) {
        g->nodes[i].visited = 0;
        g->nodes[i].distance = INT_MAX;
        g->nodes[i].parent = NULL;
    }
    start->visited = 1;
    start->distance = 0;
    struct Queue q;
    queue_init(&q);
    queue_enqueue(&q, start);
    while (!queue_empty(&q)) {
        struct GraphNode* current = queue_dequeue(&q);
        for (int i = 0; i < g->edge_count; i++)