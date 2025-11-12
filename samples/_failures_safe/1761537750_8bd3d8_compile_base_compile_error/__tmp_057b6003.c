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
    int data;
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

int enqueue(struct Queue* q, int value) {
    if (q->size >= MAX_NODES) return 0;
    struct QueueNode* new_node = malloc(sizeof(struct QueueNode));
    if (!new_node) return 0;
    new_node->data = value;
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

int dequeue(struct Queue* q, int* value) {
    if (q->front == NULL) return 0;
    struct QueueNode* temp = q->front;
    *value = temp->data;
    q->front = q->front->next;
    if (q->front == NULL) q->rear = NULL;
    free(temp);
    q->size--;
    return 1;
}

int is_queue_empty(struct Queue* q) {
    return q->front == NULL;
}

void free_queue(struct Queue* q) {
    int dummy;
    while (dequeue(q, &dummy));
}

void init_graph(struct Graph* g) {
    g->node_count = 0;
    g->edge_count = 0;
}

int add_node(struct Graph* g, const char* label) {
    if (g->node_count >= MAX_NODES) return -1;
    if (strlen(label) >= 32) return -1;
    struct Node* node = &g->nodes[g->node_count];
    node->id = g->node_count;
    strncpy(node->label, label, 31);
    node->label[31] = '\0';
    g->node_count++;
    return node->id;
}

int add_edge(struct Graph* g, int from_id, int to_id, int weight) {
    if (from_id < 0 || from_id >= g->node_count) return 0;
    if (to_id < 0 || to_id >= g->node_count) return 0;
    if (g->edge_count >= MAX_EDGES) return 0;
    struct Edge* edge = &g->edges[g->edge_count];
    edge->from = &g->nodes[from_id];
    edge->to = &g->nodes[to_id];
    edge->weight = weight;
    g->edge_count++;
    return 1;
}

void bfs(struct Graph* g, int start_id) {
    if (start_id < 0 || start_id >= g->node_count) return;
    int visited[MAX_NODES] = {0};
    struct Queue q;
    init_queue(&q);
    visited[start_id] = 1;
    if (!enqueue(&q, start_id)) {
        free_queue(&q);
        return;
    }
    printf("BFS traversal: ");
    while (!is_queue_empty(&q)) {
        int current;
        if (!dequeue(&q, &current)) break;
        printf("%s ", g->nodes[current].label);
        for (int i = 0; i < g->edge_count; i++) {
            if (g->edges[i].from->id == current) {
                int neighbor = g->edges[i].to->id;
                if (!visited[neighbor]) {
                    visited[neighbor] = 1;
                    if (!enqueue(&q, neighbor)) {
                        free_queue(&q);
                        return;
                    }
                }
            }
        }
    }
    printf("\n");
    free_queue(&q);
}

void dfs_util(struct Graph* g, int node_id, int visited[]) {
    visited[node_id] = 1;
    printf("%s ", g->nodes[node_id].label);
    for (int i = 0; i < g->edge_count; i++) {
        if (g->edges[i].from->id == node_id) {
            int neighbor = g->edges[i].to->id;
            if (!visited[neighbor]) {
                dfs_util(g, neighbor, visited);
            }
        }
    }
}

void dfs(struct Graph* g, int start_id) {
    if (start_id < 0 || start_id >= g->node_count) return;
    int visited[MAX_NODES] = {0};
    printf("DFS traversal: ");
    dfs_util(g, start_id, visited);
    printf("\n");
}

int main() {
    struct Graph g;
    init_graph(&g);
    int node_a = add_node(&g, "A");
    int node_b = add_node(&g, "B");
    int node_c = add_node(&g, "C");
    int node_d = add_node