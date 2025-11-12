//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_NODES 100
#define MAX_EDGES 1000

struct GraphNode {
    int id;
    int visited;
    int distance;
};

struct GraphEdge {
    int from;
    int to;
    int weight;
};

struct Graph {
    struct GraphNode nodes[MAX_NODES];
    struct GraphEdge edges[MAX_EDGES];
    int node_count;
    int edge_count;
};

struct QueueNode {
    int data;
    struct QueueNode* next;
};

struct Queue {
    struct QueueNode* front;
    struct QueueNode* rear;
    int size;
};

struct Graph graph_init(void) {
    struct Graph g;
    g.node_count = 0;
    g.edge_count = 0;
    return g;
}

int graph_add_node(struct Graph* g, int id) {
    if (g == NULL || g->node_count >= MAX_NODES) {
        return 0;
    }
    if (id < 0 || id >= MAX_NODES) {
        return 0;
    }
    for (int i = 0; i < g->node_count; i++) {
        if (g->nodes[i].id == id) {
            return 0;
        }
    }
    g->nodes[g->node_count].id = id;
    g->nodes[g->node_count].visited = 0;
    g->nodes[g->node_count].distance = INT_MAX;
    g->node_count++;
    return 1;
}

int graph_add_edge(struct Graph* g, int from, int to, int weight) {
    if (g == NULL || g->edge_count >= MAX_EDGES) {
        return 0;
    }
    if (from < 0 || from >= MAX_NODES || to < 0 || to >= MAX_NODES) {
        return 0;
    }
    if (weight < 0) {
        return 0;
    }
    int from_exists = 0;
    int to_exists = 0;
    for (int i = 0; i < g->node_count; i++) {
        if (g->nodes[i].id == from) from_exists = 1;
        if (g->nodes[i].id == to) to_exists = 1;
    }
    if (!from_exists || !to_exists) {
        return 0;
    }
    g->edges[g->edge_count].from = from;
    g->edges[g->edge_count].to = to;
    g->edges[g->edge_count].weight = weight;
    g->edge_count++;
    return 1;
}

struct Queue* queue_create(void) {
    struct Queue* q = malloc(sizeof(struct Queue));
    if (q == NULL) {
        return NULL;
    }
    q->front = NULL;
    q->rear = NULL;
    q->size = 0;
    return q;
}

int queue_enqueue(struct Queue* q, int data) {
    if (q == NULL) {
        return 0;
    }
    struct QueueNode* new_node = malloc(sizeof(struct QueueNode));
    if (new_node == NULL) {
        return 0;
    }
    new_node->data = data;
    new_node->next = NULL;
    if (q->rear == NULL) {
        q->front = new_node;
        q->rear = new_node;
    } else {
        q->rear->next = new_node;
        q->rear = new_node;
    }
    q->size++;
    return 1;
}

int queue_dequeue(struct Queue* q, int* data) {
    if (q == NULL || q->front == NULL || data == NULL) {
        return 0;
    }
    struct QueueNode* temp = q->front;
    *data = temp->data;
    q->front = q->front->next;
    if (q->front == NULL) {
        q->rear = NULL;
    }
    free(temp);
    q->size--;
    return 1;
}

int queue_is_empty(struct Queue* q) {
    if (q == NULL) {
        return 1;
    }
    return q->front == NULL;
}

void queue_destroy(struct Queue* q) {
    if (q == NULL) {
        return;
    }
    while (!queue_is_empty(q)) {
        int dummy;
        queue_dequeue(q, &dummy);
    }
    free(q);
}

void graph_bfs(struct Graph* g, int start) {
    if (g == NULL) {
        return;
    }
    for (int i = 0; i < g->node_count; i++) {
        g->nodes[i].visited = 0;
        g->nodes[i].distance = INT_MAX;
    }
    struct GraphNode* start_node = NULL;
    for (int i = 0; i < g->node_count; i++) {
        if (g->nodes[i].id == start) {
            start_node = &g->nodes[i];
            break;
        }
    }
    if (start_node == NULL) {
        return;
    }
    start_node->visited = 1;
    start_node->distance = 0;
    struct Queue* q = queue_create();
    if (q == NULL) {
        return;
    }
    if (!queue_enqueue(q, start)) {
        queue_destroy(q);
        return;
    }
    while (!queue_is_empty(q)) {
        int current;
        if (!queue_dequeue(q, &current)) {