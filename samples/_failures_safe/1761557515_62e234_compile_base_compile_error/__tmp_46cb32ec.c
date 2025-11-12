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

struct Graph graph_create(int capacity) {
    struct Graph g;
    g.nodes = malloc(sizeof(struct GraphNode) * capacity);
    if (g.nodes == NULL) {
        exit(EXIT_FAILURE);
    }
    g.node_count = 0;
    g.capacity = capacity;
    for (int i = 0; i < capacity; i++) {
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
        free(g->nodes[i].edges);
    }
    free(g->nodes);
}

int graph_add_node(struct Graph *g, int id) {
    if (g == NULL || id < 0) return 0;
    if (g->node_count >= g->capacity) return 0;
    for (int i = 0; i < g->node_count; i++) {
        if (g->nodes[i].id == id) return 0;
    }
    g->nodes[g->node_count].id = id;
    g->nodes[g->node_count].visited = 0;
    g->nodes[g->node_count].edges = NULL;
    g->nodes[g->node_count].edge_count = 0;
    g->node_count++;
    return 1;
}

int graph_add_edge(struct Graph *g, int from_id, int to_id, int weight) {
    if (g == NULL || weight < 0) return 0;
    struct GraphNode *from_node = NULL;
    struct GraphNode *to_node = NULL;
    for (int i = 0; i < g->node_count; i++) {
        if (g->nodes[i].id == from_id) from_node = &g->nodes[i];
        if (g->nodes[i].id == to_id) to_node = &g->nodes[i];
    }
    if (from_node == NULL || to_node == NULL) return 0;
    struct GraphEdge *new_edges = realloc(from_node->edges, sizeof(struct GraphEdge) * (from_node->edge_count + 1));
    if (new_edges == NULL) return 0;
    from_node->edges = new_edges;
    from_node->edges[from_node->edge_count].target = to_node;
    from_node->edges[from_node->edge_count].weight = weight;
    from_node->edge_count++;
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

struct GraphNode *queue_dequeue(struct Queue *q) {
    if (q == NULL || q->front == NULL) return NULL;
    struct QueueNode *temp = q->front;
    struct GraphNode *data = temp->data;
    q->front = q->front->next;
    if (q->front == NULL) {
        q->rear = NULL;
    }
    free(temp);
    q->size--;
    return data;
}

void graph_reset_visited(struct Graph *g) {
    if (g == NULL) return;
    for (int i = 0; i < g->node_count; i++) {
        g->nodes[i].visited = 0;
    }
}

void graph_bfs(struct Graph *g, int start_id) {
    if (g == NULL) return;
    graph_reset_visited(g);
    struct GraphNode *start_node = NULL;
    for (int i = 0; i < g->node_count; i++) {
        if (g->nodes[i].id == start_id) {
            start_node = &g->nodes[i];
            break;
        }
    }
    if (start_node