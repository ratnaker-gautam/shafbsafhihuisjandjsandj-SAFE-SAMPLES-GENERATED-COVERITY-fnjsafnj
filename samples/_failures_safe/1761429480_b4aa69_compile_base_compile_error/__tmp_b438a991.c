//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_NODES 100
#define MAX_EDGES 1000

struct Node {
    int id;
    char label[32];
    int visited;
    int distance;
};

struct Edge {
    struct Node* source;
    struct Node* target;
    int weight;
};

struct Graph {
    struct Node nodes[MAX_NODES];
    struct Edge edges[MAX_EDGES];
    int node_count;
    int edge_count;
};

struct QueueNode {
    struct Node* data;
    struct QueueNode* next;
};

struct Queue {
    struct QueueNode* front;
    struct QueueNode* rear;
    int size;
};

void init_queue(struct Queue* q) {
    q->front = NULL;
    q->rear = NULL;
    q->size = 0;
}

int is_queue_empty(struct Queue* q) {
    return q->size == 0;
}

void enqueue(struct Queue* q, struct Node* node) {
    if (q->size >= MAX_NODES) return;
    struct QueueNode* new_node = malloc(sizeof(struct QueueNode));
    if (!new_node) return;
    new_node->data = node;
    new_node->next = NULL;
    if (q->rear) {
        q->rear->next = new_node;
    }
    q->rear = new_node;
    if (!q->front) {
        q->front = new_node;
    }
    q->size++;
}

struct Node* dequeue(struct Queue* q) {
    if (is_queue_empty(q)) return NULL;
    struct QueueNode* temp = q->front;
    struct Node* data = temp->data;
    q->front = q->front->next;
    if (!q->front) {
        q->rear = NULL;
    }
    free(temp);
    q->size--;
    return data;
}

void init_graph(struct Graph* g) {
    g->node_count = 0;
    g->edge_count = 0;
}

int add_node(struct Graph* g, const char* label) {
    if (g->node_count >= MAX_NODES) return -1;
    if (!label || strlen(label) >= 32) return -1;
    struct Node* node = &g->nodes[g->node_count];
    node->id = g->node_count;
    strncpy(node->label, label, 31);
    node->label[31] = '\0';
    node->visited = 0;
    node->distance = INT_MAX;
    g->node_count++;
    return node->id;
}

int add_edge(struct Graph* g, int src_id, int dest_id, int weight) {
    if (src_id < 0 || src_id >= g->node_count) return -1;
    if (dest_id < 0 || dest_id >= g->node_count) return -1;
    if (g->edge_count >= MAX_EDGES) return -1;
    if (weight < 0) return -1;
    struct Edge* edge = &g->edges[g->edge_count];
    edge->source = &g->nodes[src_id];
    edge->target = &g->nodes[dest_id];
    edge->weight = weight;
    g->edge_count++;
    return 0;
}

void bfs(struct Graph* g, int start_id) {
    if (start_id < 0 || start_id >= g->node_count) return;
    for (int i = 0; i < g->node_count; i++) {
        g->nodes[i].visited = 0;
        g->nodes[i].distance = INT_MAX;
    }
    struct Queue q;
    init_queue(&q);
    struct Node* start = &g->nodes[start_id];
    start->visited = 1;
    start->distance = 0;
    enqueue(&q, start);
    printf("BFS traversal from %s:\n", start->label);
    while (!is_queue_empty(&q)) {
        struct Node* current = dequeue(&q);
        printf("Node %s (distance: %d)\n", current->label, current->distance);
        for (int i = 0; i < g->edge_count; i++) {
            if (g->edges[i].source == current && !g->edges[i].target->visited) {
                g->edges[i].target->visited = 1;
                g->edges[i].target->distance = current->distance + 1;
                enqueue(&q, g->edges[i].target);
            }
        }
    }
    while (!is_queue_empty(&q)) {
        dequeue(&q);
    }
}

void dijkstra(struct Graph* g, int start_id) {
    if (start_id < 0 || start_id >= g->node_count) return;
    for (int i = 0; i < g->node_count; i++) {
        g->nodes[i].visited = 0;
        g->nodes[i].distance = INT_MAX;
    }
    g->nodes[start_id].distance = 0;
    for (int count = 0; count < g->node_count - 1; count++) {
        int min_dist = INT_MAX;
        int min_index = -1;
        for (int i = 0; i < g->node_count; i++) {
            if (!g->nodes[i].visited && g->nodes[i].distance <= min_dist) {
                min_dist = g->nodes[i].distance;
                min_index = i;
            }
        }