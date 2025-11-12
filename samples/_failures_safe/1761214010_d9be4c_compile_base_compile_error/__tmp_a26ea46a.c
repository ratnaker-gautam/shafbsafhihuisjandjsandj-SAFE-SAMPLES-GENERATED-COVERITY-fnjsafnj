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
    int source;
    int destination;
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

void queue_init(struct Queue* q) {
    q->front = NULL;
    q->rear = NULL;
    q->size = 0;
}

int queue_empty(struct Queue* q) {
    return q->size == 0;
}

int queue_enqueue(struct Queue* q, int value) {
    if (q->size >= MAX_NODES) return 0;
    struct QueueNode* new_node = malloc(sizeof(struct QueueNode));
    if (!new_node) return 0;
    new_node->data = value;
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

int queue_dequeue(struct Queue* q) {
    if (queue_empty(q)) return -1;
    struct QueueNode* temp = q->front;
    int value = temp->data;
    q->front = q->front->next;
    if (!q->front) q->rear = NULL;
    free(temp);
    q->size--;
    return value;
}

void queue_free(struct Queue* q) {
    while (!queue_empty(q)) {
        queue_dequeue(q);
    }
}

void graph_init(struct Graph* g) {
    g->node_count = 0;
    g->edge_count = 0;
    for (int i = 0; i < MAX_NODES; i++) {
        g->nodes[i].id = -1;
        g->nodes[i].visited = 0;
        g->nodes[i].distance = INT_MAX;
    }
}

int graph_add_node(struct Graph* g, int id) {
    if (g->node_count >= MAX_NODES || id < 0 || id >= MAX_NODES) return 0;
    for (int i = 0; i < g->node_count; i++) {
        if (g->nodes[i].id == id) return 0;
    }
    g->nodes[g->node_count].id = id;
    g->nodes[g->node_count].visited = 0;
    g->nodes[g->node_count].distance = INT_MAX;
    g->node_count++;
    return 1;
}

int graph_add_edge(struct Graph* g, int src, int dest, int weight) {
    if (g->edge_count >= MAX_EDGES || src < 0 || src >= MAX_NODES || dest < 0 || dest >= MAX_NODES || weight < 0) return 0;
    int src_exists = 0, dest_exists = 0;
    for (int i = 0; i < g->node_count; i++) {
        if (g->nodes[i].id == src) src_exists = 1;
        if (g->nodes[i].id == dest) dest_exists = 1;
    }
    if (!src_exists || !dest_exists) return 0;
    g->edges[g->edge_count].source = src;
    g->edges[g->edge_count].destination = dest;
    g->edges[g->edge_count].weight = weight;
    g->edge_count++;
    return 1;
}

void graph_bfs(struct Graph* g, int start) {
    if (start < 0 || start >= MAX_NODES) return;
    int start_exists = 0;
    for (int i = 0; i < g->node_count; i++) {
        if (g->nodes[i].id == start) {
            start_exists = 1;
            break;
        }
    }
    if (!start_exists) return;
    for (int i = 0; i < g->node_count; i++) {
        g->nodes[i].visited = 0;
        g->nodes[i].distance = INT_MAX;
    }
    struct Queue q;
    queue_init(&q);
    for (int i = 0; i < g->node_count; i++) {
        if (g->nodes[i].id == start) {
            g->nodes[i].visited = 1;
            g->nodes[i].distance = 0;
            queue_enqueue(&q, start);
            break;
        }
    }
    while (!queue_empty(&q)) {
        int current = queue_dequeue(&q);
        for (int i = 0; i < g->edge_count; i++) {
            if (g->edges[i].source == current) {
                int neighbor = g->edges[i].destination;
                for (int j = 0; j < g->node_count; j++) {
                    if (g->nodes[j].id == neighbor && !g->nodes[j].visited) {
                        g->nodes[j].visited = 1;
                        for (int k = 0; k < g->node_count; k++) {
                            if (