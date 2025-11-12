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
    if (q->rear) {
        q->rear->next = new_node;
    }
    q->rear = new_node;
    if (!q->front) {
        q->front = new_node;
    }
    q->size++;
    return 1;
}

struct Node* queue_dequeue(struct Queue* q) {
    if (queue_empty(q)) return NULL;
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

int graph_add_edge(struct Graph* g, int src_id, int tgt_id, int weight) {
    if (src_id < 0 || src_id >= g->node_count) return 0;
    if (tgt_id < 0 || tgt_id >= g->node_count) return 0;
    if (g->edge_count >= MAX_EDGES) return 0;
    if (weight < 0) return 0;
    struct Edge* edge = &g->edges[g->edge_count];
    edge->source = &g->nodes[src_id];
    edge->target = &g->nodes[tgt_id];
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
    struct Node* start = &g->nodes[start_id];
    start->visited = 1;
    start->distance = 0;
    queue_enqueue(&q, start);
    while (!queue_empty(&q)) {
        struct Node* current = queue_dequeue(&q);
        printf("Visited: %s (distance: %d)\n", current->label, current->distance);
        for (int i = 0; i < g->edge_count; i++) {
            if (g->edges[i].source == current && !g->edges[i].target->visited) {
                g->edges[i].target->visited = 1;
                g->edges[i].target->distance = current->distance + 1;
                queue_enqueue(&q, g->edges[i].target);
            }
        }
    }
    while (!queue_empty(&q)) {
        queue_dequeue(&q);
    }
}

void graph_dfs_util(struct Node* node, struct Graph* g) {
    node->visited = 1;
    printf("Visited: %s\n", node->label);
    for (int i = 0; i < g->edge_count; i++) {
        if (g->edges[i].source == node && !g->edges[i].target->visited) {
            graph_dfs_util(g->edges[i].target, g);
        }
    }
}

void graph_dfs(struct Graph* g, int start_id) {
    if (start_id < 0 || start_id >= g->node_count) return;
    graph_reset_visited(g);
    graph_dfs_util(&g->nodes[start_id], g);
}

int main() {