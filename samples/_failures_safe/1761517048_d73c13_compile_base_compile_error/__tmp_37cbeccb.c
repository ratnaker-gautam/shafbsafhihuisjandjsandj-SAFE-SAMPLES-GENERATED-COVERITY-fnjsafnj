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
};

struct GraphEdge {
    struct GraphNode* from;
    struct GraphNode* to;
    int weight;
};

struct Graph {
    struct GraphNode nodes[MAX_NODES];
    struct GraphEdge edges[MAX_EDGES];
    int node_count;
    int edge_count;
};

struct QueueNode {
    struct GraphNode* data;
    struct QueueNode* next;
};

struct Queue {
    struct QueueNode* front;
    struct QueueNode* rear;
    int size;
};

void queue_init(struct Queue* q) {
    q->front = NULL;
    q->rear = NULL;
    q->size = 0;
}

int queue_empty(struct Queue* q) {
    return q->size == 0;
}

int queue_enqueue(struct Queue* q, struct GraphNode* node) {
    if (q->size >= MAX_NODES) return 0;
    
    struct QueueNode* new_node = malloc(sizeof(struct QueueNode));
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

struct GraphNode* queue_dequeue(struct Queue* q) {
    if (queue_empty(q)) return NULL;
    
    struct QueueNode* temp = q->front;
    struct GraphNode* data = temp->data;
    
    q->front = q->front->next;
    if (!q->front) q->rear = NULL;
    
    free(temp);
    q->size--;
    return data;
}

void graph_init(struct Graph* g) {
    g->node_count = 0;
    g->edge_count = 0;
    for (int i = 0; i < MAX_NODES; i++) {
        g->nodes[i].id = -1;
        g->nodes[i].visited = 0;
        g->nodes[i].distance = INT_MAX;
    }
}

int graph_add_node(struct Graph* g, int id) {
    if (g->node_count >= MAX_NODES || id < 0 || id >= MAX_NODES) return 0;
    
    for (int i = 0; i < g->node_count; i++) {
        if (g->nodes[i].id == id) return 0;
    }
    
    g->nodes[g->node_count].id = id;
    g->nodes[g->node_count].visited = 0;
    g->nodes[g->node_count].distance = INT_MAX;
    g->node_count++;
    return 1;
}

int graph_add_edge(struct Graph* g, int from_id, int to_id, int weight) {
    if (g->edge_count >= MAX_EDGES || weight < 0) return 0;
    
    struct GraphNode* from_node = NULL;
    struct GraphNode* to_node = NULL;
    
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

void graph_reset_visited(struct Graph* g) {
    for (int i = 0; i < g->node_count; i++) {
        g->nodes[i].visited = 0;
        g->nodes[i].distance = INT_MAX;
    }
}

void bfs_shortest_path(struct Graph* g, int start_id) {
    if (g->node_count == 0) return;
    
    struct GraphNode* start_node = NULL;
    for (int i = 0; i < g->node_count; i++) {
        if (g->nodes[i].id == start_id) {
            start_node = &g->nodes[i];
            break;
        }
    }
    if (!start_node) return;
    
    graph_reset_visited(g);
    start_node->distance = 0;
    start_node->visited = 1;
    
    struct Queue q;
    queue_init(&q);
    queue_enqueue(&q, start_node);
    
    while (!queue_empty(&q)) {
        struct GraphNode* current = queue_dequeue(&q);
        
        for (int i = 0; i < g->edge_count; i++) {
            if (g->edges[i].from == current && !g->edges[i].to->visited) {
                g->edges[i].to->visited = 1;
                g->edges[i].to->distance = current->distance + 1;
                queue_enqueue(&q, g->edges[i].to);
            }
        }
    }
    
    while (!queue_empty(&q)) {
        queue_dequeue(&q);
    }
}

void print_shortest_paths(struct Graph* g, int start_id)