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
    struct GraphNode **nodes;
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
    graph->nodes = malloc(sizeof(struct GraphNode*) * initial_capacity);
    if (!graph->nodes) {
        free(graph);
        return NULL;
    }
    graph->node_count = 0;
    graph->capacity = initial_capacity;
    return graph;
}

struct GraphNode *graph_add_node(struct Graph *graph, int id) {
    if (!graph || id < 0) {
        return NULL;
    }
    for (int i = 0; i < graph->node_count; i++) {
        if (graph->nodes[i]->id == id) {
            return NULL;
        }
    }
    if (graph->node_count >= graph->capacity) {
        int new_capacity = graph->capacity * 2;
        if (new_capacity <= graph->capacity) {
            return NULL;
        }
        struct GraphNode **new_nodes = realloc(graph->nodes, sizeof(struct GraphNode*) * new_capacity);
        if (!new_nodes) {
            return NULL;
        }
        graph->nodes = new_nodes;
        graph->capacity = new_capacity;
    }
    struct GraphNode *node = malloc(sizeof(struct GraphNode));
    if (!node) {
        return NULL;
    }
    node->id = id;
    node->visited = 0;
    node->edges = NULL;
    node->edge_count = 0;
    node->capacity = 4;
    node->edges = malloc(sizeof(struct GraphEdge) * node->capacity);
    if (!node->edges) {
        free(node);
        return NULL;
    }
    graph->nodes[graph->node_count] = node;
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
        if (graph->nodes[i]->id == from_id) {
            from_node = graph->nodes[i];
        }
        if (graph->nodes[i]->id == to_id) {
            to_node = graph->nodes[i];
        }
    }
    if (!from_node || !to_node) {
        return 0;
    }
    if (from_node->edge_count >= from_node->capacity) {
        int new_capacity = from_node->capacity * 2;
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

struct Queue *queue_create() {
    struct Queue *queue = malloc(sizeof(struct Queue));
    if (!queue) {
        return NULL;
    }
    queue->front = NULL;
    queue->rear = NULL;
    queue->size = 0;
    return queue;
}

int queue_enqueue(struct Queue *queue, struct GraphNode *node) {
    if (!queue || !node) {
        return 0;
    }
    struct QueueNode *new_node = malloc(sizeof(struct QueueNode));
    if (!new_node) {
        return 0;
    }
    new_node->data = node;
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
    struct QueueNode *temp = queue->front;
    struct GraphNode *data = temp->data;
    queue->front = queue->front->next;
    if (!queue->front) {
        queue->rear = NULL;
    }
    free(temp);
    queue->size--;
    return data;
}

void graph_bfs(struct Graph *graph, int start_id) {
    if (!graph) {
        return;
    }
    for (int i = 0; i < graph->node_count; i++) {
        graph->nodes[i]->visited = 0;
    }
    struct GraphNode *start_node = NULL;
    for (int i = 0; i < graph->