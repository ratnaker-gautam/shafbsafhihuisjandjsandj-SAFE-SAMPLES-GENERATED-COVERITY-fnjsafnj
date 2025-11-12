//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: graph_algorithms
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

struct Graph graph_create(int initial_capacity) {
    struct Graph g;
    if (initial_capacity <= 0) initial_capacity = 1;
    g.nodes = malloc(sizeof(struct GraphNode) * initial_capacity);
    if (g.nodes == NULL) {
        g.node_count = 0;
        g.capacity = 0;
        return g;
    }
    g.node_count = 0;
    g.capacity = initial_capacity;
    for (int i = 0; i < initial_capacity; i++) {
        g.nodes[i].id = -1;
        g.nodes[i].visited = 0;
        g.nodes[i].edges = NULL;
        g.nodes[i].edge_count = 0;
    }
    return g;
}

void graph_destroy(struct Graph *g) {
    if (g == NULL) return;
    for (int i = 0; i < g->node_count; i++) {
        if (g->nodes[i].edges != NULL) {
            free(g->nodes[i].edges);
        }
    }
    free(g->nodes);
    g->nodes = NULL;
    g->node_count = 0;
    g->capacity = 0;
}

int graph_add_node(struct Graph *g, int id) {
    if (g == NULL || id < 0) return 0;
    for (int i = 0; i < g->node_count; i++) {
        if (g->nodes[i].id == id) return 0;
    }
    if (g->node_count >= g->capacity) {
        int new_capacity = g->capacity * 2;
        if (new_capacity <= g->capacity) return 0;
        struct GraphNode *new_nodes = realloc(g->nodes, sizeof(struct GraphNode) * new_capacity);
        if (new_nodes == NULL) return 0;
        g->nodes = new_nodes;
        g->capacity = new_capacity;
        for (int i = g->node_count; i < new_capacity; i++) {
            g->nodes[i].id = -1;
            g->nodes[i].visited = 0;
            g->nodes[i].edges = NULL;
            g->nodes[i].edge_count = 0;
        }
    }
    g->nodes[g->node_count].id = id;
    g->nodes[g->node_count].visited = 0;
    g->nodes[g->node_count].edges = NULL;
    g->nodes[g->node_count].edge_count = 0;
    g->node_count++;
    return 1;
}

int graph_add_edge(struct Graph *g, int from_id, int to_id, int weight) {
    if (g == NULL || from_id < 0 || to_id < 0) return 0;
    struct GraphNode *from_node = NULL;
    struct GraphNode *to_node = NULL;
    for (int i = 0; i < g->node_count; i++) {
        if (g->nodes[i].id == from_id) from_node = &g->nodes[i];
        if (g->nodes[i].id == to_id) to_node = &g->nodes[i];
    }
    if (from_node == NULL || to_node == NULL) return 0;
    int new_count = from_node->edge_count + 1;
    struct GraphEdge *new_edges = realloc(from_node->edges, sizeof(struct GraphEdge) * new_count);
    if (new_edges == NULL) return 0;
    from_node->edges = new_edges;
    from_node->edges[from_node->edge_count].target = to_node;
    from_node->edges[from_node->edge_count].weight = weight;
    from_node->edge_count++;
    return 1;
}

struct Queue queue_create() {
    struct Queue q;
    q.front = NULL;
    q.rear = NULL;
    q.size = 0;
    return q;
}

void queue_destroy(struct Queue *q) {
    if (q == NULL) return;
    struct QueueNode *current = q->front;
    while (current != NULL) {
        struct QueueNode *next = current->next;
        free(current);
        current = next;
    }
    q->front = NULL;
    q->rear = NULL;
    q->size = 0;
}

int queue_enqueue(struct Queue *q, struct GraphNode *data) {
    if (q == NULL || data == NULL) return 0;
    struct QueueNode *new_node = malloc(sizeof(struct QueueNode));
    if (new_node == NULL) return 0;
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

struct GraphNode *queue_dequeue(struct Queue *q