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
    if (!g) return NULL;
    g->nodes = NULL;
    g->node_count = 0;
    return g;
}

struct GraphNode *graph_add_node(struct Graph *g, int id) {
    if (!g || id < 0) return NULL;
    struct GraphNode *new_node = malloc(sizeof(struct GraphNode));
    if (!new_node) return NULL;
    new_node->id = id;
    new_node->visited = 0;
    new_node->edges = NULL;
    new_node->next = g->nodes;
    g->nodes = new_node;
    g->node_count++;
    return new_node;
}

int graph_add_edge(struct Graph *g, int from_id, int to_id, int weight) {
    if (!g || weight < 0) return 0;
    struct GraphNode *from = NULL, *to = NULL;
    for (struct GraphNode *n = g->nodes; n; n = n->next) {
        if (n->id == from_id) from = n;
        if (n->id == to_id) to = n;
    }
    if (!from || !to) return 0;
    struct GraphEdge *new_edge = malloc(sizeof(struct GraphEdge));
    if (!new_edge) return 0;
    new_edge->target = to;
    new_edge->weight = weight;
    new_edge->next = from->edges;
    from->edges = new_edge;
    return 1;
}

struct GraphNode *graph_find_node(struct Graph *g, int id) {
    if (!g) return NULL;
    for (struct GraphNode *n = g->nodes; n; n = n->next) {
        if (n->id == id) return n;
    }
    return NULL;
}

void graph_reset_visited(struct Graph *g) {
    if (!g) return;
    for (struct GraphNode *n = g->nodes; n; n = n->next) {
        n->visited = 0;
    }
}

struct Queue *queue_create(void) {
    struct Queue *q = malloc(sizeof(struct Queue));
    if (!q) return NULL;
    q->front = q->rear = NULL;
    return q;
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
    return 1;
}

struct GraphNode *queue_dequeue(struct Queue *q) {
    if (!q || !q->front) return NULL;
    struct QueueNode *temp = q->front;
    struct GraphNode *data = temp->data;
    q->front = q->front->next;
    if (!q->front) q->rear = NULL;
    free(temp);
    return data;
}

int queue_is_empty(struct Queue *q) {
    return !q || !q->front;
}

void queue_destroy(struct Queue *q) {
    if (!q) return;
    while (!queue_is_empty(q)) {
        queue_dequeue(q);
    }
    free(q);
}

void bfs(struct Graph *g, int start_id) {
    if (!g) return;
    graph_reset_visited(g);
    struct GraphNode *start = graph_find_node(g, start_id);
    if (!start) return;
    struct Queue *q = queue_create();
    if (!q) return;
    start->visited = 1;
    queue_enqueue(q, start);
    printf("BFS traversal: ");
    while (!queue_is_empty(q)) {
        struct GraphNode *current = queue_dequeue(q);
        printf("%d ", current->id);
        for (struct GraphEdge *e = current->edges; e; e = e->next) {
            if (!e->target->visited) {
                e->target->visited = 1;
                queue_enqueue(q, e->target);
            }
        }
    }
    printf("\n");
    queue_destroy(q);
}

void dfs_util(struct GraphNode *node) {
    if (!node || node->visited) return;
    node->visited = 1;
    printf("%d ", node->id);
    for (struct GraphEdge *e = node->edges; e; e = e->next) {
        dfs_util(e->target);
    }
}

void dfs(struct Graph *g, int start_id) {
    if (!g) return;
    graph_reset_visited(g);
    struct GraphNode *start = graph_find_node(g, start_id);
    if (!start) return;
    printf("DFS traversal: ");
    dfs_util