//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_NODES 100
#define MAX_EDGES 1000

struct GraphNode {
    int id;
    int visited;
    int distance;
    int predecessor;
};

struct GraphEdge {
    int source;
    int destination;
    int weight;
};

struct Graph {
    struct GraphNode nodes[MAX_NODES];
    struct GraphEdge edges[MAX_EDGES];
    int node_count;
    int edge_count;
};

struct PriorityQueueItem {
    int node_id;
    int distance;
};

struct PriorityQueue {
    struct PriorityQueueItem items[MAX_NODES];
    int size;
};

void initialize_graph(struct Graph *graph) {
    if (graph == NULL) return;
    graph->node_count = 0;
    graph->edge_count = 0;
    for (int i = 0; i < MAX_NODES; i++) {
        graph->nodes[i].id = -1;
        graph->nodes[i].visited = 0;
        graph->nodes[i].distance = INT_MAX;
        graph->nodes[i].predecessor = -1;
    }
}

int add_node(struct Graph *graph, int node_id) {
    if (graph == NULL || node_id < 0 || node_id >= MAX_NODES) {
        return -1;
    }
    if (graph->node_count >= MAX_NODES) {
        return -1;
    }
    if (graph->nodes[node_id].id != -1) {
        return -1;
    }
    graph->nodes[node_id].id = node_id;
    graph->nodes[node_id].distance = INT_MAX;
    graph->nodes[node_id].visited = 0;
    graph->nodes[node_id].predecessor = -1;
    graph->node_count++;
    return 0;
}

int add_edge(struct Graph *graph, int source, int destination, int weight) {
    if (graph == NULL || graph->edge_count >= MAX_EDGES) {
        return -1;
    }
    if (source < 0 || source >= MAX_NODES || destination < 0 || destination >= MAX_NODES) {
        return -1;
    }
    if (graph->nodes[source].id == -1 || graph->nodes[destination].id == -1) {
        return -1;
    }
    if (weight < 0) {
        return -1;
    }
    graph->edges[graph->edge_count].source = source;
    graph->edges[graph->edge_count].destination = destination;
    graph->edges[graph->edge_count].weight = weight;
    graph->edge_count++;
    return 0;
}

void initialize_priority_queue(struct PriorityQueue *pq) {
    if (pq == NULL) return;
    pq->size = 0;
}

void priority_queue_push(struct PriorityQueue *pq, int node_id, int distance) {
    if (pq == NULL || pq->size >= MAX_NODES) {
        return;
    }
    pq->items[pq->size].node_id = node_id;
    pq->items[pq->size].distance = distance;
    pq->size++;
}

int priority_queue_pop(struct PriorityQueue *pq) {
    if (pq == NULL || pq->size <= 0) {
        return -1;
    }
    int min_index = 0;
    int min_distance = pq->items[0].distance;
    for (int i = 1; i < pq->size; i++) {
        if (pq->items[i].distance < min_distance) {
            min_distance = pq->items[i].distance;
            min_index = i;
        }
    }
    int result = pq->items[min_index].node_id;
    pq->items[min_index] = pq->items[pq->size - 1];
    pq->size--;
    return result;
}

int priority_queue_empty(struct PriorityQueue *pq) {
    if (pq == NULL) return 1;
    return pq->size == 0;
}

void dijkstra(struct Graph *graph, int start_node) {
    if (graph == NULL || start_node < 0 || start_node >= MAX_NODES || graph->nodes[start_node].id == -1) {
        return;
    }
    for (int i = 0; i < MAX_NODES; i++) {
        if (graph->nodes[i].id != -1) {
            graph->nodes[i].distance = INT_MAX;
            graph->nodes[i].visited = 0;
            graph->nodes[i].predecessor = -1;
        }
    }
    graph->nodes[start_node].distance = 0;
    struct PriorityQueue pq;
    initialize_priority_queue(&pq);
    priority_queue_push(&pq, start_node, 0);
    while (!priority_queue_empty(&pq)) {
        int current_node = priority_queue_pop(&pq);
        if (graph->nodes[current_node].visited) {
            continue;
        }
        graph->nodes[current_node].visited = 1;
        for (int i = 0; i < graph->edge_count; i++) {
            if (graph->edges[i].source == current_node) {
                int neighbor = graph->edges[i].destination;
                if (neighbor < 0 || neighbor >= MAX_NODES || graph->nodes[neighbor].id == -1) {
                    continue;
                }
                if (graph->nodes[current_node].distance > INT_MAX - graph->edges[i].weight) {
                    continue;
                }
                int new_distance = graph->nodes[current_node].distance