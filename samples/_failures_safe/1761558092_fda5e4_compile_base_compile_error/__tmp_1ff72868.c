//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_NODES 100
#define MAX_EDGES 1000

struct GraphNode {
    int id;
    int visited;
    int distance;
    struct GraphEdge *edges;
    int edge_count;
};

struct GraphEdge {
    struct GraphNode *target;
    int weight;
};

struct Graph {
    struct GraphNode nodes[MAX_NODES];
    int node_count;
    int edge_count;
};

struct QueueNode {
    struct GraphNode *node;
    struct QueueNode *next;
};

struct Queue {
    struct QueueNode *front;
    struct QueueNode *rear;
    int size;
};

void queue_init(struct Queue *q) {
    q->front = NULL;
    q->rear = NULL;
    q->size = 0;
}

int queue_empty(struct Queue *q) {
    return q->size == 0;
}

void queue_enqueue(struct Queue *q, struct GraphNode *node) {
    if (q->size >= MAX_NODES) return;
    
    struct QueueNode *new_node = malloc(sizeof(struct QueueNode));
    if (!new_node) return;
    
    new_node->node = node;
    new_node->next = NULL;
    
    if (q->rear) {
        q->rear->next = new_node;
    } else {
        q->front = new_node;
    }
    q->rear = new_node;
    q->size++;
}

struct GraphNode *queue_dequeue(struct Queue *q) {
    if (queue_empty(q)) return NULL;
    
    struct QueueNode *temp = q->front;
    struct GraphNode *node = temp->node;
    
    q->front = q->front->next;
    if (!q->front) {
        q->rear = NULL;
    }
    
    free(temp);
    q->size--;
    return node;
}

void graph_init(struct Graph *g) {
    g->node_count = 0;
    g->edge_count = 0;
    for (int i = 0; i < MAX_NODES; i++) {
        g->nodes[i].id = i;
        g->nodes[i].visited = 0;
        g->nodes[i].distance = INT_MAX;
        g->nodes[i].edges = NULL;
        g->nodes[i].edge_count = 0;
    }
}

struct GraphNode *graph_add_node(struct Graph *g) {
    if (g->node_count >= MAX_NODES) return NULL;
    g->node_count++;
    return &g->nodes[g->node_count - 1];
}

int graph_add_edge(struct Graph *g, struct GraphNode *from, struct GraphNode *to, int weight) {
    if (!from || !to || g->edge_count >= MAX_EDGES) return 0;
    if (from->edge_count >= MAX_EDGES || to->edge_count >= MAX_EDGES) return 0;
    
    struct GraphEdge *new_edge = malloc(sizeof(struct GraphEdge));
    if (!new_edge) return 0;
    
    new_edge->target = to;
    new_edge->weight = weight;
    
    struct GraphEdge *temp = realloc(from->edges, (from->edge_count + 1) * sizeof(struct GraphEdge));
    if (!temp) {
        free(new_edge);
        return 0;
    }
    
    from->edges = temp;
    from->edges[from->edge_count] = *new_edge;
    from->edge_count++;
    g->edge_count++;
    free(new_edge);
    return 1;
}

void graph_bfs(struct Graph *g, struct GraphNode *start) {
    if (!g || !start) return;
    
    for (int i = 0; i < g->node_count; i++) {
        g->nodes[i].visited = 0;
        g->nodes[i].distance = INT_MAX;
    }
    
    struct Queue q;
    queue_init(&q);
    
    start->visited = 1;
    start->distance = 0;
    queue_enqueue(&q, start);
    
    while (!queue_empty(&q)) {
        struct GraphNode *current = queue_dequeue(&q);
        if (!current) continue;
        
        for (int i = 0; i < current->edge_count; i++) {
            struct GraphNode *neighbor = current->edges[i].target;
            if (!neighbor->visited) {
                neighbor->visited = 1;
                neighbor->distance = current->distance + 1;
                queue_enqueue(&q, neighbor);
            }
        }
    }
    
    while (!queue_empty(&q)) {
        queue_dequeue(&q);
    }
}

void graph_print_distances(struct Graph *g) {
    if (!g) return;
    
    printf("Node distances from start:\n");
    for (int i = 0; i < g->node_count; i++) {
        if (g->nodes[i].distance == INT_MAX) {
            printf("Node %d: unreachable\n", g->nodes[i].id);
        } else {
            printf("Node %d: distance %d\n", g->nodes[i].id, g->nodes[i].distance);
        }
    }
}

void graph_cleanup(struct Graph *g) {
    if (!g) return;
    
    for (int i = 0; i < g->node_count; i++) {
        if (g->nodes[i].edges) {
            free(g->nodes[i].edges);
            g->nodes[i].edges = NULL;