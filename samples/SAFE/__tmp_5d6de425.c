//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: graph_algorithms
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

struct Graph* create_graph(int capacity) {
    if (capacity <= 0 || capacity > 1000) {
        return NULL;
    }
    struct Graph* graph = malloc(sizeof(struct Graph));
    if (!graph) return NULL;
    
    graph->nodes = malloc(capacity * sizeof(struct GraphNode*));
    graph->edges = malloc(capacity * 2 * sizeof(struct GraphEdge*));
    if (!graph->nodes || !graph->edges) {
        free(graph->nodes);
        free(graph->edges);
        free(graph);
        return NULL;
    }
    
    graph->node_count = 0;
    graph->edge_count = 0;
    graph->capacity = capacity;
    return graph;
}

struct GraphNode* add_node(struct Graph* graph, int id) {
    if (!graph || id < 0 || graph->node_count >= graph->capacity) {
        return NULL;
    }
    for (int i = 0; i < graph->node_count; i++) {
        if (graph->nodes[i]->id == id) {
            return NULL;
        }
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

int add_edge(struct Graph* graph, int source_id, int target_id, int weight) {
    if (!graph || weight < 0 || weight > 1000) {
        return 0;
    }
    
    struct GraphNode* source = NULL;
    struct GraphNode* target = NULL;
    
    for (int i = 0; i < graph->node_count; i++) {
        if (graph->nodes[i]->id == source_id) source = graph->nodes[i];
        if (graph->nodes[i]->id == target_id) target = graph->nodes[i];
    }
    
    if (!source || !target || graph->edge_count >= graph->capacity * 2) {
        return 0;
    }
    
    struct GraphEdge* edge = malloc(sizeof(struct GraphEdge));
    if (!edge) return 0;
    
    edge->source = source;
    edge->target = target;
    edge->weight = weight;
    
    graph->edges[graph->edge_count] = edge;
    graph->edge_count++;
    return 1;
}

struct Queue* create_queue() {
    struct Queue* queue = malloc(sizeof(struct Queue));
    if (!queue) return NULL;
    queue->front = NULL;
    queue->rear = NULL;
    queue->size = 0;
    return queue;
}

int enqueue(struct Queue* queue, struct GraphNode* node) {
    if (!queue || !node) return 0;
    
    struct QueueNode* new_node = malloc(sizeof(struct QueueNode));
    if (!new_node) return 0;
    
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

struct GraphNode* dequeue(struct Queue* queue) {
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

void bfs_shortest_path(struct Graph* graph, int start_id) {
    if (!graph || graph->node_count == 0) return;
    
    struct GraphNode* start = NULL;
    for (int i = 0; i < graph->node_count; i++) {
        graph->nodes[i]->visited = 0;
        graph->nodes[i]->distance = INT_MAX;
        graph->nodes[i]->parent = NULL;
        if (graph->nodes[i]->id == start_id) {
            start = graph->nodes[i];
        }
    }
    
    if (!start) return;
    
    start->distance = 0;
    start->visited = 1;
    
    struct Queue* queue = create_queue();
    if (!queue) return;
    
    enqueue(queue, start);
    
    while (queue->size > 0) {
        struct GraphNode* current = dequeue(queue);
        if (!current) continue;
        
        for (int i = 0; i < graph->edge_count; i++) {
            if (graph->edges[i]->source == current && !graph->edges[i]->target->visited) {
                struct GraphNode* neighbor = graph->edges[i]->target;