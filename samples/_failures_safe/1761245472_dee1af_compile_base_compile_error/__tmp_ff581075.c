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

void initialize_queue(struct Queue *q) {
    if (q == NULL) return;
    q->front = NULL;
    q->rear = NULL;
    q->size = 0;
}

int is_queue_empty(struct Queue *q) {
    if (q == NULL) return 1;
    return q->size == 0;
}

void enqueue(struct Queue *q, struct GraphNode *node) {
    if (q == NULL || node == NULL) return;
    struct QueueNode *new_node = malloc(sizeof(struct QueueNode));
    if (new_node == NULL) return;
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
}

struct GraphNode *dequeue(struct Queue *q) {
    if (q == NULL || q->front == NULL) return NULL;
    struct QueueNode *temp = q->front;
    struct GraphNode *node = temp->data;
    q->front = q->front->next;
    if (q->front == NULL) {
        q->rear = NULL;
    }
    free(temp);
    q->size--;
    return node;
}

void bfs_shortest_path(struct Graph *graph, int start_id, int end_id) {
    if (graph == NULL || graph->nodes == NULL) return;
    struct GraphNode *start = find_node_by_id(graph, start_id);
    struct GraphNode *end = find_node_by_id(graph, end_id);
    if (start == NULL || end == NULL) return;

    for (int i = 0; i < graph->node_count; i++) {
        graph->nodes[i].visited = 0;
        graph->nodes[i].distance = -1;
        graph->nodes[i].next = NULL;
    }

    start->visited = 1;
    start->distance = 0;
    struct Queue q;
    initialize_queue(&q);
    enqueue(&q, start);

    while (!is_queue_empty(&q)) {
        struct GraphNode *current = dequeue(&q);
        if (current == NULL) continue;

        struct GraphEdge *edge = graph->edges;
        while (edge != NULL) {
            if (edge->source == current && !edge->target->visited) {
                edge->target->visited = 1;
                edge->target->distance = current->distance + 1;
                enqueue(&q, edge->target);
                if (edge->target == end) {
                    while (!is_queue_empty(&q)) dequeue(&q);
                    return;
                }
            }
            edge = edge->next;
        }
    }
}

struct Graph *create_sample_graph(void) {
    struct Graph *graph = malloc(sizeof(struct Graph));
    if (graph == NULL) return NULL;

    graph->node_count = 6;
    graph->edge_count = 7;
    graph->nodes = malloc(sizeof(struct GraphNode) * 6);
    graph->edges = NULL;

    if (graph->nodes == NULL) {
        free(graph);
        return NULL;
    }

    for (int i = 0; i < 6; i++) {
        graph->nodes[i].id = i + 1;
        graph->nodes[i].visited = 0;
        graph->nodes[i].distance = -1;
        graph->nodes[i].next = NULL;
    }

    int edges[7][3] = {{1,2,1}, {1,3,1}, {2,4,1}, {3,4,1}, {3,5,1}, {4,6,1}, {5,6,1}};
    
    for (int i = 0; i < 7; i++) {
        struct GraphEdge *new_edge = malloc(sizeof(struct GraphEdge));
        if (new_edge == NULL) continue;
        new_edge->source = find_node_by_id(graph, edges[i][0]);
        new_edge->target = find_node_by_id(graph, edges[i][1]);
        new_edge->weight = edges[i][2];
        new_edge->next = graph->edges;
        graph->edges = new_edge;
    }

    return graph;
}

void free_graph(struct Graph *graph) {
    if (graph == NULL) return;
    if (graph->nodes != NULL) free(graph->nodes);
    struct GraphEdge *edge = graph->edges