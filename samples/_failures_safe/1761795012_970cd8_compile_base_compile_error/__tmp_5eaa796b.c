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
};

struct Graph *graph_create(void) {
    struct Graph *graph = malloc(sizeof(struct Graph));
    if (graph == NULL) return NULL;
    graph->nodes = NULL;
    graph->node_count = 0;
    return graph;
}

struct GraphNode *graph_find_node(struct Graph *graph, int id) {
    if (graph == NULL) return NULL;
    struct GraphNode *current = graph->nodes;
    while (current != NULL) {
        if (current->id == id) return current;
        current = current->next;
    }
    return NULL;
}

int graph_add_node(struct Graph *graph, int id) {
    if (graph == NULL || id < 0) return 0;
    if (graph_find_node(graph, id) != NULL) return 0;
    struct GraphNode *new_node = malloc(sizeof(struct GraphNode));
    if (new_node == NULL) return 0;
    new_node->id = id;
    new_node->visited = 0;
    new_node->edges = NULL;
    new_node->next = graph->nodes;
    graph->nodes = new_node;
    graph->node_count++;
    return 1;
}

int graph_add_edge(struct Graph *graph, int from_id, int to_id, int weight) {
    if (graph == NULL || weight < 0) return 0;
    struct GraphNode *from_node = graph_find_node(graph, from_id);
    struct GraphNode *to_node = graph_find_node(graph, to_id);
    if (from_node == NULL || to_node == NULL) return 0;
    struct GraphEdge *new_edge = malloc(sizeof(struct GraphEdge));
    if (new_edge == NULL) return 0;
    new_edge->target = to_node;
    new_edge->weight = weight;
    new_edge->next = from_node->edges;
    from_node->edges = new_edge;
    return 1;
}

void graph_reset_visited(struct Graph *graph) {
    if (graph == NULL) return;
    struct GraphNode *current = graph->nodes;
    while (current != NULL) {
        current->visited = 0;
        current = current->next;
    }
}

struct Queue *queue_create(void) {
    struct Queue *queue = malloc(sizeof(struct Queue));
    if (queue == NULL) return NULL;
    queue->front = NULL;
    queue->rear = NULL;
    return queue;
}

int queue_enqueue(struct Queue *queue, struct GraphNode *data) {
    if (queue == NULL || data == NULL) return 0;
    struct QueueNode *new_node = malloc(sizeof(struct QueueNode));
    if (new_node == NULL) return 0;
    new_node->data = data;
    new_node->next = NULL;
    if (queue->rear == NULL) {
        queue->front = new_node;
        queue->rear = new_node;
    } else {
        queue->rear->next = new_node;
        queue->rear = new_node;
    }
    return 1;
}

struct GraphNode *queue_dequeue(struct Queue *queue) {
    if (queue == NULL || queue->front == NULL) return NULL;
    struct QueueNode *temp = queue->front;
    struct GraphNode *data = temp->data;
    queue->front = queue->front->next;
    if (queue->front == NULL) queue->rear = NULL;
    free(temp);
    return data;
}

int queue_is_empty(struct Queue *queue) {
    if (queue == NULL) return 1;
    return queue->front == NULL;
}

void queue_destroy(struct Queue *queue) {
    if (queue == NULL) return;
    while (!queue_is_empty(queue)) {
        queue_dequeue(queue);
    }
    free(queue);
}

void dfs_visit(struct GraphNode *node) {
    if (node == NULL || node->visited) return;
    node->visited = 1;
    printf("%d ", node->id);
    struct GraphEdge *edge = node->edges;
    while (edge != NULL) {
        dfs_visit(edge->target);
        edge = edge->next;
    }
}

void dfs_traversal(struct Graph *graph, int start_id) {
    if (graph == NULL) return;
    struct GraphNode *start_node = graph_find_node(graph, start_id);
    if (start_node == NULL) return;
    graph_reset_visited(graph);
    printf("DFS traversal from node %d: ", start_id);
    dfs_visit(start_node);
    printf("\n");
}

void bfs_traversal(struct Graph *graph, int start_id) {
    if (graph == NULL) return;
    struct GraphNode *start_node = graph_find_node(graph, start_id);
    if (start_node == NULL) return;
    graph_reset_visited(graph);
    struct Queue *queue = queue_create();
    if (queue == NULL) return;
    queue_enqueue(queue, start_node);
    start_node->visited = 1;
    printf("BFS traversal from node %d: ",