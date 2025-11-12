//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

struct Node {
    int id;
    int visited;
    struct Edge* edges;
    struct Node* next;
};

struct Edge {
    struct Node* target;
    int weight;
    struct Edge* next;
};

struct Graph {
    struct Node* nodes;
    int node_count;
};

struct QueueNode {
    struct Node* data;
    struct QueueNode* next;
};

struct Queue {
    struct QueueNode* front;
    struct QueueNode* rear;
};

struct Graph* create_graph() {
    struct Graph* graph = malloc(sizeof(struct Graph));
    if (graph == NULL) return NULL;
    graph->nodes = NULL;
    graph->node_count = 0;
    return graph;
}

struct Node* create_node(int id) {
    struct Node* node = malloc(sizeof(struct Node));
    if (node == NULL) return NULL;
    node->id = id;
    node->visited = 0;
    node->edges = NULL;
    node->next = NULL;
    return node;
}

struct Edge* create_edge(struct Node* target, int weight) {
    struct Edge* edge = malloc(sizeof(struct Edge));
    if (edge == NULL) return NULL;
    edge->target = target;
    edge->weight = weight;
    edge->next = NULL;
    return edge;
}

struct Queue* create_queue() {
    struct Queue* queue = malloc(sizeof(struct Queue));
    if (queue == NULL) return NULL;
    queue->front = NULL;
    queue->rear = NULL;
    return queue;
}

void enqueue(struct Queue* queue, struct Node* node) {
    if (queue == NULL || node == NULL) return;
    struct QueueNode* new_node = malloc(sizeof(struct QueueNode));
    if (new_node == NULL) return;
    new_node->data = node;
    new_node->next = NULL;
    if (queue->rear == NULL) {
        queue->front = new_node;
        queue->rear = new_node;
    } else {
        queue->rear->next = new_node;
        queue->rear = new_node;
    }
}

struct Node* dequeue(struct Queue* queue) {
    if (queue == NULL || queue->front == NULL) return NULL;
    struct QueueNode* temp = queue->front;
    struct Node* data = temp->data;
    queue->front = queue->front->next;
    if (queue->front == NULL) queue->rear = NULL;
    free(temp);
    return data;
}

int is_queue_empty(struct Queue* queue) {
    return queue == NULL || queue->front == NULL;
}

void add_edge(struct Graph* graph, int from_id, int to_id, int weight) {
    if (graph == NULL || weight < 0) return;
    struct Node* from_node = NULL;
    struct Node* to_node = NULL;
    struct Node* current = graph->nodes;
    while (current != NULL) {
        if (current->id == from_id) from_node = current;
        if (current->id == to_id) to_node = current;
        current = current->next;
    }
    if (from_node == NULL || to_node == NULL) return;
    struct Edge* new_edge = create_edge(to_node, weight);
    if (new_edge == NULL) return;
    new_edge->next = from_node->edges;
    from_node->edges = new_edge;
}

void add_node(struct Graph* graph, int id) {
    if (graph == NULL) return;
    struct Node* current = graph->nodes;
    while (current != NULL) {
        if (current->id == id) return;
        current = current->next;
    }
    struct Node* new_node = create_node(id);
    if (new_node == NULL) return;
    new_node->next = graph->nodes;
    graph->nodes = new_node;
    graph->node_count++;
}

void reset_visited(struct Graph* graph) {
    if (graph == NULL) return;
    struct Node* current = graph->nodes;
    while (current != NULL) {
        current->visited = 0;
        current = current->next;
    }
}

void bfs(struct Graph* graph, int start_id) {
    if (graph == NULL) return;
    reset_visited(graph);
    struct Node* start_node = NULL;
    struct Node* current = graph->nodes;
    while (current != NULL) {
        if (current->id == start_id) {
            start_node = current;
            break;
        }
        current = current->next;
    }
    if (start_node == NULL) return;
    struct Queue* queue = create_queue();
    if (queue == NULL) return;
    enqueue(queue, start_node);
    start_node->visited = 1;
    printf("BFS traversal: ");
    while (!is_queue_empty(queue)) {
        struct Node* node = dequeue(queue);
        printf("%d ", node->id);
        struct Edge* edge = node->edges;
        while (edge != NULL) {
            if (edge->target->visited == 0) {
                edge->target->visited = 1;
                enqueue(queue, edge->target);
            }
            edge = edge->next;
        }
    }
    printf("\n");
    free(queue);
}

void dfs_util(struct Node* node) {
    if (node == NULL || node->visited) return;
    node->visited = 1;
    printf("%d ", node->id);
    struct Edge* edge = node->edges;
    while (edge != NULL) {
        dfs_util(edge->target);
        edge = edge->next;
    }
}