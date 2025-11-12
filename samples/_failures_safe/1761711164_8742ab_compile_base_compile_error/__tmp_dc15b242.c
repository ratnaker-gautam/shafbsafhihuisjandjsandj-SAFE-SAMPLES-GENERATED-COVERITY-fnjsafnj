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
    struct GraphNode *current = graph->nodes;
    while (current != NULL) {
        if (current->id == id) return current;
        current = current->next;
    }
    return NULL;
}

struct Graph *create_graph(void) {
    struct Graph *graph = malloc(sizeof(struct Graph));
    if (graph == NULL) return NULL;
    graph->node_count = 0;
    graph->edge_count = 0;
    graph->nodes = NULL;
    graph->edges = NULL;
    return graph;
}

int add_node(struct Graph *graph, int id) {
    if (graph == NULL || id < 0) return 0;
    if (find_node_by_id(graph, id) != NULL) return 0;
    struct GraphNode *new_node = malloc(sizeof(struct GraphNode));
    if (new_node == NULL) return 0;
    new_node->id = id;
    new_node->visited = 0;
    new_node->distance = INT_MAX;
    new_node->next = graph->nodes;
    graph->nodes = new_node;
    graph->node_count++;
    return 1;
}

int add_edge(struct Graph *graph, int source_id, int target_id, int weight) {
    if (graph == NULL || weight < 0) return 0;
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

struct Queue *create_queue(void) {
    struct Queue *queue = malloc(sizeof(struct Queue));
    if (queue == NULL) return NULL;
    queue->front = NULL;
    queue->rear = NULL;
    queue->size = 0;
    return queue;
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
    if (queue->front == NULL) queue->rear = NULL;
    free(temp);
    queue->size--;
    return data;
}

void reset_graph(struct Graph *graph) {
    if (graph == NULL) return;
    struct GraphNode *current = graph->nodes;
    while (current != NULL) {
        current->visited = 0;
        current->distance = INT_MAX;
        current = current->next;
    }
}

void bfs_shortest_path(struct Graph *graph, int start_id) {
    if (graph == NULL) return;
    reset_graph(graph);
    struct GraphNode *start = find_node_by_id(graph, start_id);
    if (start == NULL) return;
    start->distance = 0;
    struct Queue *queue = create_queue();
    if (queue == NULL) return;
    enqueue(queue, start);
    while (queue->size > 0) {
        struct GraphNode *current = dequeue(queue);
        if (current == NULL) continue;
        struct GraphEdge *edge = graph->edges;
        while (edge != NULL) {
            if (edge->source == current && edge->target->visited == 0) {
                int new_distance = current->distance + 1;
                if (new_distance < edge->target->distance) {
                    edge->target->distance = new_distance;
                    enqueue(queue, edge->target);
                }
            }
            edge = edge->next;
        }
        current->visited = 1;
    }
    free(queue);
}

void print_shortest_paths(struct Graph *graph, int start_id) {
    if (graph == NULL) return;
    bfs_shortest_path(graph, start_id);
    printf("Shortest paths from node