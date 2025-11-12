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
    if (graph->node_count >= MAX_NODES || node_id < 0 || node_id >= MAX_NODES) {
        return -1;
    }
    if (graph->nodes[node_id].id != -1) {
        return -1;
    }
    graph->nodes[node_id].id = node_id;
    graph->nodes[node_id].visited = 0;
    graph->nodes[node_id].distance = INT_MAX;
    graph->nodes[node_id].predecessor = -1;
    graph->node_count++;
    return 0;
}

int add_edge(struct Graph *graph, int source, int destination, int weight) {
    if (graph->edge_count >= MAX_EDGES) {
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
    pq->size = 0;
}

void priority_queue_push(struct PriorityQueue *pq, int node_id, int distance) {
    if (pq->size >= MAX_NODES) {
        return;
    }
    pq->items[pq->size].node_id = node_id;
    pq->items[pq->size].distance = distance;
    pq->size++;
}

int priority_queue_pop(struct PriorityQueue *pq) {
    if (pq->size <= 0) {
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
    return pq->size == 0;
}

void dijkstra(struct Graph *graph, int start_node) {
    if (start_node < 0 || start_node >= MAX_NODES || graph->nodes[start_node].id == -1) {
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
        int current = priority_queue_pop(&pq);
        if (graph->nodes[current].visited) {
            continue;
        }
        graph->nodes[current].visited = 1;
        for (int i = 0; i < graph->edge_count; i++) {
            if (graph->edges[i].source == current) {
                int neighbor = graph->edges[i].destination;
                if (graph->nodes[neighbor].id == -1) {
                    continue;
                }
                int new_distance;
                if (graph->nodes[current].distance == INT_MAX) {
                    new_distance = INT_MAX;
                } else {
                    new_distance = graph->nodes[current].distance + graph->edges[i].weight;
                    if (new_distance < graph->nodes[current].distance) {
                        new_distance = INT_MAX;
                    }
                }
                if (new_distance < graph->nodes[neighbor].distance) {
                    graph->nodes[neighbor].distance = new_distance;
                    graph->nodes[neighbor].predecessor = current;