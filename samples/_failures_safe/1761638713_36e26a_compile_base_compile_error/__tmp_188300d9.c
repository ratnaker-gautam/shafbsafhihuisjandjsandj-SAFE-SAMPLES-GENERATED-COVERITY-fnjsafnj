//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 100

typedef struct {
    int edges[MAX_NODES][MAX_NODES];
    int node_count;
} Graph;

typedef struct {
    int node;
    int distance;
} HeapNode;

typedef struct {
    HeapNode nodes[MAX_NODES];
    int size;
} MinHeap;

void init_graph(Graph* g, int node_count) {
    if (node_count <= 0 || node_count > MAX_NODES) {
        node_count = MAX_NODES;
    }
    g->node_count = node_count;
    for (int i = 0; i < node_count; i++) {
        for (int j = 0; j < node_count; j++) {
            g->edges[i][j] = (i == j) ? 0 : INT_MAX;
        }
    }
}

void add_edge(Graph* g, int from, int to, int weight) {
    if (from < 0 || from >= g->node_count || to < 0 || to >= g->node_count) {
        return;
    }
    if (weight < 0) {
        return;
    }
    g->edges[from][to] = weight;
}

void heap_init(MinHeap* h) {
    h->size = 0;
}

void heap_swap(HeapNode* a, HeapNode* b) {
    HeapNode temp = *a;
    *a = *b;
    *b = temp;
}

void heap_up(MinHeap* h, int idx) {
    while (idx > 0) {
        int parent = (idx - 1) / 2;
        if (h->nodes[parent].distance <= h->nodes[idx].distance) {
            break;
        }
        heap_swap(&h->nodes[parent], &h->nodes[idx]);
        idx = parent;
    }
}

void heap_down(MinHeap* h, int idx) {
    while (1) {
        int left = 2 * idx + 1;
        int right = 2 * idx + 2;
        int smallest = idx;
        
        if (left < h->size && h->nodes[left].distance < h->nodes[smallest].distance) {
            smallest = left;
        }
        if (right < h->size && h->nodes[right].distance < h->nodes[smallest].distance) {
            smallest = right;
        }
        if (smallest == idx) {
            break;
        }
        heap_swap(&h->nodes[idx], &h->nodes[smallest]);
        idx = smallest;
    }
}

void heap_push(MinHeap* h, int node, int distance) {
    if (h->size >= MAX_NODES) {
        return;
    }
    h->nodes[h->size].node = node;
    h->nodes[h->size].distance = distance;
    heap_up(h, h->size);
    h->size++;
}

int heap_pop(MinHeap* h) {
    if (h->size <= 0) {
        return -1;
    }
    int node = h->nodes[0].node;
    h->size--;
    if (h->size > 0) {
        h->nodes[0] = h->nodes[h->size];
        heap_down(h, 0);
    }
    return node;
}

void dijkstra(Graph* g, int start, int distances[]) {
    if (start < 0 || start >= g->node_count) {
        return;
    }
    
    int visited[MAX_NODES] = {0};
    for (int i = 0; i < g->node_count; i++) {
        distances[i] = INT_MAX;
    }
    distances[start] = 0;
    
    MinHeap heap;
    heap_init(&heap);
    heap_push(&heap, start, 0);
    
    while (heap.size > 0) {
        int current = heap_pop(&heap);
        if (current == -1) {
            break;
        }
        if (visited[current]) {
            continue;
        }
        visited[current] = 1;
        
        for (int neighbor = 0; neighbor < g->node_count; neighbor++) {
            if (g->edges[current][neighbor] == INT_MAX) {
                continue;
            }
            if (visited[neighbor]) {
                continue;
            }
            int new_dist;
            if (distances[current] < INT_MAX - g->edges[current][neighbor]) {
                new_dist = distances[current] + g->edges[current][neighbor];
            } else {
                new_dist = INT_MAX;
            }
            if (new_dist < distances[neighbor]) {
                distances[neighbor] = new_dist;
                heap_push(&heap, neighbor, new_dist);
            }
        }
    }
}

int main() {
    Graph graph;
    int distances[MAX_NODES];
    
    init_graph(&graph, 6);
    
    add_edge(&graph, 0, 1, 4);
    add_edge(&graph, 0, 2, 2);
    add_edge(&graph, 1, 2, 1);
    add_edge(&graph, 1, 3, 5);
    add_edge(&graph, 2, 3, 8);
    add_edge(&graph, 2, 4, 10);
    add_edge(&graph, 3, 4, 2);
    add_edge(&graph, 3, 5, 6);
    add_edge(&graph, 4, 5,