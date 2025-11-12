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
    
    if (from == NULL || to == NULL) return 0;
    
    graph->edges[graph->edge_count].from = from;
    graph->edges[graph->edge_count].to = to;
    graph->edges[graph->edge_count].weight = weight;
    graph->edge_count++;
    return 1;
}

void init_queue(struct Queue* q) {
    q->front = NULL;
    q->rear = NULL;
    q->size = 0;
}

int enqueue(struct Queue* q, struct Node* node) {
    if (q->size >= MAX_NODES) return 0;
    
    struct QueueNode* new_node = malloc(sizeof(struct QueueNode));
    if (new_node == NULL) return 0;
    
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

struct Node* dequeue(struct Queue* q) {
    if (q->front == NULL) return NULL;
    
    struct QueueNode* temp = q->front;
    struct Node* node = temp->data;
    
    q->front = q->front->next;
    if (q->front == NULL) {
        q->rear = NULL;
    }
    
    free(temp);
    q->size--;
    return node;
}

int is_queue_empty(struct Queue* q) {
    return q->front == NULL;
}

void reset_visited(struct Graph* graph) {
    for (int i = 0; i < graph->node_count; i++) {
        graph->nodes[i].visited = 0;
    }
}

void bfs(struct Graph* graph, int start_id) {
    struct Node* start = find_node(graph, start_id);
    if (start == NULL) return;
    
    reset_visited(graph);
    struct Queue q;
    init_queue(&q);
    
    start->visited = 1;
    if (!enqueue(&q, start)) return;
    
    printf("BFS traversal: ");
    while (!is_queue_empty(&q)) {
        struct Node* current = dequeue(&q);
        printf("%s ", current->label);
        
        for (int i = 0; i < graph->edge_count; i++) {
            if (graph->edges[i].from == current && !graph->edges[i].to->visited) {
                graph->edges[i].to->visited = 1;
                if (!enqueue(&q, graph->edges[i].to)) return;
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

void dfs(struct Graph* graph, int start