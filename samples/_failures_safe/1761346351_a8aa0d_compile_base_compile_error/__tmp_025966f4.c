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
    int source;
    int destination;
    int weight;
};

struct Graph {
    struct GraphNode nodes[MAX_NODES];
    struct GraphEdge edges[MAX_EDGES];
    int node_count;
    int edge_count;
};

struct QueueNode {
    int data;
    struct QueueNode* next;
};

struct Queue {
    struct QueueNode* front;
    struct QueueNode* rear;
    int size;
};

void initialize_graph(struct Graph* graph) {
    graph->node_count = 0;
    graph->edge_count = 0;
    for (int i = 0; i < MAX_NODES; i++) {
        graph->nodes[i].id = -1;
        graph->nodes[i].visited = 0;
        graph->nodes[i].distance = INT_MAX;
    }
}

int add_node(struct Graph* graph, int id) {
    if (graph->node_count >= MAX_NODES || id < 0 || id >= MAX_NODES) {
        return -1;
    }
    for (int i = 0; i < graph->node_count; i++) {
        if (graph->nodes[i].id == id) {
            return -1;
        }
    }
    graph->nodes[graph->node_count].id = id;
    graph->nodes[graph->node_count].visited = 0;
    graph->nodes[graph->node_count].distance = INT_MAX;
    graph->node_count++;
    return 0;
}

int add_edge(struct Graph* graph, int source, int destination, int weight) {
    if (graph->edge_count >= MAX_EDGES) {
        return -1;
    }
    if (source < 0 || source >= MAX_NODES || destination < 0 || destination >= MAX_NODES) {
        return -1;
    }
    if (weight < 0) {
        return -1;
    }
    int source_exists = 0;
    int dest_exists = 0;
    for (int i = 0; i < graph->node_count; i++) {
        if (graph->nodes[i].id == source) source_exists = 1;
        if (graph->nodes[i].id == destination) dest_exists = 1;
    }
    if (!source_exists || !dest_exists) {
        return -1;
    }
    graph->edges[graph->edge_count].source = source;
    graph->edges[graph->edge_count].destination = destination;
    graph->edges[graph->edge_count].weight = weight;
    graph->edge_count++;
    return 0;
}

struct Queue* create_queue() {
    struct Queue* queue = (struct Queue*)malloc(sizeof(struct Queue));
    if (queue == NULL) return NULL;
    queue->front = NULL;
    queue->rear = NULL;
    queue->size = 0;
    return queue;
}

int enqueue(struct Queue* queue, int data) {
    if (queue == NULL || queue->size >= MAX_NODES) return -1;
    struct QueueNode* new_node = (struct QueueNode*)malloc(sizeof(struct QueueNode));
    if (new_node == NULL) return -1;
    new_node->data = data;
    new_node->next = NULL;
    if (queue->rear == NULL) {
        queue->front = new_node;
        queue->rear = new_node;
    } else {
        queue->rear->next = new_node;
        queue->rear = new_node;
    }
    queue->size++;
    return 0;
}

int dequeue(struct Queue* queue) {
    if (queue == NULL || queue->front == NULL) return -1;
    struct QueueNode* temp = queue->front;
    int data = temp->data;
    queue->front = queue->front->next;
    if (queue->front == NULL) {
        queue->rear = NULL;
    }
    free(temp);
    queue->size--;
    return data;
}

int is_queue_empty(struct Queue* queue) {
    if (queue == NULL) return 1;
    return queue->front == NULL;
}

void free_queue(struct Queue* queue) {
    if (queue == NULL) return;
    while (!is_queue_empty(queue)) {
        dequeue(queue);
    }
    free(queue);
}

void bfs(struct Graph* graph, int start_node) {
    if (graph == NULL) return;
    int start_index = -1;
    for (int i = 0; i < graph->node_count; i++) {
        graph->nodes[i].visited = 0;
        graph->nodes[i].distance = INT_MAX;
        if (graph->nodes[i].id == start_node) {
            start_index = i;
        }
    }
    if (start_index == -1) return;
    graph->nodes[start_index].distance = 0;
    graph->nodes[start_index].visited = 1;
    struct Queue* queue = create_queue();
    if (queue == NULL) return;
    if (enqueue(queue, start_node) != 0) {
        free_queue(queue);
        return;
    }
    while (!is_queue_empty(queue)) {
        int current = dequeue(queue);
        if (current == -1) break;
        int current_index = -1;
        for (int i = 0; i < graph->node_count; i++) {
            if (graph->nodes