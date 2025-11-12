//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

struct GraphNode {
    int id;
    int visited;
    struct Edge* edges;
    struct GraphNode* next;
};

struct Edge {
    struct GraphNode* target;
    int weight;
    struct Edge* next;
};

struct Graph {
    struct GraphNode* nodes;
    int node_count;
};

struct QueueNode {
    struct GraphNode* data;
    struct QueueNode* next;
};

struct Queue {
    struct QueueNode* front;
    struct QueueNode* rear;
};

struct Graph* create_graph(void) {
    struct Graph* graph = malloc(sizeof(struct Graph));
    if (graph == NULL) {
        exit(EXIT_FAILURE);
    }
    graph->nodes = NULL;
    graph->node_count = 0;
    return graph;
}

struct GraphNode* create_node(int id) {
    struct GraphNode* node = malloc(sizeof(struct GraphNode));
    if (node == NULL) {
        exit(EXIT_FAILURE);
    }
    node->id = id;
    node->visited = 0;
    node->edges = NULL;
    node->next = NULL;
    return node;
}

void add_edge(struct GraphNode* from, struct GraphNode* to, int weight) {
    if (from == NULL || to == NULL) {
        return;
    }
    struct Edge* edge = malloc(sizeof(struct Edge));
    if (edge == NULL) {
        exit(EXIT_FAILURE);
    }
    edge->target = to;
    edge->weight = weight;
    edge->next = from->edges;
    from->edges = edge;
}

struct GraphNode* find_or_create_node(struct Graph* graph, int id) {
    if (graph == NULL) {
        return NULL;
    }
    struct GraphNode* current = graph->nodes;
    while (current != NULL) {
        if (current->id == id) {
            return current;
        }
        current = current->next;
    }
    struct GraphNode* new_node = create_node(id);
    new_node->next = graph->nodes;
    graph->nodes = new_node;
    graph->node_count++;
    return new_node;
}

struct Queue* create_queue(void) {
    struct Queue* queue = malloc(sizeof(struct Queue));
    if (queue == NULL) {
        exit(EXIT_FAILURE);
    }
    queue->front = NULL;
    queue->rear = NULL;
    return queue;
}

void enqueue(struct Queue* queue, struct GraphNode* node) {
    if (queue == NULL || node == NULL) {
        return;
    }
    struct QueueNode* new_node = malloc(sizeof(struct QueueNode));
    if (new_node == NULL) {
        exit(EXIT_FAILURE);
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
}

struct GraphNode* dequeue(struct Queue* queue) {
    if (queue == NULL || queue->front == NULL) {
        return NULL;
    }
    struct QueueNode* temp = queue->front;
    struct GraphNode* data = temp->data;
    queue->front = queue->front->next;
    if (queue->front == NULL) {
        queue->rear = NULL;
    }
    free(temp);
    return data;
}

int is_queue_empty(struct Queue* queue) {
    if (queue == NULL) {
        return 1;
    }
    return queue->front == NULL;
}

void reset_visited(struct Graph* graph) {
    if (graph == NULL) {
        return;
    }
    struct GraphNode* current = graph->nodes;
    while (current != NULL) {
        current->visited = 0;
        current = current->next;
    }
}

void bfs(struct Graph* graph, int start_id) {
    if (graph == NULL) {
        return;
    }
    reset_visited(graph);
    struct GraphNode* start = find_or_create_node(graph, start_id);
    if (start == NULL) {
        return;
    }
    struct Queue* queue = create_queue();
    start->visited = 1;
    enqueue(queue, start);
    printf("BFS traversal from node %d: ", start_id);
    while (!is_queue_empty(queue)) {
        struct GraphNode* current = dequeue(queue);
        printf("%d ", current->id);
        struct Edge* edge = current->edges;
        while (edge != NULL) {
            if (edge->target != NULL && edge->target->visited == 0) {
                edge->target->visited = 1;
                enqueue(queue, edge->target);
            }
            edge = edge->next;
        }
    }
    printf("\n");
    free(queue);
}

void dfs_util(struct GraphNode* node) {
    if (node == NULL || node->visited) {
        return;
    }
    node->visited = 1;
    printf("%d ", node->id);
    struct Edge* edge = node->edges;
    while (edge != NULL) {
        if (edge->target != NULL) {
            dfs_util(edge->target);
        }
        edge = edge->next;
    }
}

void dfs(struct Graph* graph, int start_id) {
    if (graph == NULL) {
        return;
    }
    reset_visited(graph);
    struct GraphNode* start = find_or_create_node(graph, start_id);
    if (start == NULL) {
        return;
    }
    printf("