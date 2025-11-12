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

struct Graph graph_create(int initial_capacity) {
    struct Graph g;
    g.nodes = NULL;
    g.node_count = 0;
    g.capacity = 0;
    if (initial_capacity > 0) {
        g.nodes = malloc(sizeof(struct GraphNode) * initial_capacity);
        if (g.nodes) {
            g.capacity = initial_capacity;
        }
    }
    return g;
}

void graph_destroy(struct Graph *g) {
    if (g) {
        for (int i = 0; i < g->node_count; i++) {
            if (g->nodes[i].edges) {
                free(g->nodes[i].edges);
            }
        }
        if (g->nodes) {
            free(g->nodes);
        }
        g->nodes = NULL;
        g->node_count = 0;
        g->capacity = 0;
    }
}

int graph_add_node(struct Graph *g, int id) {
    if (!g || id < 0) return 0;
    for (int i = 0; i < g->node_count; i++) {
        if (g->nodes[i].id == id) return 0;
    }
    if (g->node_count >= g->capacity) {
        int new_capacity = g->capacity == 0 ? 4 : g->capacity * 2;
        if (new_capacity <= g->capacity) return 0;
        struct GraphNode *new_nodes = realloc(g->nodes, sizeof(struct GraphNode) * new_capacity);
        if (!new_nodes) return 0;
        g->nodes = new_nodes;
        g->capacity = new_capacity;
    }
    g->nodes[g->node_count].id = id;
    g->nodes[g->node_count].visited = 0;
    g->nodes[g->node_count].edges = NULL;
    g->nodes[g->node_count].edge_count = 0;
    g->nodes[g->node_count].capacity = 0;
    g->node_count++;
    return 1;
}

struct GraphNode *graph_find_node(struct Graph *g, int id) {
    if (!g) return NULL;
    for (int i = 0; i < g->node_count; i++) {
        if (g->nodes[i].id == id) return &g->nodes[i];
    }
    return NULL;
}

int graph_add_edge(struct Graph *g, int from_id, int to_id, int weight) {
    if (!g || weight < 0) return 0;
    struct GraphNode *from = graph_find_node(g, from_id);
    struct GraphNode *to = graph_find_node(g, to_id);
    if (!from || !to) return 0;
    if (from->edge_count >= from->capacity) {
        int new_capacity = from->capacity == 0 ? 2 : from->capacity * 2;
        if (new_capacity <= from->capacity) return 0;
        struct GraphEdge *new_edges = realloc(from->edges, sizeof(struct GraphEdge) * new_capacity);
        if (!new_edges) return 0;
        from->edges = new_edges;
        from->capacity = new_capacity;
    }
    from->edges[from->edge_count].target = to;
    from->edges[from->edge_count].weight = weight;
    from->edge_count++;
    return 1;
}

struct Queue queue_create(void) {
    struct Queue q;
    q.front = NULL;
    q.rear = NULL;
    q.size = 0;
    return q;
}

void queue_destroy(struct Queue *q) {
    if (q) {
        while (q->front) {
            struct QueueNode *temp = q->front;
            q->front = q->front->next;
            free(temp);
        }
        q->rear = NULL;
        q->size = 0;
    }
}

int queue_enqueue(struct Queue *q, struct GraphNode *data) {
    if (!q || !data) return 0;
    struct QueueNode *new_node = malloc(sizeof(struct QueueNode));
    if (!new_node) return 0;
    new_node->data = data;
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
    if (!g