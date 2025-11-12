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
    struct GraphNode *next;
};

struct GraphEdge {
    struct GraphNode *src;
    struct GraphNode *dest;
    int weight;
    struct GraphEdge *next;
};

struct Graph {
    int node_count;
    int edge_count;
    struct GraphNode *nodes;
    struct GraphEdge *edges;
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

struct GraphNode *find_node(struct Graph *graph, int id) {
    if (graph == NULL || id < 0 || id >= graph->node_count) {
        return NULL;
    }
    return &graph->nodes[id];
}

int add_edge(struct Graph *graph, int src_id, int dest_id, int weight) {
    if (graph == NULL || weight < 0) {
        return 0;
    }
    struct GraphNode *src_node = find_node(graph, src_id);
    struct GraphNode *dest_node = find_node(graph, dest_id);
    if (src_node == NULL || dest_node == NULL) {
        return 0;
    }
    
    struct GraphEdge *new_edge = malloc(sizeof(struct GraphEdge));
    if (new_edge == NULL) {
        return 0;
    }
    
    new_edge->src = src_node;
    new_edge->dest = dest_node;
    new_edge->weight = weight;
    new_edge->next = graph->edges;
    graph->edges = new_edge;
    graph->edge_count++;
    
    struct GraphNode *new_adj = malloc(sizeof(struct GraphNode));
    if (new_adj == NULL) {
        return 0;
    }
    new_adj->id = dest_id;
    new_adj->visited = 0;
    new_adj->distance = 0;
    new_adj->next = src_node->next;
    src_node->next = new_adj;
    
    return 1;
}

struct Graph *create_graph(int node_count) {
    if (node_count <= 0 || node_count > 1000) {
        return NULL;
    }
    
    struct Graph *graph = malloc(sizeof(struct Graph));
    if (graph == NULL) {
        return NULL;
    }
    
    graph->node_count = node_count;
    graph->edge_count = 0;
    graph->edges = NULL;
    
    graph->nodes = malloc(node_count * sizeof(struct GraphNode));
    if (graph->nodes == NULL) {
        free(graph);
        return NULL;
    }
    
    for (int i = 0; i < node_count; i++) {
        graph->nodes[i].id = i;
        graph->nodes[i].visited = 0;
        graph->nodes[i].distance = INT_MAX;
        graph->nodes[i].next = NULL;
    }
    
    return graph;
}

void destroy_graph(struct Graph *graph) {
    if (graph == NULL) {
        return;
    }
    
    struct GraphEdge *edge = graph->edges;
    while (edge != NULL) {
        struct GraphEdge *next_edge = edge->next;
        free(edge);
        edge = next_edge;
    }
    
    for (int i = 0; i < graph->node_count; i++) {
        struct GraphNode *adj = graph->nodes[i].next;
        while (adj != NULL) {
            struct GraphNode *next_adj = adj->next;
            free(adj);
            adj = next_adj;
        }
    }
    
    free(graph->nodes);
    free(graph);
}

struct Queue *create_queue(void) {
    struct Queue *queue = malloc(sizeof(struct Queue));
    if (queue == NULL) {
        return NULL;
    }
    queue->front = NULL;
    queue->rear = NULL;
    queue->size = 0;
    return queue;
}

int enqueue(struct Queue *queue, struct GraphNode *node) {
    if (queue == NULL || node == NULL) {
        return 0;
    }
    
    struct QueueNode *new_node = malloc(sizeof(struct QueueNode));
    if (new_node == NULL) {
        return 0;
    }
    
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

struct GraphNode *dequeue(struct Queue *queue) {
    if (queue == NULL || queue->front == NULL) {
        return NULL;
    }
    
    struct QueueNode *temp = queue->front;
    struct GraphNode *data = temp->data;
    
    queue->front = queue->front->next;
    if (queue->front == NULL) {
        queue->rear = NULL;
    }
    
    free(temp);
    queue->size--;
    return data;
}

void destroy_queue(struct Queue *queue) {
    if (queue == NULL) {
        return;
    }
    
    while (queue->front != NULL) {
        dequeue(queue);
    }
    free(queue);
}

void bfs(struct Graph *graph, int start_id) {
    if (graph == NULL) {
        return;
    }
    
    struct GraphNode *start_node = find_node(graph, start_id);
    if (start_node ==