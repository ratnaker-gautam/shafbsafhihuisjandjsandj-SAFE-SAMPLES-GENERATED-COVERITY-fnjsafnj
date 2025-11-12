//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: heap_stack_mix ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_NODES 100
#define MAX_EDGES 1000

typedef struct {
    int u, v, weight;
} Edge;

typedef struct {
    int node, dist;
} HeapNode;

typedef struct {
    HeapNode* arr;
    int size;
    int capacity;
} MinHeap;

typedef struct {
    Edge edges[MAX_EDGES];
    int edge_count;
    int node_count;
} Graph;

MinHeap* create_heap(int capacity) {
    MinHeap* heap = malloc(sizeof(MinHeap));
    if (!heap) return NULL;
    heap->arr = malloc(capacity * sizeof(HeapNode));
    if (!heap->arr) {
        free(heap);
        return NULL;
    }
    heap->size = 0;
    heap->capacity = capacity;
    return heap;
}

void swap_nodes(HeapNode* a, HeapNode* b) {
    HeapNode temp = *a;
    *a = *b;
    *b = temp;
}

void heapify_up(MinHeap* heap, int idx) {
    while (idx > 0) {
        int parent = (idx - 1) / 2;
        if (heap->arr[parent].dist <= heap->arr[idx].dist) break;
        swap_nodes(&heap->arr[parent], &heap->arr[idx]);
        idx = parent;
    }
}

void heapify_down(MinHeap* heap, int idx) {
    while (1) {
        int left = 2 * idx + 1;
        int right = 2 * idx + 2;
        int smallest = idx;
        
        if (left < heap->size && heap->arr[left].dist < heap->arr[smallest].dist)
            smallest = left;
        if (right < heap->size && heap->arr[right].dist < heap->arr[smallest].dist)
            smallest = right;
        
        if (smallest == idx) break;
        swap_nodes(&heap->arr[idx], &heap->arr[smallest]);
        idx = smallest;
    }
}

void heap_push(MinHeap* heap, int node, int dist) {
    if (heap->size >= heap->capacity) return;
    heap->arr[heap->size].node = node;
    heap->arr[heap->size].dist = dist;
    heapify_up(heap, heap->size);
    heap->size++;
}

HeapNode heap_pop(MinHeap* heap) {
    HeapNode empty = {-1, -1};
    if (heap->size <= 0) return empty;
    HeapNode root = heap->arr[0];
    heap->arr[0] = heap->arr[heap->size - 1];
    heap->size--;
    heapify_down(heap, 0);
    return root;
}

void init_graph(Graph* graph, int nodes) {
    if (nodes <= 0 || nodes > MAX_NODES) nodes = MAX_NODES;
    graph->node_count = nodes;
    graph->edge_count = 0;
}

int add_edge(Graph* graph, int u, int v, int weight) {
    if (u < 0 || u >= graph->node_count) return 0;
    if (v < 0 || v >= graph->node_count) return 0;
    if (graph->edge_count >= MAX_EDGES) return 0;
    if (weight < 0) return 0;
    
    graph->edges[graph->edge_count].u = u;
    graph->edges[graph->edge_count].v = v;
    graph->edges[graph->edge_count].weight = weight;
    graph->edge_count++;
    return 1;
}

void dijkstra(Graph* graph, int start, int distances[]) {
    if (start < 0 || start >= graph->node_count) return;
    
    for (int i = 0; i < graph->node_count; i++) {
        distances[i] = INT_MAX;
    }
    distances[start] = 0;
    
    MinHeap* heap = create_heap(graph->node_count);
    if (!heap) return;
    heap_push(heap, start, 0);
    
    while (heap->size > 0) {
        HeapNode current = heap_pop(heap);
        int u = current.node;
        
        for (int i = 0; i < graph->edge_count; i++) {
            Edge edge = graph->edges[i];
            if (edge.u != u) continue;
            
            int v = edge.v;
            if (distances[u] == INT_MAX) continue;
            if (edge.weight > INT_MAX - distances[u]) continue;
            int new_dist = distances[u] + edge.weight;
            if (new_dist >= 0 && new_dist < distances[v]) {
                distances[v] = new_dist;
                heap_push(heap, v, new_dist);
            }
        }
    }
    
    free(heap->arr);
    free(heap);
}

int main() {
    Graph graph;
    int distances[MAX_NODES];
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    int node_count;
    if (scanf("%d", &node_count) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (node_count <= 0 || node_count > MAX_NODES) {
        printf("Invalid node count\n");
        return 1;
    }
    
    init_graph(&graph, node_count);
    
    printf("Enter