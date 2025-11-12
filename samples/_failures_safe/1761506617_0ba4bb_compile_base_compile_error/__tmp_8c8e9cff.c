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
    heapify_up(heap, heap->size);
    heap->size++;
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
    
    int visited[MAX_NODES] = {0};
    for (int i = 0; i < g->node_count; i++) {
        distances[i] = INT_MAX;
    }
    distances[start] = 0;
    
    MinHeap heap = {0};
    heap_push(&heap, start, 0);
    
    while (!is_heap_empty(&heap)) {
        HeapNode current = heap_pop(&heap);
        int u = current.node;
        
        if (visited[u]) continue;
        visited[u] = 1;
        
        for (int v = 0; v < g->node_count; v++) {
            if (g->edges[u][v] != INT_MAX && !visited[v]) {
                int alt;
                if (distances[u] < INT_MAX - g->edges[u][v]) {
                    alt = distances[u] + g->edges[u][v];
                } else {
                    alt = INT_MAX;
                }
                
                if (alt < distances[v]) {
                    distances[v] = alt;
                    heap_push(&heap, v, alt);
                }
            }
        }
    }
}

int main() {
    Graph graph;
    int distances[MAX_NODES];
    int node_count = 6;
    
    init_graph(&graph, node_count);
    
    add_edge(&graph, 0, 1, 4);
    add_edge(&graph, 0, 2, 2);
    add_edge(&graph, 1, 2, 1);
    add_edge(&graph, 1, 3, 5);
    add_edge(&graph, 2, 3, 8);
    add_edge(&graph, 2, 4, 10);
    add_edge(&graph, 3, 4, 2);
    add_edge(&graph, 3, 5, 6);
    add_edge(&graph, 4, 5, 3);
    
    int start_node = 0;
    dijkstra(&graph, start_node, distances);
    
    printf("Shortest distances from node %d:\n", start_node);
    for (int i = 0