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
    struct GraphNode *next;
};

struct GraphEdge {
    struct GraphNode *target;
    int weight;
    struct GraphEdge *next;
};

struct Graph {
    struct GraphNode *nodes;
    int node_count;
};

struct QueueNode {
    struct GraphNode *data;
    struct QueueNode *next;
};

struct Queue {
    struct QueueNode *front;
    struct QueueNode *rear;
};

struct Graph *graph_create(void) {
    struct Graph *g = malloc(sizeof(struct Graph));
    if (g == NULL) return NULL;
    g->nodes = NULL;
    g->node_count = 0;
    return g;
}

struct GraphNode *graph_find_node(struct Graph *g, int id) {
    if (g == NULL) return NULL;
    struct GraphNode *current = g->nodes;
    while (current != NULL) {
        if (current->id == id) return current;
        current = current->next;
    }
    return NULL;
}

int graph_add_node(struct Graph *g, int id) {
    if (g == NULL || id < 0) return 0;
    if (graph_find_node(g, id) != NULL) return 0;
    struct GraphNode *new_node = malloc(sizeof(struct GraphNode));
    if (new_node == NULL) return 0;
    new_node->id = id;
    new_node->visited = 0;
    new_node->edges = NULL;
    new_node->next = g->nodes;
    g->nodes = new_node;
    g->node_count++;
    return 1;
}

int graph_add_edge(struct Graph *g, int from_id, int to_id, int weight) {
    if (g == NULL || weight < 0) return 0;
    struct GraphNode *from = graph_find_node(g, from_id);
    struct GraphNode *to = graph_find_node(g, to_id);
    if (from == NULL || to == NULL) return 0;
    struct GraphEdge *new_edge = malloc(sizeof(struct GraphEdge));
    if (new_edge == NULL) return 0;
    new_edge->target = to;
    new_edge->weight = weight;
    new_edge->next = from->edges;
    from->edges = new_edge;
    return 1;
}

void graph_reset_visited(struct Graph *g) {
    if (g == NULL) return;
    struct GraphNode *current = g->nodes;
    while (current != NULL) {
        current->visited = 0;
        current = current->next;
    }
}

struct Queue *queue_create(void) {
    struct Queue *q = malloc(sizeof(struct Queue));
    if (q == NULL) return NULL;
    q->front = NULL;
    q->rear = NULL;
    return q;
}

int queue_enqueue(struct Queue *q, struct GraphNode *node) {
    if (q == NULL || node == NULL) return 0;
    struct QueueNode *new_node = malloc(sizeof(struct QueueNode));
    if (new_node == NULL) return 0;
    new_node->data = node;
    new_node->next = NULL;
    if (q->rear == NULL) {
        q->front = new_node;
        q->rear = new_node;
    } else {
        q->rear->next = new_node;
        q->rear = new_node;
    }
    return 1;
}

struct GraphNode *queue_dequeue(struct Queue *q) {
    if (q == NULL || q->front == NULL) return NULL;
    struct QueueNode *temp = q->front;
    struct GraphNode *data = temp->data;
    q->front = q->front->next;
    if (q->front == NULL) q->rear = NULL;
    free(temp);
    return data;
}

int queue_is_empty(struct Queue *q) {
    if (q == NULL) return 1;
    return q->front == NULL;
}

void queue_destroy(struct Queue *q) {
    if (q == NULL) return;
    while (!queue_is_empty(q)) {
        queue_dequeue(q);
    }
    free(q);
}

void bfs(struct Graph *g, int start_id) {
    if (g == NULL) return;
    graph_reset_visited(g);
    struct GraphNode *start = graph_find_node(g, start_id);
    if (start == NULL) return;
    struct Queue *q = queue_create();
    if (q == NULL) return;
    start->visited = 1;
    queue_enqueue(q, start);
    printf("BFS traversal: ");
    while (!queue_is_empty(q)) {
        struct GraphNode *current = queue_dequeue(q);
        printf("%d ", current->id);
        struct GraphEdge *edge = current->edges;
        while (edge != NULL) {
            if (!edge->target->visited) {
                edge->target->visited = 1;
                queue_enqueue(q, edge->target);
            }
            edge = edge->next;
        }
    }
    printf("\n");
    queue_destroy(q);
}

void dfs_util(struct GraphNode *node) {
    if (node == NULL || node->visited) return;
    node->visited = 1;
    printf("%d ", node->id);
    struct GraphEdge *edge = node->edges;
    while (edge != NULL) {
        dfs_util(edge->target);
        edge = edge->next;
    }
}