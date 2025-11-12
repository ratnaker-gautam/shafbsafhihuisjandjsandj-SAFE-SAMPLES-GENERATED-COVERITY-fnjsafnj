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
    int size;
};

struct Graph *graph_create(void) {
    struct Graph *g = malloc(sizeof(struct Graph));
    if (g == NULL) return NULL;
    g->nodes = NULL;
    g->node_count = 0;
    return g;
}

struct GraphNode *graph_add_node(struct Graph *g, int id) {
    if (g == NULL || id < 0) return NULL;
    struct GraphNode *current = g->nodes;
    while (current != NULL) {
        if (current->id == id) return NULL;
        current = current->next;
    }
    struct GraphNode *new_node = malloc(sizeof(struct GraphNode));
    if (new_node == NULL) return NULL;
    new_node->id = id;
    new_node->visited = 0;
    new_node->edges = NULL;
    new_node->next = g->nodes;
    g->nodes = new_node;
    g->node_count++;
    return new_node;
}

int graph_add_edge(struct Graph *g, int from_id, int to_id, int weight) {
    if (g == NULL || weight < 0) return 0;
    struct GraphNode *from_node = NULL;
    struct GraphNode *to_node = NULL;
    struct GraphNode *current = g->nodes;
    while (current != NULL) {
        if (current->id == from_id) from_node = current;
        if (current->id == to_id) to_node = current;
        current = current->next;
    }
    if (from_node == NULL || to_node == NULL) return 0;
    struct GraphEdge *new_edge = malloc(sizeof(struct GraphEdge));
    if (new_edge == NULL) return 0;
    new_edge->target = to_node;
    new_edge->weight = weight;
    new_edge->next = from_node->edges;
    from_node->edges = new_edge;
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
    q->size = 0;
    return q;
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
    if (q->front == NULL) q->rear = NULL;
    free(temp);
    q->size--;
    return data;
}

void queue_destroy(struct Queue *q) {
    if (q == NULL) return;
    while (q->front != NULL) {
        struct QueueNode *temp = q->front;
        q->front = q->front->next;
        free(temp);
    }
    free(q);
}

void graph_dfs_util(struct GraphNode *node) {
    if (node == NULL || node->visited) return;
    node->visited = 1;
    printf("%d ", node->id);
    struct GraphEdge *edge = node->edges;
    while (edge != NULL) {
        graph_dfs_util(edge->target);
        edge = edge->next;
    }
}

void graph_dfs(struct Graph *g, int start_id) {
    if (g == NULL) return;
    struct GraphNode *start = NULL;
    struct GraphNode *current = g->nodes;
    while (current != NULL) {
        if (current->id == start_id) {
            start = current;
            break;
        }
        current = current->next;
    }
    if (start == NULL) return;
    graph_reset_visited(g);
    printf("DFS traversal from node %d: ", start_id);
    graph_dfs_util(start);
    printf("\n");
}

void graph_bfs(struct Graph *g, int start_id) {
    if (g == NULL) return;
    struct GraphNode *start = NULL;
    struct GraphNode *current = g->nodes;
    while (current != NULL) {
        if (