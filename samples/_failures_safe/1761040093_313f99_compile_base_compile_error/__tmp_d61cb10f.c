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

void queue_init(struct Queue* q) {
    q->front = NULL;
    q->rear = NULL;
    q->size = 0;
}

int queue_enqueue(struct Queue* q, struct Node* node) {
    if (q->size >= MAX_NODES) return 0;
    struct QueueNode* new_node = malloc(sizeof(struct QueueNode));
    if (!new_node) return 0;
    new_node->data = node;
    new_node->next = NULL;
    if (q->rear) q->rear->next = new_node;
    q->rear = new_node;
    if (!q->front) q->front = new_node;
    q->size++;
    return 1;
}

struct Node* queue_dequeue(struct Queue* q) {
    if (!q->front) return NULL;
    struct QueueNode* temp = q->front;
    struct Node* data = temp->data;
    q->front = q->front->next;
    if (!q->front) q->rear = NULL;
    free(temp);
    q->size--;
    return data;
}

void queue_free(struct Queue* q) {
    while (q->front) {
        queue_dequeue(q);
    }
}

int graph_init(struct Graph* g) {
    g->node_count = 0;
    g->edge_count = 0;
    return 1;
}

int graph_add_node(struct Graph* g, const char* label) {
    if (g->node_count >= MAX_NODES) return -1;
    if (!label || strlen(label) >= 32) return -1;
    struct Node* node = &g->nodes[g->node_count];
    node->id = g->node_count;
    strncpy(node->label, label, 31);
    node->label[31] = '\0';
    node->visited = 0;
    g->node_count++;
    return node->id;
}

int graph_add_edge(struct Graph* g, int from_id, int to_id, int weight) {
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

void graph_reset_visited(struct Graph* g) {
    for (int i = 0; i < g->node_count; i++) {
        g->nodes[i].visited = 0;
    }
}

void graph_bfs(struct Graph* g, int start_id) {
    if (start_id < 0 || start_id >= g->node_count) return;
    graph_reset_visited(g);
    struct Queue q;
    queue_init(&q);
    struct Node* start = &g->nodes[start_id];
    start->visited = 1;
    queue_enqueue(&q, start);
    printf("BFS traversal: ");
    while (q.front) {
        struct Node* current = queue_dequeue(&q);
        printf("%s ", current->label);
        for (int i = 0; i < g->edge_count; i++) {
            if (g->edges[i].from == current && !g->edges[i].to->visited) {
                g->edges[i].to->visited = 1;
                queue_enqueue(&q, g->edges[i].to);
            }
        }
    }
    printf("\n");
    queue_free(&q);
}

void graph_dfs_util(struct Graph* g, struct Node* node) {
    node->visited = 1;
    printf("%s ", node->label);
    for (int i = 0; i < g->edge_count; i++) {
        if (g->edges[i].from == node && !g->edges[i].to->visited) {
            graph_dfs_util(g, g->edges[i].to);
        }
    }
}

void graph_dfs(struct Graph* g, int start_id) {
    if (start_id < 0 || start_id >= g->node_count) return;
    graph_reset_visited(g);
    printf("DFS traversal: ");
    graph_dfs_util(g, &g->nodes[start_id]);
    printf("\n");
}

int graph_dijkstra(struct Graph* g, int start_id, int end_id) {
    if (start_id < 0 || start_id >= g->node_count) return -1;
    if (end_id < 0 || end_id >= g->node_count) return -1;
    int dist[MAX_NODES];
    int visited[MAX_NODES