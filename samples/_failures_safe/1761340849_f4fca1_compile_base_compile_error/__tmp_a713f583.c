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
    struct GraphNode* parent;
};

struct GraphEdge {
    struct GraphNode* source;
    struct GraphNode* target;
    int weight;
};

struct Graph {
    struct GraphNode** nodes;
    struct GraphEdge** edges;
    int node_count;
    int edge_count;
    int max_nodes;
    int max_edges;
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

struct Graph* create_graph(int max_nodes, int max_edges) {
    if (max_nodes <= 0 || max_edges <= 0) return NULL;
    struct Graph* graph = malloc(sizeof(struct Graph));
    if (!graph) return NULL;
    graph->nodes = malloc(max_nodes * sizeof(struct GraphNode*));
    graph->edges = malloc(max_edges * sizeof(struct GraphEdge*));
    if (!graph->nodes || !graph->edges) {
        free(graph->nodes);
        free(graph->edges);
        free(graph);
        return NULL;
    }
    graph->node_count = 0;
    graph->edge_count = 0;
    graph->max_nodes = max_nodes;
    graph->max_edges = max_edges;
    return graph;
}

struct GraphNode* add_node(struct Graph* graph, int id) {
    if (!graph || graph->node_count >= graph->max_nodes) return NULL;
    for (int i = 0; i < graph->node_count; i++) {
        if (graph->nodes[i]->id == id) return NULL;
    }
    struct GraphNode* node = malloc(sizeof(struct GraphNode));
    if (!node) return NULL;
    node->id = id;
    node->visited = 0;
    node->distance = INT_MAX;
    node->parent = NULL;
    graph->nodes[graph->node_count] = node;
    graph->node_count++;
    return node;
}

struct GraphEdge* add_edge(struct Graph* graph, struct GraphNode* source, struct GraphNode* target, int weight) {
    if (!graph || !source || !target || graph->edge_count >= graph->max_edges) return NULL;
    if (weight < 0) return NULL;
    struct GraphEdge* edge = malloc(sizeof(struct GraphEdge));
    if (!edge) return NULL;
    edge->source = source;
    edge->target = target;
    edge->weight = weight;
    graph->edges[graph->edge_count] = edge;
    graph->edge_count++;
    return edge;
}

struct Queue* create_queue() {
    struct Queue* queue = malloc(sizeof(struct Queue));
    if (!queue) return NULL;
    queue->front = NULL;
    queue->rear = NULL;
    queue->size = 0;
    return queue;
}

void enqueue(struct Queue* queue, struct GraphNode* node) {
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

struct GraphNode* dequeue(struct Queue* queue) {
    if (!queue || !queue->front) return NULL;
    struct QueueNode* temp = queue->front;
    struct GraphNode* data = temp->data;
    queue->front = queue->front->next;
    if (!queue->front) queue->rear = NULL;
    free(temp);
    queue->size--;
    return data;
}

void bfs_shortest_path(struct Graph* graph, struct GraphNode* start) {
    if (!graph || !start) return;
    for (int i = 0; i < graph->node_count; i++) {
        graph->nodes[i]->visited = 0;
        graph->nodes[i]->distance = INT_MAX;
        graph->nodes[i]->parent = NULL;
    }
    start->visited = 1;
    start->distance = 0;
    struct Queue* queue = create_queue();
    if (!queue) return;
    enqueue(queue, start);
    while (queue->size > 0) {
        struct GraphNode* current = dequeue(queue);
        for (int i = 0; i < graph->edge_count; i++) {
            if (graph->edges[i]->source == current && !graph->edges[i]->target->visited) {
                graph->edges[i]->target->visited = 1;
                graph->edges[i]->target->distance = current->distance + 1;
                graph->edges[i]->target->parent = current;
                enqueue(queue, graph->edges[i]->target);
            }
        }
    }
    free(queue);
}

void print_shortest_path(struct Graph* graph, struct GraphNode* target) {
    if (!graph || !target) return;
    if (target->distance == INT_MAX) {
        printf("No path exists to node %d\n", target->id);
        return;
    }
    printf("Shortest path to node %d (distance %d): ", target->id, target->distance);
    struct GraphNode* path[100];
    int path_len = 0;
    struct GraphNode* current = target;
    while (current) {