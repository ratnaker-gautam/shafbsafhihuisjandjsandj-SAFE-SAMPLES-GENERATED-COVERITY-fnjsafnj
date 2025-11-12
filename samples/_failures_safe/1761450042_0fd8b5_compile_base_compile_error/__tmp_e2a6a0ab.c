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
    int data;
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

void enqueue(struct Queue* q, int value) {
    if (q->size >= MAX_NODES) return;
    struct QueueNode* new_node = malloc(sizeof(struct QueueNode));
    if (!new_node) return;
    new_node->data = value;
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

int dequeue(struct Queue* q) {
    if (is_queue_empty(q)) return -1;
    struct QueueNode* temp = q->front;
    int value = temp->data;
    q->front = q->front->next;
    if (!q->front) {
        q->rear = NULL;
    }
    free(temp);
    q->size--;
    return value;
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
    g->node_count++;
    return node->id;
}

int add_edge(struct Graph* g, int from_id, int to_id, int weight) {
    if (from_id < 0 || from_id >= g->node_count) return -1;
    if (to_id < 0 || to_id >= g->node_count) return -1;
    if (g->edge_count >= MAX_EDGES) return -1;
    if (weight < 0) return -1;
    struct Edge* edge = &g->edges[g->edge_count];
    edge->from = &g->nodes[from_id];
    edge->to = &g->nodes[to_id];
    edge->weight = weight;
    g->edge_count++;
    return 0;
}

void bfs(struct Graph* g, int start_id) {
    if (start_id < 0 || start_id >= g->node_count) return;
    int visited[MAX_NODES] = {0};
    struct Queue q;
    init_queue(&q);
    visited[start_id] = 1;
    enqueue(&q, start_id);
    printf("BFS traversal: ");
    while (!is_queue_empty(&q)) {
        int current = dequeue(&q);
        if (current == -1) break;
        printf("%s ", g->nodes[current].label);
        for (int i = 0; i < g->edge_count; i++) {
            if (g->edges[i].from->id == current && !visited[g->edges[i].to->id]) {
                visited[g->edges[i].to->id] = 1;
                enqueue(&q, g->edges[i].to->id);
            }
        }
    }
    printf("\n");
    while (!is_queue_empty(&q)) dequeue(&q);
}

void dijkstra(struct Graph* g, int start_id) {
    if (start_id < 0 || start_id >= g->node_count) return;
    int dist[MAX_NODES];
    int visited[MAX_NODES] = {0};
    for (int i = 0; i < g->node_count; i++) {
        dist[i] = INT_MAX;
    }
    dist[start_id] = 0;
    for (int count = 0; count < g->node_count - 1; count++) {
        int min_dist = INT_MAX;
        int min_index = -1;
        for (int i = 0; i < g->node_count; i++) {
            if (!visited[i] && dist[i] < min_dist) {
                min_dist = dist[i];
                min_index = i;
            }
        }
        if (min_index == -1) break;
        visited[min_index] = 1;
        for (int i = 0; i < g->edge_count; i++) {
            if (g->edges[i].from->id == min_index) {
                int to_id = g->edges[i].to->id;
                if (dist[min_index] < INT_MAX - g->edges[i].weight) {
                    int new_dist = dist[min_index] + g->edges[i].weight;