//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_NODES 100
#define MAX_EDGES 1000

struct Node {
    int id;
    char label[32];
    int visited;
    int distance;
};

struct Edge {
    struct Node* source;
    struct Node* target;
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
    } else {
        q->front = new_node;
    }
    q->rear = new_node;
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

int add_node(struct Graph* g, int id, const char* label) {
    if (g->node_count >= MAX_NODES) return 0;
    if (id < 0 || id >= MAX_NODES) return 0;
    if (!label || strlen(label) >= 32) return 0;
    
    for (int i = 0; i < g->node_count; i++) {
        if (g->nodes[i].id == id) return 0;
    }
    
    struct Node* node = &g->nodes[g->node_count];
    node->id = id;
    strncpy(node->label, label, 31);
    node->label[31] = '\0';
    node->visited = 0;
    node->distance = INT_MAX;
    
    g->node_count++;
    return 1;
}

int add_edge(struct Graph* g, int source_id, int target_id, int weight) {
    if (g->edge_count >= MAX_EDGES) return 0;
    if (weight < 0) return 0;
    
    struct Node* source = NULL;
    struct Node* target = NULL;
    
    for (int i = 0; i < g->node_count; i++) {
        if (g->nodes[i].id == source_id) source = &g->nodes[i];
        if (g->nodes[i].id == target_id) target = &g->nodes[i];
    }
    
    if (!source || !target) return 0;
    
    struct Edge* edge = &g->edges[g->edge_count];
    edge->source = source;
    edge->target = target;
    edge->weight = weight;
    
    g->edge_count++;
    return 1;
}

struct Node* find_node_by_id(struct Graph* g, int id) {
    for (int i = 0; i < g->node_count; i++) {
        if (g->nodes[i].id == id) {
            return &g->nodes[i];
        }
    }
    return NULL;
}

void bfs(struct Graph* g, int start_id) {
    struct Node* start = find_node_by_id(g, start_id);
    if (!start) return;
    
    for (int i = 0; i < g->node_count; i++) {
        g->nodes[i].visited = 0;
        g->nodes[i].distance = INT_MAX;
    }
    
    start->visited = 1;
    start->distance = 0;
    
    struct Queue q;
    init_queue(&q);
    enqueue(&q, start);
    
    while (!is_queue_empty(&q)) {
        struct Node* current = dequeue(&q);
        printf("Visited node %d (%s) at distance %d\n", 
               current->id, current->label, current->distance);
        
        for (int i = 0; i < g->edge_count; i++) {
            if (g->edges[i].source->id == current->id && 
                !g->edges[i].target->visited) {
                g->edges[i].target->visited = 1;
                g->edges[i].target->distance = current->distance + 1;
                enqueue(&q, g->edges[i].target);
            }
        }
    }
}

void dijkstra(struct Graph* g, int start_id) {
    struct Node* start = find_node_by_id(g, start_id);
    if (!start) return;
    
    for (int i = 0; i