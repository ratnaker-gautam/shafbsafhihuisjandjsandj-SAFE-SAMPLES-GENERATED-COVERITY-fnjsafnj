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
    struct GraphNode *src = find_node(graph, src_id);
    struct GraphNode *dest = find_node(graph, dest_id);
    if (src == NULL || dest == NULL) {
        return 0;
    }
    
    struct GraphEdge *new_edge = malloc(sizeof(struct GraphEdge));
    if (new_edge == NULL) {
        return 0;
    }
    new_edge->src = src;
    new_edge->dest = dest;
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
    new_adj->next = src->next;
    src->next = new_adj;
    
    return 1;
}

void init_queue(struct Queue *q) {
    if (q == NULL) return;
    q->front = NULL;
    q->rear = NULL;
    q->size = 0;
}

int enqueue(struct Queue *q, struct GraphNode *node) {
    if (q == NULL || node == NULL) {
        return 0;
    }
    struct QueueNode *new_node = malloc(sizeof(struct QueueNode));
    if (new_node == NULL) {
        return 0;
    }
    new_node->data = node;
    new_node->next = NULL;
    
    if (q->rear == NULL) {
        q->front = new_node;
        q->rear = new_node;
    } else {
        q->rear->next = new_node;
        q->rear = new_node;
    }
    q->size++;
    return 1;
}

struct GraphNode *dequeue(struct Queue *q) {
    if (q == NULL || q->front == NULL) {
        return NULL;
    }
    struct QueueNode *temp = q->front;
    struct GraphNode *data = temp->data;
    q->front = q->front->next;
    if (q->front == NULL) {
        q->rear = NULL;
    }
    free(temp);
    q->size--;
    return data;
}

void bfs(struct Graph *graph, int start_id) {
    if (graph == NULL) return;
    struct GraphNode *start = find_node(graph, start_id);
    if (start == NULL) return;
    
    for (int i = 0; i < graph->node_count; i++) {
        graph->nodes[i].visited = 0;
        graph->nodes[i].distance = -1;
    }
    
    struct Queue q;
    init_queue(&q);
    start->visited = 1;
    start->distance = 0;
    enqueue(&q, start);
    
    while (q.size > 0) {
        struct GraphNode *current = dequeue(&q);
        if (current == NULL) continue;
        
        struct GraphNode *neighbor = current->next;
        while (neighbor != NULL) {
            struct GraphNode *actual_neighbor = find_node(graph, neighbor->id);
            if (actual_neighbor != NULL && !actual_neighbor->visited) {
                actual_neighbor->visited = 1;
                actual_neighbor->distance = current->distance + 1;
                enqueue(&q, actual_neighbor);
            }
            neighbor = neighbor->next;
        }
    }
    
    while (q.size > 0) {
        dequeue(&q);
    }
}

void print_bfs_results(struct Graph *graph, int start_id) {
    if (graph == NULL) return;
    printf("BFS results from node %d:\n", start_id);
    for (int i = 0; i < graph->node_count; i++) {
        printf("Node %d: distance %d\n", i, graph->nodes[i].distance);
    }
}

void free_graph(struct Graph *graph) {
    if (graph == NULL) return;
    
    if (graph->nodes != NULL) {
        for (int i = 0; i < graph->node_count; i++) {
            struct GraphNode *current = graph->nodes[i].next;
            while (current != NULL) {
                struct GraphNode *temp = current;
                current = current->next;
                free(temp);
            }
        }
        free(graph->nodes);