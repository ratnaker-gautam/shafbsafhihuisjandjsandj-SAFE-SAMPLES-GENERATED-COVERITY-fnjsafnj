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

struct Graph {
    int node_count;
    struct GraphNode **adjacency_list;
};

struct QueueNode {
    struct GraphNode *graph_node;
    struct QueueNode *next;
};

struct Queue {
    struct QueueNode *front;
    struct QueueNode *rear;
};

struct Graph *graph_create(int node_count) {
    if (node_count <= 0 || node_count > 1000) {
        return NULL;
    }
    struct Graph *graph = malloc(sizeof(struct Graph));
    if (!graph) {
        return NULL;
    }
    graph->node_count = node_count;
    graph->adjacency_list = malloc(node_count * sizeof(struct GraphNode *));
    if (!graph->adjacency_list) {
        free(graph);
        return NULL;
    }
    for (int i = 0; i < node_count; i++) {
        graph->adjacency_list[i] = NULL;
    }
    return graph;
}

void graph_destroy(struct Graph *graph) {
    if (!graph) {
        return;
    }
    for (int i = 0; i < graph->node_count; i++) {
        struct GraphNode *current = graph->adjacency_list[i];
        while (current) {
            struct GraphNode *temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(graph->adjacency_list);
    free(graph);
}

int graph_add_edge(struct Graph *graph, int source, int destination) {
    if (!graph || source < 0 || source >= graph->node_count || destination < 0 || destination >= graph->node_count) {
        return 0;
    }
    struct GraphNode *new_node = malloc(sizeof(struct GraphNode));
    if (!new_node) {
        return 0;
    }
    new_node->id = destination;
    new_node->visited = 0;
    new_node->distance = INT_MAX;
    new_node->next = graph->adjacency_list[source];
    graph->adjacency_list[source] = new_node;
    return 1;
}

struct Queue *queue_create(void) {
    struct Queue *queue = malloc(sizeof(struct Queue));
    if (!queue) {
        return NULL;
    }
    queue->front = NULL;
    queue->rear = NULL;
    return queue;
}

void queue_destroy(struct Queue *queue) {
    if (!queue) {
        return;
    }
    struct QueueNode *current = queue->front;
    while (current) {
        struct QueueNode *temp = current;
        current = current->next;
        free(temp);
    }
    free(queue);
}

int queue_enqueue(struct Queue *queue, struct GraphNode *graph_node) {
    if (!queue || !graph_node) {
        return 0;
    }
    struct QueueNode *new_node = malloc(sizeof(struct QueueNode));
    if (!new_node) {
        return 0;
    }
    new_node->graph_node = graph_node;
    new_node->next = NULL;
    if (queue->rear) {
        queue->rear->next = new_node;
    } else {
        queue->front = new_node;
    }
    queue->rear = new_node;
    return 1;
}

struct GraphNode *queue_dequeue(struct Queue *queue) {
    if (!queue || !queue->front) {
        return NULL;
    }
    struct QueueNode *temp = queue->front;
    struct GraphNode *graph_node = temp->graph_node;
    queue->front = queue->front->next;
    if (!queue->front) {
        queue->rear = NULL;
    }
    free(temp);
    return graph_node;
}

int queue_is_empty(struct Queue *queue) {
    return queue ? (queue->front == NULL) : 1;
}

void graph_reset(struct Graph *graph) {
    if (!graph) {
        return;
    }
    for (int i = 0; i < graph->node_count; i++) {
        struct GraphNode *current = graph->adjacency_list[i];
        while (current) {
            current->visited = 0;
            current->distance = INT_MAX;
            current = current->next;
        }
    }
}

void graph_bfs(struct Graph *graph, int start_id) {
    if (!graph || start_id < 0 || start_id >= graph->node_count) {
        return;
    }
    graph_reset(graph);
    struct Queue *queue = queue_create();
    if (!queue) {
        return;
    }
    struct GraphNode *start_node = graph->adjacency_list[start_id];
    if (!start_node) {
        queue_destroy(queue);
        return;
    }
    start_node->visited = 1;
    start_node->distance = 0;
    queue_enqueue(queue, start_node);
    while (!queue_is_empty(queue)) {
        struct GraphNode *current = queue_dequeue(queue);
        if (!current) {
            continue;
        }
        printf("Node %d: distance %d\n", current->id, current->distance);
        struct GraphNode *neighbor = graph->adjacency_list[current->id];
        while (neighbor) {
            if (!neighbor->visited) {
                neighbor->visited = 1;
                neighbor->distance = current->distance + 1;
                queue_enqueue(queue, neighbor);
            }
            neighbor = neighbor->next;
        }
    }
    queue_destroy(queue);
}

int main(void) {
    struct Graph *graph = graph_create