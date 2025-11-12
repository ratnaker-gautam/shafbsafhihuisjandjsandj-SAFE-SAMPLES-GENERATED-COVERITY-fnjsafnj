//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_NODES 100
#define MAX_EDGES 500

struct Node {
    int id;
    char label[32];
    int visited;
    int distance;
};

struct Edge {
    struct Node* from;
    struct Node* to;
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

void queue_init(struct Queue* q) {
    q->front = NULL;
    q->rear = NULL;
    q->size = 0;
}

int queue_empty(struct Queue* q) {
    return q->size == 0;
}

int queue_enqueue(struct Queue* q, struct Node* node) {
    if (q->size >= MAX_NODES) return 0;
    struct QueueNode* new_node = malloc(sizeof(struct QueueNode));
    if (!new_node) return 0;
    new_node->data = node;
    new_node->next = NULL;
    if (q->rear) q->rear->next = new_node;
    q->rear = new_node;
    if (!q->front) q->front = new_node;
    q->size++;
    return 1;
}

struct Node* queue_dequeue(struct Queue* q) {
    if (queue_empty(q)) return NULL;
    struct QueueNode* temp = q->front;
    struct Node* data = temp->data;
    q->front = q->front->next;
    if (!q->front) q->rear = NULL;
    free(temp);
    q->size--;
    return data;
}

void graph_init(struct Graph* g) {
    g->node_count = 0;
    g->edge_count = 0;
}

int graph_add_node(struct Graph* g, const char* label) {
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

int graph_add_edge(struct Graph* g, int from_id, int to_id, int weight) {
    if (from_id < 0 || from_id >= g->node_count) return 0;
    if (to_id < 0 || to_id >= g->node_count) return 0;
    if (g->edge_count >= MAX_EDGES) return 0;
    struct Edge* edge = &g->edges[g->edge_count];
    edge->from = &g->nodes[from_id];
    edge->to = &g->nodes[to_id];
    edge->weight = weight;
    g->edge_count++;
    return 1;
}

void graph_reset_visited(struct Graph* g) {
    for (int i = 0; i < g->node_count; i++) {
        g->nodes[i].visited = 0;
        g->nodes[i].distance = INT_MAX;
    }
}

void graph_bfs(struct Graph* g, int start_id) {
    if (start_id < 0 || start_id >= g->node_count) return;
    graph_reset_visited(g);
    struct Queue q;
    queue_init(&q);
    g->nodes[start_id].visited = 1;
    g->nodes[start_id].distance = 0;
    queue_enqueue(&q, &g->nodes[start_id]);
    while (!queue_empty(&q)) {
        struct Node* current = queue_dequeue(&q);
        printf("Visited: %s (distance: %d)\n", current->label, current->distance);
        for (int i = 0; i < g->edge_count; i++) {
            if (g->edges[i].from->id == current->id && !g->edges[i].to->visited) {
                g->edges[i].to->visited = 1;
                g->edges[i].to->distance = current->distance + 1;
                queue_enqueue(&q, g->edges[i].to);
            }
        }
    }
}

void graph_dijkstra(struct Graph* g, int start_id) {
    if (start_id < 0 || start_id >= g->node_count) return;
    graph_reset_visited(g);
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
        if (min_index == -1) break;
        g->nodes[min_index].visited = 1;
        for (int i = 0; i < g->edge_count; i++) {
            if (g->edges[i].from