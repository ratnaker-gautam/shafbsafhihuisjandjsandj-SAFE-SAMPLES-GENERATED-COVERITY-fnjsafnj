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
    if (graph == NULL || id < 0) return NULL;
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
    if (graph == NULL || weight < 0) return 0;
    if (source_id < 0 || source_id >= graph->node_count) return 0;
    if (target_id < 0 || target_id >= graph->node_count) return 0;
    struct GraphEdge *new_edge = malloc(sizeof(struct GraphEdge));
    if (new_edge == NULL) return 0;
    new_edge->source = &graph->nodes[source_id];
    new_edge->target = &graph->nodes[target_id];
    new_edge->weight = weight;
    new_edge->next = graph->edges;
    graph->edges = new_edge;
    graph->edge_count++;
    return 1;
}

void init_queue(struct Queue *queue) {
    if (queue == NULL) return;
    queue->front = NULL;
    queue->rear = NULL;
    queue->size = 0;
}

int enqueue(struct Queue *queue, struct GraphNode *node) {
    if (queue == NULL || node == NULL) return 0;
    struct QueueNode *new_node = malloc(sizeof(struct QueueNode));
    if (new_node == NULL) return 0;
    new_node->data = node;
    new_node->next = NULL;
    if (queue->rear == NULL) {
        queue->front = new_node;
        queue->rear = new_node;
    } else {
        queue->rear->next = new_node;
        queue->rear = new_node;
    }
    queue->size++;
    return 1;
}

struct GraphNode *dequeue(struct Queue *queue) {
    if (queue == NULL || queue->front == NULL) return NULL;
    struct QueueNode *temp = queue->front;
    struct GraphNode *data = temp->data;
    queue->front = queue->front->next;
    if (queue->front == NULL) {
        queue->rear = NULL;
    }
    free(temp);
    queue->size--;
    return data;
}

int is_queue_empty(struct Queue *queue) {
    if (queue == NULL) return 1;
    return queue->front == NULL;
}

void bfs(struct Graph *graph, int start_id) {
    if (graph == NULL) return;
    if (start_id < 0 || start_id >= graph->node_count) return;
    for (int i = 0; i < graph->node_count; i++) {
        graph->nodes[i].visited = 0;
        graph->nodes[i].distance = INT_MAX;
    }
    struct GraphNode *start = &graph->nodes[start_id];
    start->visited = 1;
    start->distance = 0;
    struct Queue queue;
    init_queue(&queue);
    enqueue(&queue, start);
    while (!is_queue_empty(&queue)) {
        struct GraphNode *current = dequeue(&queue);
        struct GraphEdge *edge = graph->edges;
        while (edge != NULL) {
            if (edge->source == current && !edge->target->visited) {
                edge->target->visited = 1;
                edge->target->distance = current->distance + 1;
                enqueue(&queue, edge->target);
            }
            edge = edge->next;
        }
    }
}

void print_distances(struct Graph *graph) {
    if (graph == NULL) return;
    for (int i = 0; i < graph->node_count; i++) {
        if (graph->nodes[i].distance == INT_MAX) {
            printf("Node %d: unreachable\n", graph->nodes[i].id);
        } else {
            printf("Node %d: distance %d\n", graph->nodes