//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

struct GraphNode {
    int id;
    int visited;
    struct GraphNode** neighbors;
    int neighbor_count;
    int capacity;
};

struct Graph {
    struct GraphNode** nodes;
    int node_count;
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

struct Graph* create_graph(int initial_capacity) {
    if (initial_capacity <= 0) return NULL;
    struct Graph* graph = malloc(sizeof(struct Graph));
    if (!graph) return NULL;
    graph->nodes = malloc(sizeof(struct GraphNode*) * initial_capacity);
    if (!graph->nodes) {
        free(graph);
        return NULL;
    }
    graph->node_count = 0;
    graph->capacity = initial_capacity;
    return graph;
}

struct GraphNode* create_node(int id, int initial_neighbors) {
    if (initial_neighbors <= 0) return NULL;
    struct GraphNode* node = malloc(sizeof(struct GraphNode));
    if (!node) return NULL;
    node->id = id;
    node->visited = 0;
    node->neighbors = malloc(sizeof(struct GraphNode*) * initial_neighbors);
    if (!node->neighbors) {
        free(node);
        return NULL;
    }
    node->neighbor_count = 0;
    node->capacity = initial_neighbors;
    return node;
}

int add_node(struct Graph* graph, struct GraphNode* node) {
    if (!graph || !node) return 0;
    if (graph->node_count >= graph->capacity) {
        int new_capacity = graph->capacity * 2;
        if (new_capacity <= graph->capacity) return 0;
        struct GraphNode** new_nodes = realloc(graph->nodes, sizeof(struct GraphNode*) * new_capacity);
        if (!new_nodes) return 0;
        graph->nodes = new_nodes;
        graph->capacity = new_capacity;
    }
    graph->nodes[graph->node_count] = node;
    graph->node_count++;
    return 1;
}

int add_edge(struct GraphNode* from, struct GraphNode* to) {
    if (!from || !to) return 0;
    if (from->neighbor_count >= from->capacity) {
        int new_capacity = from->capacity * 2;
        if (new_capacity <= from->capacity) return 0;
        struct GraphNode** new_neighbors = realloc(from->neighbors, sizeof(struct GraphNode*) * new_capacity);
        if (!new_neighbors) return 0;
        from->neighbors = new_neighbors;
        from->capacity = new_capacity;
    }
    from->neighbors[from->neighbor_count] = to;
    from->neighbor_count++;
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
    if (!queue->front) queue->rear = NULL;
    free(temp);
    queue->size--;
    return data;
}

void reset_visited(struct Graph* graph) {
    if (!graph) return;
    for (int i = 0; i < graph->node_count; i++) {
        if (graph->nodes[i]) {
            graph->nodes[i]->visited = 0;
        }
    }
}

void dfs_util(struct GraphNode* node) {
    if (!node || node->visited) return;
    node->visited = 1;
    printf("%d ", node->id);
    for (int i = 0; i < node->neighbor_count; i++) {
        dfs_util(node->neighbors[i]);
    }
}

void dfs(struct Graph* graph, struct GraphNode* start) {
    if (!graph || !start) return;
    reset_visited(graph);
    dfs_util(start);
    printf("\n");
}

void bfs(struct Graph* graph, struct GraphNode* start) {
    if (!graph || !start) return;
    reset_visited(graph);
    struct Queue* queue = create_queue();
    if (!queue) return;
    start->visited = 1;
    enqueue(queue, start);
    while (queue->size > 0) {
        struct GraphNode* current = dequeue(queue);
        printf("%d ", current->id);
        for (int i = 0; i < current->neighbor_count; i++) {
            struct GraphNode* neighbor = current->neighbors[i];
            if (neighbor && !neighbor->visited) {