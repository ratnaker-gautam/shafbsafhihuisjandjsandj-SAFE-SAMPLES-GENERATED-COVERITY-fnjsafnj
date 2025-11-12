//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_NODES 100
#define MAX_EDGES 1000

struct Node {
    int id;
    char label[32];
    int visited;
};

struct Edge {
    struct Node* from;
    struct Node* to;
    int weight;
};

struct Graph {
    struct Node nodes[MAX_NODES];
    struct Edge edges[MAX_EDGES];
    int node_count;
    int edge_count;
};

struct QueueNode {
    struct Node* data;
    struct QueueNode* next;
};

struct Queue {
    struct QueueNode* front;
    struct QueueNode* rear;
    int size;
};

void init_graph(struct Graph* graph) {
    graph->node_count = 0;
    graph->edge_count = 0;
}

int add_node(struct Graph* graph, int id, const char* label) {
    if (graph->node_count >= MAX_NODES) return 0;
    if (id < 0 || id >= MAX_NODES) return 0;
    if (strlen(label) >= 32) return 0;
    
    for (int i = 0; i < graph->node_count; i++) {
        if (graph->nodes[i].id == id) return 0;
    }
    
    graph->nodes[graph->node_count].id = id;
    strncpy(graph->nodes[graph->node_count].label, label, 31);
    graph->nodes[graph->node_count].label[31] = '\0';
    graph->nodes[graph->node_count].visited = 0;
    graph->node_count++;
    return 1;
}

struct Node* find_node(struct Graph* graph, int id) {
    for (int i = 0; i < graph->node_count; i++) {
        if (graph->nodes[i].id == id) {
            return &graph->nodes[i];
        }
    }
    return NULL;
}

int add_edge(struct Graph* graph, int from_id, int to_id, int weight) {
    if (graph->edge_count >= MAX_EDGES) return 0;
    if (weight < 0) return 0;
    
    struct Node* from = find_node(graph, from_id);
    struct Node* to = find_node(graph, to_id);
    
    if (!from || !to) return 0;
    
    graph->edges[graph->edge_count].from = from;
    graph->edges[graph->edge_count].to = to;
    graph->edges[graph->edge_count].weight = weight;
    graph->edge_count++;
    return 1;
}

void init_queue(struct Queue* queue) {
    queue->front = NULL;
    queue->rear = NULL;
    queue->size = 0;
}

int enqueue(struct Queue* queue, struct Node* data) {
    if (queue->size >= MAX_NODES) return 0;
    
    struct QueueNode* new_node = malloc(sizeof(struct QueueNode));
    if (!new_node) return 0;
    
    new_node->data = data;
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

struct Node* dequeue(struct Queue* queue) {
    if (queue->front == NULL) return NULL;
    
    struct QueueNode* temp = queue->front;
    struct Node* data = temp->data;
    
    queue->front = queue->front->next;
    if (queue->front == NULL) {
        queue->rear = NULL;
    }
    
    free(temp);
    queue->size--;
    return data;
}

int is_queue_empty(struct Queue* queue) {
    return queue->front == NULL;
}

void reset_visited(struct Graph* graph) {
    for (int i = 0; i < graph->node_count; i++) {
        graph->nodes[i].visited = 0;
    }
}

void bfs(struct Graph* graph, int start_id) {
    struct Node* start = find_node(graph, start_id);
    if (!start) return;
    
    reset_visited(graph);
    
    struct Queue queue;
    init_queue(&queue);
    
    start->visited = 1;
    enqueue(&queue, start);
    
    printf("BFS traversal: ");
    
    while (!is_queue_empty(&queue)) {
        struct Node* current = dequeue(&queue);
        printf("%s ", current->label);
        
        for (int i = 0; i < graph->edge_count; i++) {
            if (graph->edges[i].from == current && !graph->edges[i].to->visited) {
                graph->edges[i].to->visited = 1;
                enqueue(&queue, graph->edges[i].to);
            }
        }
    }
    printf("\n");
}

void dfs_util(struct Node* node, struct Graph* graph) {
    node->visited = 1;
    printf("%s ", node->label);
    
    for (int i = 0; i < graph->edge_count; i++) {
        if (graph->edges[i].from == node && !graph->edges[i].to->visited) {
            dfs_util(graph->edges[i].to, graph);
        }
    }
}

void dfs(struct Graph* graph, int start_id) {
    struct Node* start = find_node(graph