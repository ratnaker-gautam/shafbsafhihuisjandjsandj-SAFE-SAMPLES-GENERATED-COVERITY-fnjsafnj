//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

struct GraphNode {
    int id;
    int visited;
    int distance;
    struct GraphNode *next;
};

struct GraphEdge {
    struct GraphNode *source;
    struct GraphNode *target;
    int weight;
    struct GraphEdge *next;
};

struct Graph {
    int node_count;
    int edge_count;
    struct GraphNode *nodes;
    struct GraphEdge *edges;
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

struct GraphNode *find_node_by_id(struct Graph *graph, int id) {
    if (graph == NULL || graph->nodes == NULL) return NULL;
    for (int i = 0; i < graph->node_count; i++) {
        if (graph->nodes[i].id == id) {
            return &graph->nodes[i];
        }
    }
    return NULL;
}

struct Graph *create_graph(int node_count) {
    if (node_count <= 0 || node_count > 1000) return NULL;
    struct Graph *graph = malloc(sizeof(struct Graph));
    if (graph == NULL) return NULL;
    graph->node_count = node_count;
    graph->edge_count = 0;
    graph->nodes = malloc(node_count * sizeof(struct GraphNode));
    if (graph->nodes == NULL) {
        free(graph);
        return NULL;
    }
    graph->edges = NULL;
    for (int i = 0; i < node_count; i++) {
        graph->nodes[i].id = i;
        graph->nodes[i].visited = 0;
        graph->nodes[i].distance = INT_MAX;
        graph->nodes[i].next = NULL;
    }
    return graph;
}

int add_edge(struct Graph *graph, int source_id, int target_id, int weight) {
    if (graph == NULL) return 0;
    if (weight < 0 || weight > 10000) return 0;
    struct GraphNode *source = find_node_by_id(graph, source_id);
    struct GraphNode *target = find_node_by_id(graph, target_id);
    if (source == NULL || target == NULL) return 0;
    struct GraphEdge *new_edge = malloc(sizeof(struct GraphEdge));
    if (new_edge == NULL) return 0;
    new_edge->source = source;
    new_edge->target = target;
    new_edge->weight = weight;
    new_edge->next = graph->edges;
    graph->edges = new_edge;
    graph->edge_count++;
    return 1;
}

void init_queue(struct Queue *q) {
    q->front = NULL;
    q->rear = NULL;
    q->size = 0;
}

int enqueue(struct Queue *q, struct GraphNode *node) {
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
    q->size++;
    return 1;
}

struct GraphNode *dequeue(struct Queue *q) {
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

int is_queue_empty(struct Queue *q) {
    return q == NULL || q->front == NULL;
}

void bfs_shortest_path(struct Graph *graph, int start_id) {
    if (graph == NULL) return;
    struct GraphNode *start = find_node_by_id(graph, start_id);
    if (start == NULL) return;
    for (int i = 0; i < graph->node_count; i++) {
        graph->nodes[i].visited = 0;
        graph->nodes[i].distance = INT_MAX;
    }
    start->distance = 0;
    start->visited = 1;
    struct Queue q;
    init_queue(&q);
    enqueue(&q, start);
    while (!is_queue_empty(&q)) {
        struct GraphNode *current = dequeue(&q);
        struct GraphEdge *edge = graph->edges;
        while (edge != NULL) {
            if (edge->source == current && !edge->target->visited) {
                edge->target->visited = 1;
                edge->target->distance = current->distance + 1;
                enqueue(&q, edge->target);
            }
            edge = edge->next;
        }
    }
    while (!is_queue_empty(&q)) {
        dequeue(&q);
    }
}

void print_distances(struct Graph *graph) {
    if (graph == NULL) return;
    for (int i = 0; i < graph->node_count; i++) {
        if (graph->nodes[i].distance == INT_MAX) {
            printf("Node %d: unreachable\n", graph->nodes[i].id);
        } else {
            printf("Node %d: distance %d