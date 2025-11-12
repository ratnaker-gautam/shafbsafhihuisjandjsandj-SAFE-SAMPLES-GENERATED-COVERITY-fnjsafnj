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
    int edge_count;
    int capacity;
};

struct GraphEdge {
    struct GraphNode *target;
    int weight;
};

struct Graph {
    struct GraphNode *nodes;
    int node_count;
    int capacity;
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

struct Graph *graph_create(int initial_capacity) {
    if (initial_capacity <= 0) {
        return NULL;
    }
    struct Graph *graph = malloc(sizeof(struct Graph));
    if (!graph) {
        return NULL;
    }
    graph->nodes = malloc(sizeof(struct GraphNode) * initial_capacity);
    if (!graph->nodes) {
        free(graph);
        return NULL;
    }
    graph->node_count = 0;
    graph->capacity = initial_capacity;
    return graph;
}

void graph_destroy(struct Graph *graph) {
    if (!graph) {
        return;
    }
    for (int i = 0; i < graph->node_count; i++) {
        if (graph->nodes[i].edges) {
            free(graph->nodes[i].edges);
        }
    }
    free(graph->nodes);
    free(graph);
}

struct GraphNode *graph_add_node(struct Graph *graph, int id) {
    if (!graph || id < 0) {
        return NULL;
    }
    if (graph->node_count >= graph->capacity) {
        int new_capacity = graph->capacity * 2;
        if (new_capacity <= graph->capacity) {
            return NULL;
        }
        struct GraphNode *new_nodes = realloc(graph->nodes, sizeof(struct GraphNode) * new_capacity);
        if (!new_nodes) {
            return NULL;
        }
        graph->nodes = new_nodes;
        graph->capacity = new_capacity;
    }
    for (int i = 0; i < graph->node_count; i++) {
        if (graph->nodes[i].id == id) {
            return NULL;
        }
    }
    struct GraphNode *node = &graph->nodes[graph->node_count];
    node->id = id;
    node->visited = 0;
    node->edges = NULL;
    node->edge_count = 0;
    node->capacity = 0;
    graph->node_count++;
    return node;
}

int graph_add_edge(struct Graph *graph, int from_id, int to_id, int weight) {
    if (!graph || weight < 0) {
        return 0;
    }
    struct GraphNode *from_node = NULL;
    struct GraphNode *to_node = NULL;
    for (int i = 0; i < graph->node_count; i++) {
        if (graph->nodes[i].id == from_id) {
            from_node = &graph->nodes[i];
        }
        if (graph->nodes[i].id == to_id) {
            to_node = &graph->nodes[i];
        }
    }
    if (!from_node || !to_node) {
        return 0;
    }
    if (from_node->edge_count >= from_node->capacity) {
        int new_capacity = from_node->capacity == 0 ? 4 : from_node->capacity * 2;
        if (new_capacity <= from_node->capacity) {
            return 0;
        }
        struct GraphEdge *new_edges = realloc(from_node->edges, sizeof(struct GraphEdge) * new_capacity);
        if (!new_edges) {
            return 0;
        }
        from_node->edges = new_edges;
        from_node->capacity = new_capacity;
    }
    from_node->edges[from_node->edge_count].target = to_node;
    from_node->edges[from_node->edge_count].weight = weight;
    from_node->edge_count++;
    return 1;
}

struct Queue *queue_create(void) {
    struct Queue *queue = malloc(sizeof(struct Queue));
    if (!queue) {
        return NULL;
    }
    queue->front = NULL;
    queue->rear = NULL;
    queue->size = 0;
    return queue;
}

void queue_destroy(struct Queue *queue) {
    if (!queue) {
        return;
    }
    struct QueueNode *current = queue->front;
    while (current) {
        struct QueueNode *next = current->next;
        free(current);
        current = next;
    }
    free(queue);
}

int queue_enqueue(struct Queue *queue, struct GraphNode *data) {
    if (!queue || !data) {
        return 0;
    }
    struct QueueNode *new_node = malloc(sizeof(struct QueueNode));
    if (!new_node) {
        return 0;
    }
    new_node->data = data;
    new_node->next = NULL;
    if (queue->rear) {
        queue->rear->next = new_node;
    } else {
        queue->front = new_node;
    }
    queue->rear = new_node;
    queue->size++;
    return 1;
}

struct GraphNode *queue_dequeue(struct Queue *queue) {
    if (!queue || !queue->front) {
        return NULL;
    }
    struct QueueNode *front_node = queue->front;
    struct GraphNode *data = front_node->data;
    queue->front = front_node->next;
    if (!queue->front) {
        queue->rear