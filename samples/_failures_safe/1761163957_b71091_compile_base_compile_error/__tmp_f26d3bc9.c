//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 100
#define MAX_EDGES 1000

struct Node {
    int id;
    int visited;
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
    struct Node* data;
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

void enqueue(struct Queue* q, struct Node* node) {
    if (q->size >= MAX_NODES) return;
    struct QueueNode* new_node = malloc(sizeof(struct QueueNode));
    if (!new_node) return;
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
}

struct Node* dequeue(struct Queue* q) {
    if (is_queue_empty(q)) return NULL;
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

void init_graph(struct Graph* g) {
    g->node_count = 0;
    g->edge_count = 0;
}

int add_node(struct Graph* g, int id) {
    if (g->node_count >= MAX_NODES) return 0;
    if (id < 0 || id >= MAX_NODES) return 0;
    for (int i = 0; i < g->node_count; i++) {
        if (g->nodes[i].id == id) return 0;
    }
    g->nodes[g->node_count].id = id;
    g->nodes[g->node_count].visited = 0;
    g->node_count++;
    return 1;
}

int add_edge(struct Graph* g, int from_id, int to_id, int weight) {
    if (g->edge_count >= MAX_EDGES) return 0;
    if (weight < 0) return 0;
    struct Node* from_node = NULL;
    struct Node* to_node = NULL;
    for (int i = 0; i < g->node_count; i++) {
        if (g->nodes[i].id == from_id) from_node = &g->nodes[i];
        if (g->nodes[i].id == to_id) to_node = &g->nodes[i];
    }
    if (!from_node || !to_node) return 0;
    g->edges[g->edge_count].from = from_node;
    g->edges[g->edge_count].to = to_node;
    g->edges[g->edge_count].weight = weight;
    g->edge_count++;
    return 1;
}

void reset_visited(struct Graph* g) {
    for (int i = 0; i < g->node_count; i++) {
        g->nodes[i].visited = 0;
    }
}

void bfs(struct Graph* g, int start_id) {
    reset_visited(g);
    struct Node* start_node = NULL;
    for (int i = 0; i < g->node_count; i++) {
        if (g->nodes[i].id == start_id) {
            start_node = &g->nodes[i];
            break;
        }
    }
    if (!start_node) return;
    struct Queue q;
    init_queue(&q);
    start_node->visited = 1;
    enqueue(&q, start_node);
    printf("BFS traversal: ");
    while (!is_queue_empty(&q)) {
        struct Node* current = dequeue(&q);
        printf("%d ", current->id);
        for (int i = 0; i < g->edge_count; i++) {
            if (g->edges[i].from == current && !g->edges[i].to->visited) {
                g->edges[i].to->visited = 1;
                enqueue(&q, g->edges[i].to);
            }
        }
    }
    printf("\n");
    while (!is_queue_empty(&q)) dequeue(&q);
}

void dfs_util(struct Graph* g, struct Node* node) {
    node->visited = 1;
    printf("%d ", node->id);
    for (int i = 0; i < g->edge_count; i++) {
        if (g->edges[i].from == node && !g->edges[i].to->visited) {
            dfs_util(g, g->edges[i].to);
        }
    }
}

void dfs(struct Graph* g, int start_id) {
    reset_visited(g);
    struct Node* start_node = NULL;
    for (int i = 0; i < g->node_count; i++) {
        if (g->nodes[i].id == start_id) {
            start_node = &g->nodes[i