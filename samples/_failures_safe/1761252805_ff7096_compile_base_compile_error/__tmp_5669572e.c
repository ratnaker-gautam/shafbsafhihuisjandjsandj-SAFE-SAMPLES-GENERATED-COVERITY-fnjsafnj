//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: graph_algorithms
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
};

struct Edge {
    struct Node* from;
    struct Node* to;
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

struct StackNode {
    struct Node* data;
    struct StackNode* next;
};

struct Stack {
    struct StackNode* top;
    int size;
};

void init_graph(struct Graph* graph) {
    if (graph == NULL) return;
    graph->node_count = 0;
    graph->edge_count = 0;
}

void init_queue(struct Queue* queue) {
    if (queue == NULL) return;
    queue->front = NULL;
    queue->rear = NULL;
    queue->size = 0;
}

void init_stack(struct Stack* stack) {
    if (stack == NULL) return;
    stack->top = NULL;
    stack->size = 0;
}

int push(struct Stack* stack, struct Node* node) {
    if (stack == NULL || node == NULL) return 0;
    struct StackNode* new_node = malloc(sizeof(struct StackNode));
    if (new_node == NULL) return 0;
    new_node->data = node;
    new_node->next = stack->top;
    stack->top = new_node;
    stack->size++;
    return 1;
}

struct Node* pop(struct Stack* stack) {
    if (stack == NULL || stack->top == NULL) return NULL;
    struct StackNode* temp = stack->top;
    struct Node* data = temp->data;
    stack->top = stack->top->next;
    free(temp);
    stack->size--;
    return data;
}

int is_stack_empty(struct Stack* stack) {
    if (stack == NULL) return 1;
    return stack->top == NULL;
}

int enqueue(struct Queue* queue, struct Node* node) {
    if (queue == NULL || node == NULL) return 0;
    struct QueueNode* new_node = malloc(sizeof(struct QueueNode));
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

struct Node* dequeue(struct Queue* queue) {
    if (queue == NULL || queue->front == NULL) return NULL;
    struct QueueNode* temp = queue->front;
    struct Node* data = temp->data;
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

int add_node(struct Graph* graph, int id, const char* label) {
    if (graph == NULL || label == NULL) return 0;
    if (graph->node_count >= MAX_NODES) return 0;
    if (id < 0 || id >= MAX_NODES) return 0;
    for (int i = 0; i < graph->node_count; i++) {
        if (graph->nodes[i].id == id) return 0;
    }
    struct Node* node = &graph->nodes[graph->node_count];
    node->id = id;
    strncpy(node->label, label, sizeof(node->label) - 1);
    node->label[sizeof(node->label) - 1] = '\0';
    node->visited = 0;
    graph->node_count++;
    return 1;
}

int add_edge(struct Graph* graph, int from_id, int to_id, int weight) {
    if (graph == NULL) return 0;
    if (graph->edge_count >= MAX_EDGES) return 0;
    if (weight < 0) return 0;
    struct Node* from_node = NULL;
    struct Node* to_node = NULL;
    for (int i = 0; i < graph->node_count; i++) {
        if (graph->nodes[i].id == from_id) from_node = &graph->nodes[i];
        if (graph->nodes[i].id == to_id) to_node = &graph->nodes[i];
    }
    if (from_node == NULL || to_node == NULL) return 0;
    struct Edge* edge = &graph->edges[graph->edge_count];
    edge->from = from_node;
    edge->to = to_node;
    edge->weight = weight;
    graph->edge_count++;
    return 1;
}

void reset_visited(struct Graph* graph) {
    if (graph == NULL) return;
    for (int i = 0; i < graph->node_count; i++) {
        graph->nodes[i].visited = 0;
    }
}

void dfs_util