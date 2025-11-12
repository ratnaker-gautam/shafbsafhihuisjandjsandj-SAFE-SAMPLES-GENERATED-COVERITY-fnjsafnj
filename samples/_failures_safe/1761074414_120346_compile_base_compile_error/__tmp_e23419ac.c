//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: graph_algorithms
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

void init_queue(struct Queue* q) {
    q->front = NULL;
    q->rear = NULL;
    q->size = 0;
}

int is_queue_empty(struct Queue* q) {
    return q->size == 0;
}

void enqueue(struct Queue* q, struct Node* node) {
    if (q->size >= MAX_NODES) return;
    
    struct QueueNode* new_node = malloc(sizeof(struct QueueNode));
    if (!new_node) return;
    
    new_node->data = node;
    new_node->next = NULL;
    
    if (q->rear) {
        q->rear->next = new_node;
    } else {
        q->front = new_node;
    }
    q->rear = new_node;
    q->size++;
}

struct Node* dequeue(struct Queue* q) {
    if (is_queue_empty(q)) return NULL;
    
    struct QueueNode* temp = q->front;
    struct Node* data = temp->data;
    
    q->front = q->front->next;
    if (!q->front) {
        q->rear = NULL;
    }
    
    free(temp);
    q->size--;
    return data;
}

void init_graph(struct Graph* g) {
    g->node_count = 0;
    g->edge_count = 0;
    for (int i = 0; i < MAX_NODES; i++) {
        g->nodes[i].id = -1;
        g->nodes[i].visited = 0;
        g->nodes[i].label[0] = '\0';
    }
}

int add_node(struct Graph* g, const char* label) {
    if (g->node_count >= MAX_NODES || !label) return -1;
    
    for (int i = 0; i < g->node_count; i++) {
        if (strcmp(g->nodes[i].label, label) == 0) {
            return -1;
        }
    }
    
    struct Node* node = &g->nodes[g->node_count];
    node->id = g->node_count;
    node->visited = 0;
    strncpy(node->label, label, 31);
    node->label[31] = '\0';
    
    g->node_count++;
    return node->id;
}

int add_edge(struct Graph* g, const char* from_label, const char* to_label, int weight) {
    if (g->edge_count >= MAX_EDGES || !from_label || !to_label) return -1;
    
    struct Node* from_node = NULL;
    struct Node* to_node = NULL;
    
    for (int i = 0; i < g->node_count; i++) {
        if (strcmp(g->nodes[i].label, from_label) == 0) {
            from_node = &g->nodes[i];
        }
        if (strcmp(g->nodes[i].label, to_label) == 0) {
            to_node = &g->nodes[i];
        }
    }
    
    if (!from_node || !to_node) return -1;
    
    struct Edge* edge = &g->edges[g->edge_count];
    edge->from = from_node;
    edge->to = to_node;
    edge->weight = weight;
    
    g->edge_count++;
    return 0;
}

void reset_visited(struct Graph* g) {
    for (int i = 0; i < g->node_count; i++) {
        g->nodes[i].visited = 0;
    }
}

void dfs_util(struct Graph* g, struct Node* node) {
    if (!g || !node) return;
    
    node->visited = 1;
    printf("%s ", node->label);
    
    for (int i = 0; i < g->edge_count; i++) {
        if (g->edges[i].from == node && !g->edges[i].to->visited) {
            dfs_util(g, g->edges[i].to);
        }
    }
}

void dfs(struct Graph* g, const char* start_label) {
    if (!g || !start_label) return;
    
    struct Node* start_node = NULL;
    for (int i = 0; i < g->node_count; i++) {
        if (strcmp(g->nodes[i].label, start_label) == 0) {
            start_node = &g->nodes[i];
            break;
        }
    }
    
    if (!start_node) return;
    
    reset_visited(g);
    printf("DFS traversal: ");
    dfs_util(g, start_node);
    printf("\n");
}

void bfs(struct Graph* g, const char* start_label) {
    if (!g || !start_label) return;
    
    struct Node* start_node = NULL;
    for (int i = 0; i < g->node_count;