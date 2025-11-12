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

int stack[MAX_NODES];
int stack_top = -1;

void init_graph(Graph* g, int n) {
    if (n <= 0 || n > MAX_NODES) n = MAX_NODES;
    g->node_count = n;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            g->edges[i][j] = (i == j) ? 0 : INT_MAX;
        }
    }
}

void add_edge(Graph* g, int u, int v, int weight) {
    if (u < 0 || u >= g->node_count || v < 0 || v >= g->node_count) return;
    if (weight < 0) return;
    g->edges[u][v] = weight;
    g->edges[v][u] = weight;
}

void heapify_up(MinHeap* heap, int index) {
    if (index <= 0) return;
    int parent = (index - 1) / 2;
    if (heap->nodes[index].distance < heap->nodes[parent].distance) {
        HeapNode temp = heap->nodes[index];
        heap->nodes[index] = heap->nodes[parent];
        heap->nodes[parent] = temp;
        heapify_up(heap, parent);
    }
}

void heapify_down(MinHeap* heap, int index) {
    int left = 2 * index + 1;
    int right = 2 * index + 2;
    int smallest = index;
    
    if (left < heap->size && heap->nodes[left].distance < heap->nodes[smallest].distance) {
        smallest = left;
    }
    if (right < heap->size && heap->nodes[right].distance < heap->nodes[smallest].distance) {
        smallest = right;
    }
    
    if (smallest != index) {
        HeapNode temp = heap->nodes[index];
        heap->nodes[index] = heap->nodes[smallest];
        heap->nodes[smallest] = temp;
        heapify_down(heap, smallest);
    }
}

void heap_push(MinHeap* heap, int node, int distance) {
    if (heap->size >= MAX_NODES) return;
    heap->nodes[heap->size].node = node;
    heap->nodes[heap->size].distance = distance;
    heap->size++;
    heapify_up(heap, heap->size - 1);
}

HeapNode heap_pop(MinHeap* heap) {
    HeapNode empty = {-1, -1};
    if (heap->size <= 0) return empty;
    
    HeapNode result = heap->nodes[0];
    heap->nodes[0] = heap->nodes[heap->size - 1];
    heap->size--;
    heapify_down(heap, 0);
    return result;
}

int is_heap_empty(MinHeap* heap) {
    return heap->size == 0;
}

void dijkstra(Graph* g, int start, int distances[]) {
    if (start < 0 || start >= g->node_count) return;
    
    for (int i = 0; i < g->node_count; i++) {
        distances[i] = INT_MAX;
    }
    distances[start] = 0;
    
    MinHeap heap = {0};
    heap_push(&heap, start, 0);
    
    while (!is_heap_empty(&heap)) {
        HeapNode current = heap_pop(&heap);
        int u = current.node;
        
        for (int v = 0; v < g->node_count; v++) {
            if (g->edges[u][v] != INT_MAX && g->edges[u][v] >= 0) {
                int alt;
                if (distances[u] == INT_MAX) {
                    alt = INT_MAX;
                } else if (g->edges[u][v] > INT_MAX - distances[u]) {
                    alt = INT_MAX;
                } else {
                    alt = distances[u] + g->edges[u][v];
                }
                
                if (alt < distances[v] && alt >= 0) {
                    distances[v] = alt;
                    heap_push(&heap, v, alt);
                }
            }
        }
    }
}

void stack_push(int value) {
    if (stack_top < MAX_NODES - 1) {
        stack_top++;
        stack[stack_top] = value;
    }
}

int stack_pop() {
    if (stack_top >= 0) {
        int value = stack[stack_top];
        stack_top--;
        return value;
    }
    return -1;
}

int is_stack_empty() {
    return stack_top == -1;
}

void dfs(Graph* g, int start, int visited[]) {
    if (start < 0 || start >= g->node_count) return;
    
    for (int i = 0; i < g->node_count; i++) {
        visited[i] = 0;
    }
    
    stack_push(start);
    visited[start] = 1;
    
    while (!is_stack_empty()) {
        int current = stack_pop();
        printf("%d ", current);
        
        for (int