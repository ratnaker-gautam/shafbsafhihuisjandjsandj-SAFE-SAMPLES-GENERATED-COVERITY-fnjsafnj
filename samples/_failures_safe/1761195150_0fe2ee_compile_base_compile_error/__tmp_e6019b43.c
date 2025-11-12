//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

struct GraphNode {
    int id;
    int visited;
    struct GraphNode** neighbors;
    int neighbor_count;
    int capacity;
};

struct Graph {
    struct GraphNode** nodes;
    int node_count;
    int capacity;
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

struct Graph* graph_create(int initial_capacity) {
    if (initial_capacity <= 0) {
        initial_capacity = 10;
    }
    struct Graph* graph = malloc(sizeof(struct Graph));
    if (!graph) return NULL;
    graph->nodes = malloc(sizeof(struct GraphNode*) * initial_capacity);
    if (!graph->nodes) {
        free(graph);
        return NULL;
    }
    graph->node_count = 0;
    graph->capacity = initial_capacity;
    return graph;
}

struct GraphNode* graph_create_node(int id) {
    struct GraphNode* node = malloc(sizeof(struct GraphNode));
    if (!node) return NULL;
    node->id = id;
    node->visited = 0;
    node->neighbor_count = 0;
    node->capacity = 5;
    node->neighbors = malloc(sizeof(struct GraphNode*) * node->capacity);
    if (!node->neighbors) {
        free(node);
        return NULL;
    }
    return node;
}

void graph_add_node(struct Graph* graph, struct GraphNode* node) {
    if (!graph || !node) return;
    if (graph->node_count >= graph->capacity) {
        int new_capacity = graph->capacity * 2;
        struct GraphNode** new_nodes = realloc(graph->nodes, sizeof(struct GraphNode*) * new_capacity);
        if (!new_nodes) return;
        graph->nodes = new_nodes;
        graph->capacity = new_capacity;
    }
    graph->nodes[graph->node_count++] = node;
}

void graph_add_edge(struct GraphNode* from, struct GraphNode* to) {
    if (!from || !to) return;
    if (from->neighbor_count >= from->capacity) {
        int new_capacity = from->capacity * 2;
        struct GraphNode** new_neighbors = realloc(from->neighbors, sizeof(struct GraphNode*) * new_capacity);
        if (!new_neighbors) return;
        from->neighbors = new_neighbors;
        from->capacity = new_capacity;
    }
    from->neighbors[from->neighbor_count++] = to;
}

struct Queue* queue_create() {
    struct Queue* queue = malloc(sizeof(struct Queue));
    if (!queue) return NULL;
    queue->front = NULL;
    queue->rear = NULL;
    queue->size = 0;
    return queue;
}

void queue_enqueue(struct Queue* queue, struct GraphNode* node) {
    if (!queue || !node) return;
    struct QueueNode* new_node = malloc(sizeof(struct QueueNode));
    if (!new_node) return;
    new_node->data = node;
    new_node->next = NULL;
    if (queue->rear) {
        queue->rear->next = new_node;
    } else {
        queue->front = new_node;
    }
    queue->rear = new_node;
    queue->size++;
}

struct GraphNode* queue_dequeue(struct Queue* queue) {
    if (!queue || !queue->front) return NULL;
    struct QueueNode* temp = queue->front;
    struct GraphNode* data = temp->data;
    queue->front = queue->front->next;
    if (!queue->front) {
        queue->rear = NULL;
    }
    free(temp);
    queue->size--;
    return data;
}

void queue_free(struct Queue* queue) {
    if (!queue) return;
    while (queue->front) {
        queue_dequeue(queue);
    }
    free(queue);
}

void graph_reset_visited(struct Graph* graph) {
    if (!graph) return;
    for (int i = 0; i < graph->node_count; i++) {
        if (graph->nodes[i]) {
            graph->nodes[i]->visited = 0;
        }
    }
}

void graph_bfs(struct Graph* graph, int start_id) {
    if (!graph || graph->node_count == 0) return;
    struct GraphNode* start = NULL;
    for (int i = 0; i < graph->node_count; i++) {
        if (graph->nodes[i] && graph->nodes[i]->id == start_id) {
            start = graph->nodes[i];
            break;
        }
    }
    if (!start) return;
    graph_reset_visited(graph);
    struct Queue* queue = queue_create();
    if (!queue) return;
    start->visited = 1;
    queue_enqueue(queue, start);
    printf("BFS traversal from node %d: ", start_id);
    while (queue->size > 0) {
        struct GraphNode* current = queue_dequeue(queue);
        if (!current) continue;
        printf("%d ", current->id);
        for (int i = 0; i < current->neighbor_count; i++) {
            struct GraphNode* neighbor = current->neighbors[i];
            if (neighbor && !neighbor->visited) {
                neighbor->visited = 1;
                queue_enqueue(queue, neighbor);
            }
        }
    }
    printf("\n");
    queue_free(queue);
}

void