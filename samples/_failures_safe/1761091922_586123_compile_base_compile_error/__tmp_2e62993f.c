//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: graph_algorithms
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

struct Graph* graph_create(int initial_capacity) {
    if (initial_capacity <= 0) return NULL;
    struct Graph* g = malloc(sizeof(struct Graph));
    if (!g) return NULL;
    g->nodes = malloc(sizeof(struct GraphNode*) * initial_capacity);
    if (!g->nodes) {
        free(g);
        return NULL;
    }
    g->node_count = 0;
    g->capacity = initial_capacity;
    return g;
}

struct GraphNode* graph_node_create(int id) {
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

int graph_add_node(struct Graph* g, int id) {
    if (!g || id < 0) return 0;
    for (int i = 0; i < g->node_count; i++) {
        if (g->nodes[i]->id == id) return 0;
    }
    if (g->node_count >= g->capacity) {
        int new_cap = g->capacity * 2;
        if (new_cap <= g->capacity) return 0;
        struct GraphNode** new_nodes = realloc(g->nodes, sizeof(struct GraphNode*) * new_cap);
        if (!new_nodes) return 0;
        g->nodes = new_nodes;
        g->capacity = new_cap;
    }
    struct GraphNode* new_node = graph_node_create(id);
    if (!new_node) return 0;
    g->nodes[g->node_count] = new_node;
    g->node_count++;
    return 1;
}

int graph_find_node(struct Graph* g, int id) {
    if (!g) return -1;
    for (int i = 0; i < g->node_count; i++) {
        if (g->nodes[i]->id == id) return i;
    }
    return -1;
}

int graph_add_edge(struct Graph* g, int from_id, int to_id) {
    if (!g) return 0;
    int from_idx = graph_find_node(g, from_id);
    int to_idx = graph_find_node(g, to_id);
    if (from_idx == -1 || to_idx == -1) return 0;
    struct GraphNode* from = g->nodes[from_idx];
    struct GraphNode* to = g->nodes[to_idx];
    if (from->neighbor_count >= from->capacity) {
        int new_cap = from->capacity * 2;
        if (new_cap <= from->capacity) return 0;
        struct GraphNode** new_neighbors = realloc(from->neighbors, sizeof(struct GraphNode*) * new_cap);
        if (!new_neighbors) return 0;
        from->neighbors = new_neighbors;
        from->capacity = new_cap;
    }
    from->neighbors[from->neighbor_count] = to;
    from->neighbor_count++;
    return 1;
}

struct Queue* queue_create() {
    struct Queue* q = malloc(sizeof(struct Queue));
    if (!q) return NULL;
    q->front = NULL;
    q->rear = NULL;
    q->size = 0;
    return q;
}

int queue_enqueue(struct Queue* q, struct GraphNode* node) {
    if (!q || !node) return 0;
    struct QueueNode* new_node = malloc(sizeof(struct QueueNode));
    if (!new_node) return 0;
    new_node->data = node;
    new_node->next = NULL;
    if (q->rear) {
        q->rear->next = new_node;
    } else {
        q->front = new_node;
    }
    q->rear = new_node;
    q->size++;
    return 1;
}

struct GraphNode* queue_dequeue(struct Queue* q) {
    if (!q || !q->front) return NULL;
    struct QueueNode* temp = q->front;
    struct GraphNode* data = temp->data;
    q->front = q->front->next;
    if (!q->front) q->rear = NULL;
    free(temp);
    q->size--;
    return data;
}

void queue_free(struct Queue* q) {
    if (!q) return;
    while (q->front) {
        struct QueueNode* temp = q->front;
        q->front = q->front->next;
        free(temp);
    }
    free(q);
}

void graph_reset_visited(struct Graph* g) {
    if (!g) return;
    for (int i = 0; i < g->node_count; i++) {
        g->nodes[i]->visited = 0;
    }