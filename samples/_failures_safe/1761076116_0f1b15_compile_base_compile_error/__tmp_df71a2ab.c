//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: graph_algorithms
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
    struct GraphNode* source;
    struct GraphNode* destination;
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

int graph_add_edge(struct Graph* g, int src_id, int dest_id, int weight) {
    if (g->edge_count >= MAX_EDGES || weight < 0) return 0;
    struct GraphNode* src = NULL;
    struct GraphNode* dest = NULL;
    for (int i = 0; i < g->node_count; i++) {
        if (g->nodes[i].id == src_id) src = &g->nodes[i];
        if (g->nodes[i].id == dest_id) dest = &g->nodes[i];
    }
    if (!src || !dest) return 0;
    g->edges[g->edge_count].source = src;
    g->edges[g->edge_count].destination = dest;
    g->edges[g->edge_count].weight = weight;
    g->edge_count++;
    return 1;
}

void graph_reset(struct Graph* g) {
    for (int i = 0; i < g->node_count; i++) {
        g->nodes[i].visited = 0;
        g->nodes[i].distance = INT_MAX;
    }
}

void graph_bfs(struct Graph* g, int start_id) {
    graph_reset(g);
    struct GraphNode* start = NULL;
    for (int i = 0; i < g->node_count; i++) {
        if (g->nodes[i].id == start_id) {
            start = &g->nodes[i];
            break;
        }
    }
    if (!start) return;
    struct Queue q;
    queue_init(&q);
    start->distance = 0;
    start->visited = 1;
    queue_enqueue(&q, start);
    while (!queue_empty(&q)) {
        struct GraphNode* current = queue_dequeue(&q);
        for (int i = 0; i < g->edge_count; i++) {
            if (g->edges[i].source == current && !g->edges[i].destination->visited) {
                g->edges[i].destination->visited = 1;
                g->edges[i].destination->distance = current->distance + 1;
                queue_enqueue(&q, g->edges[i].destination);
            }
        }
    }
}

void graph_print_distances(struct Graph* g) {
    for (int i = 0; i < g->node_count; i++) {
        if (g->nodes[i].distance == INT_MAX) {
            printf("Node %d: unreachable\n", g->nodes[i].id);
        } else {
            printf("Node %d: distance %d\n", g->nodes[i].id, g->nodes[i].distance);
        }
    }
}

int main() {
    struct Graph g;
    graph_init(&g);
    printf("Adding nodes 0 through 5\n");
    for (int i = 0; i < 6;