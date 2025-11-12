//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_VERTICES 100
#define INF INT_MAX

struct Edge {
    int dest;
    int weight;
    struct Edge* next;
};

struct Vertex {
    int id;
    struct Edge* edges;
};

struct Graph {
    int num_vertices;
    struct Vertex vertices[MAX_VERTICES];
};

struct QueueNode {
    int data;
    struct QueueNode* next;
};

struct Queue {
    struct QueueNode* front;
    struct QueueNode* rear;
};

void init_graph(struct Graph* g, int num_vertices) {
    if (num_vertices <= 0 || num_vertices > MAX_VERTICES) {
        num_vertices = MAX_VERTICES;
    }
    g->num_vertices = num_vertices;
    for (int i = 0; i < num_vertices; i++) {
        g->vertices[i].id = i;
        g->vertices[i].edges = NULL;
    }
}

int add_edge(struct Graph* g, int src, int dest, int weight) {
    if (src < 0 || src >= g->num_vertices || dest < 0 || dest >= g->num_vertices) {
        return 0;
    }
    if (weight < 0) {
        return 0;
    }
    struct Edge* new_edge = malloc(sizeof(struct Edge));
    if (!new_edge) {
        return 0;
    }
    new_edge->dest = dest;
    new_edge->weight = weight;
    new_edge->next = g->vertices[src].edges;
    g->vertices[src].edges = new_edge;
    return 1;
}

void free_graph(struct Graph* g) {
    for (int i = 0; i < g->num_vertices; i++) {
        struct Edge* current = g->vertices[i].edges;
        while (current) {
            struct Edge* temp = current;
            current = current->next;
            free(temp);
        }
    }
}

void init_queue(struct Queue* q) {
    q->front = NULL;
    q->rear = NULL;
}

int is_queue_empty(struct Queue* q) {
    return q->front == NULL;
}

void enqueue(struct Queue* q, int data) {
    struct QueueNode* new_node = malloc(sizeof(struct QueueNode));
    if (!new_node) {
        return;
    }
    new_node->data = data;
    new_node->next = NULL;
    if (is_queue_empty(q)) {
        q->front = new_node;
        q->rear = new_node;
    } else {
        q->rear->next = new_node;
        q->rear = new_node;
    }
}

int dequeue(struct Queue* q) {
    if (is_queue_empty(q)) {
        return -1;
    }
    struct QueueNode* temp = q->front;
    int data = temp->data;
    q->front = q->front->next;
    if (q->front == NULL) {
        q->rear = NULL;
    }
    free(temp);
    return data;
}

void bfs(struct Graph* g, int start) {
    if (start < 0 || start >= g->num_vertices) {
        return;
    }
    int visited[MAX_VERTICES] = {0};
    struct Queue q;
    init_queue(&q);
    visited[start] = 1;
    enqueue(&q, start);
    printf("BFS traversal: ");
    while (!is_queue_empty(&q)) {
        int current = dequeue(&q);
        printf("%d ", current);
        struct Edge* edge = g->vertices[current].edges;
        while (edge) {
            if (!visited[edge->dest]) {
                visited[edge->dest] = 1;
                enqueue(&q, edge->dest);
            }
            edge = edge->next;
        }
    }
    printf("\n");
}

void dijkstra(struct Graph* g, int start) {
    if (start < 0 || start >= g->num_vertices) {
        return;
    }
    int dist[MAX_VERTICES];
    int visited[MAX_VERTICES] = {0};
    for (int i = 0; i < g->num_vertices; i++) {
        dist[i] = INF;
    }
    dist[start] = 0;
    for (int count = 0; count < g->num_vertices - 1; count++) {
        int min_dist = INF;
        int min_index = -1;
        for (int v = 0; v < g->num_vertices; v++) {
            if (!visited[v] && dist[v] <= min_dist) {
                min_dist = dist[v];
                min_index = v;
            }
        }
        if (min_index == -1) {
            break;
        }
        visited[min_index] = 1;
        struct Edge* edge = g->vertices[min_index].edges;
        while (edge) {
            if (!visited[edge->dest] && dist[min_index] != INF) {
                long new_dist = (long)dist[min_index] + edge->weight;
                if (new_dist < dist[edge->dest] && new_dist <= INT_MAX) {
                    dist[edge->dest] = new_dist;
                }
            }
            edge = edge->next;
        }
    }
    printf("Dijkstra distances from vertex %d:\n", start);
    for (int i = 0; i < g->num_vertices; i++) {
        if (dist[i] == INF) {
            printf("Vertex %d: INF\n", i);
        }