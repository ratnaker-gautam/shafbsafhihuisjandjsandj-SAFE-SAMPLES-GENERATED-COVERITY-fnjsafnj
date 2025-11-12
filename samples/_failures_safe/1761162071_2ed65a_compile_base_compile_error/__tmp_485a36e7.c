//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

struct GraphNode {
    int id;
    int visited;
    struct GraphEdge *edges;
    int edge_count;
    int capacity;
};

struct GraphEdge {
    struct GraphNode *target;
    int weight;
};

struct Graph {
    struct GraphNode *nodes;
    int node_count;
    int capacity;
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

struct Graph *graph_create(int initial_capacity) {
    if (initial_capacity <= 0) return NULL;
    struct Graph *g = malloc(sizeof(struct Graph));
    if (!g) return NULL;
    g->nodes = malloc(sizeof(struct GraphNode) * initial_capacity);
    if (!g->nodes) {
        free(g);
        return NULL;
    }
    g->node_count = 0;
    g->capacity = initial_capacity;
    return g;
}

void graph_destroy(struct Graph *g) {
    if (!g) return;
    for (int i = 0; i < g->node_count; i++) {
        if (g->nodes[i].edges) free(g->nodes[i].edges);
    }
    free(g->nodes);
    free(g);
}

struct GraphNode *graph_add_node(struct Graph *g, int id) {
    if (!g || id < 0) return NULL;
    for (int i = 0; i < g->node_count; i++) {
        if (g->nodes[i].id == id) return NULL;
    }
    if (g->node_count >= g->capacity) {
        int new_cap = g->capacity * 2;
        if (new_cap <= g->capacity) return NULL;
        struct GraphNode *new_nodes = realloc(g->nodes, sizeof(struct GraphNode) * new_cap);
        if (!new_nodes) return NULL;
        g->nodes = new_nodes;
        g->capacity = new_cap;
    }
    struct GraphNode *node = &g->nodes[g->node_count];
    node->id = id;
    node->visited = 0;
    node->edges = NULL;
    node->edge_count = 0;
    node->capacity = 0;
    g->node_count++;
    return node;
}

int graph_add_edge(struct Graph *g, int from_id, int to_id, int weight) {
    if (!g || weight < 0) return 0;
    struct GraphNode *from = NULL, *to = NULL;
    for (int i = 0; i < g->node_count; i++) {
        if (g->nodes[i].id == from_id) from = &g->nodes[i];
        if (g->nodes[i].id == to_id) to = &g->nodes[i];
    }
    if (!from || !to) return 0;
    if (from->edge_count >= from->capacity) {
        int new_cap = from->capacity ? from->capacity * 2 : 4;
        if (new_cap <= from->capacity) return 0;
        struct GraphEdge *new_edges = realloc(from->edges, sizeof(struct GraphEdge) * new_cap);
        if (!new_edges) return 0;
        from->edges = new_edges;
        from->capacity = new_cap;
    }
    from->edges[from->edge_count].target = to;
    from->edges[from->edge_count].weight = weight;
    from->edge_count++;
    return 1;
}

struct Queue *queue_create() {
    struct Queue *q = malloc(sizeof(struct Queue));
    if (!q) return NULL;
    q->front = q->rear = NULL;
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

int queue_enqueue(struct Queue *q, struct GraphNode *data) {
    if (!q || !data) return 0;
    struct QueueNode *new_node = malloc(sizeof(struct QueueNode));
    if (!new_node) return 0;
    new_node->data = data;
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

void graph_reset_visited(struct Graph *g) {
    if (!g) return;
    for (int i = 0; i < g->node_count; i++) {
        g->nodes[i].visited = 0;
    }
}

void bfs(struct Graph *g, int start_id) {
    if (!g) return;
    graph_reset_visited(g);
    struct GraphNode *start = NULL;
    for (int i = 0; i < g