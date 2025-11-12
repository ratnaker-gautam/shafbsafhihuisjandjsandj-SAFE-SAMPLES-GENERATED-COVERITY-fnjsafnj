//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

struct GraphNode {
    int id;
    int visited;
    struct EdgeList *edges;
};

struct Edge {
    struct GraphNode *dest;
    int weight;
};

struct EdgeList {
    struct Edge *edges;
    int count;
    int capacity;
};

struct Graph {
    struct GraphNode **nodes;
    int node_count;
    int node_capacity;
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

struct Graph *graph_create(void) {
    struct Graph *g = malloc(sizeof(struct Graph));
    if (!g) return NULL;
    g->node_count = 0;
    g->node_capacity = 8;
    g->nodes = malloc(g->node_capacity * sizeof(struct GraphNode *));
    if (!g->nodes) {
        free(g);
        return NULL;
    }
    return g;
}

void graph_destroy(struct Graph *g) {
    if (!g) return;
    for (int i = 0; i < g->node_count; i++) {
        if (g->nodes[i]) {
            if (g->nodes[i]->edges) {
                if (g->nodes[i]->edges->edges) {
                    free(g->nodes[i]->edges->edges);
                }
                free(g->nodes[i]->edges);
            }
            free(g->nodes[i]);
        }
    }
    free(g->nodes);
    free(g);
}

struct GraphNode *graph_add_node(struct Graph *g, int id) {
    if (!g || id < 0) return NULL;
    for (int i = 0; i < g->node_count; i++) {
        if (g->nodes[i] && g->nodes[i]->id == id) return NULL;
    }
    if (g->node_count >= g->node_capacity) {
        int new_cap = g->node_capacity * 2;
        if (new_cap <= g->node_capacity) return NULL;
        struct GraphNode **new_nodes = realloc(g->nodes, new_cap * sizeof(struct GraphNode *));
        if (!new_nodes) return NULL;
        g->nodes = new_nodes;
        g->node_capacity = new_cap;
    }
    struct GraphNode *node = malloc(sizeof(struct GraphNode));
    if (!node) return NULL;
    node->id = id;
    node->visited = 0;
    node->edges = malloc(sizeof(struct EdgeList));
    if (!node->edges) {
        free(node);
        return NULL;
    }
    node->edges->count = 0;
    node->edges->capacity = 4;
    node->edges->edges = malloc(node->edges->capacity * sizeof(struct Edge));
    if (!node->edges->edges) {
        free(node->edges);
        free(node);
        return NULL;
    }
    g->nodes[g->node_count] = node;
    g->node_count++;
    return node;
}

int graph_add_edge(struct Graph *g, int src_id, int dest_id, int weight) {
    if (!g || weight < 0) return 0;
    struct GraphNode *src = NULL, *dest = NULL;
    for (int i = 0; i < g->node_count; i++) {
        if (g->nodes[i]->id == src_id) src = g->nodes[i];
        if (g->nodes[i]->id == dest_id) dest = g->nodes[i];
    }
    if (!src || !dest) return 0;
    if (src->edges->count >= src->edges->capacity) {
        int new_cap = src->edges->capacity * 2;
        if (new_cap <= src->edges->capacity) return 0;
        struct Edge *new_edges = realloc(src->edges->edges, new_cap * sizeof(struct Edge));
        if (!new_edges) return 0;
        src->edges->edges = new_edges;
        src->edges->capacity = new_cap;
    }
    src->edges->edges[src->edges->count].dest = dest;
    src->edges->edges[src->edges->count].weight = weight;
    src->edges->count++;
    return 1;
}

struct Queue *queue_create(void) {
    struct Queue *q = malloc(sizeof(struct Queue));
    if (!q) return NULL;
    q->front = NULL;
    q->rear = NULL;
    q->size = 0;
    return q;
}

void queue_destroy(struct Queue *q) {
    if (!q) return;
    struct QueueNode *current = q->front;
    while (current) {
        struct QueueNode *next = current->next;
        free(current);
        current = next;
    }
    free(q);
}

int queue_enqueue(struct Queue *q, struct GraphNode *node) {
    if (!q || !node) return 0;
    struct QueueNode *new_node = malloc(sizeof(struct QueueNode));
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

struct GraphNode *queue_dequeue(struct Queue *q) {
    if (!q || !q->