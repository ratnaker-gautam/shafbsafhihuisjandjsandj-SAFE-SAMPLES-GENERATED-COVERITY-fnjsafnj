//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 100
#define INF INT_MAX

struct GraphNode {
    int id;
    int visited;
};

struct GraphEdge {
    struct GraphNode* source;
    struct GraphNode* target;
    int weight;
};

struct Graph {
    struct GraphNode nodes[MAX_NODES];
    struct GraphEdge edges[MAX_NODES * MAX_NODES];
    int node_count;
    int edge_count;
    int adjacency[MAX_NODES][MAX_NODES];
};

struct GraphPath {
    int nodes[MAX_NODES];
    int length;
    int total_weight;
};

struct GraphQueue {
    int items[MAX_NODES];
    int front;
    int rear;
};

void initialize_graph(struct Graph* graph) {
    graph->node_count = 0;
    graph->edge_count = 0;
    for (int i = 0; i < MAX_NODES; i++) {
        graph->nodes[i].id = -1;
        graph->nodes[i].visited = 0;
        for (int j = 0; j < MAX_NODES; j++) {
            graph->adjacency[i][j] = INF;
        }
    }
}

int add_node(struct Graph* graph, int id) {
    if (graph->node_count >= MAX_NODES || id < 0 || id >= MAX_NODES) {
        return -1;
    }
    for (int i = 0; i < graph->node_count; i++) {
        if (graph->nodes[i].id == id) {
            return -1;
        }
    }
    graph->nodes[graph->node_count].id = id;
    graph->nodes[graph->node_count].visited = 0;
    graph->node_count++;
    return 0;
}

int add_edge(struct Graph* graph, int source_id, int target_id, int weight) {
    if (weight <= 0 || source_id < 0 || target_id < 0 || source_id >= MAX_NODES || target_id >= MAX_NODES) {
        return -1;
    }
    int source_index = -1;
    int target_index = -1;
    for (int i = 0; i < graph->node_count; i++) {
        if (graph->nodes[i].id == source_id) {
            source_index = i;
        }
        if (graph->nodes[i].id == target_id) {
            target_index = i;
        }
    }
    if (source_index == -1 || target_index == -1) {
        return -1;
    }
    graph->edges[graph->edge_count].source = &graph->nodes[source_index];
    graph->edges[graph->edge_count].target = &graph->nodes[target_index];
    graph->edges[graph->edge_count].weight = weight;
    graph->adjacency[source_index][target_index] = weight;
    graph->edge_count++;
    return 0;
}

void initialize_queue(struct GraphQueue* queue) {
    queue->front = -1;
    queue->rear = -1;
}

int is_queue_empty(struct GraphQueue* queue) {
    return queue->front == -1;
}

void enqueue(struct GraphQueue* queue, int value) {
    if (queue->rear == MAX_NODES - 1) {
        return;
    }
    if (queue->front == -1) {
        queue->front = 0;
    }
    queue->rear++;
    queue->items[queue->rear] = value;
}

int dequeue(struct GraphQueue* queue) {
    if (is_queue_empty(queue)) {
        return -1;
    }
    int item = queue->items[queue->front];
    queue->front++;
    if (queue->front > queue->rear) {
        queue->front = queue->rear = -1;
    }
    return item;
}

void bfs_shortest_path(struct Graph* graph, int start_id, int end_id) {
    if (start_id < 0 || end_id < 0 || start_id >= MAX_NODES || end_id >= MAX_NODES) {
        printf("Invalid node IDs\n");
        return;
    }
    int start_index = -1;
    int end_index = -1;
    for (int i = 0; i < graph->node_count; i++) {
        graph->nodes[i].visited = 0;
        if (graph->nodes[i].id == start_id) {
            start_index = i;
        }
        if (graph->nodes[i].id == end_id) {
            end_index = i;
        }
    }
    if (start_index == -1 || end_index == -1) {
        printf("Start or end node not found\n");
        return;
    }
    int parent[MAX_NODES];
    for (int i = 0; i < MAX_NODES; i++) {
        parent[i] = -1;
    }
    struct GraphQueue queue;
    initialize_queue(&queue);
    graph->nodes[start_index].visited = 1;
    enqueue(&queue, start_index);
    while (!is_queue_empty(&queue)) {
        int current_index = dequeue(&queue);
        if (current_index == end_index) {
            break;
        }
        for (int i = 0; i < graph->node_count; i++) {
            if (graph->adjacency[current_index][i] != INF && !graph->nodes[i].visited) {
                graph->nodes[i].visited = 1;
                parent[i] = current_index;
                enqueue(&queue, i);
            }