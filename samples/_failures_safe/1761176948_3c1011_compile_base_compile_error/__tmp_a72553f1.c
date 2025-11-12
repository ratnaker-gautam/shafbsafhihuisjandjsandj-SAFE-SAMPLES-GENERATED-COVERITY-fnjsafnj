//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_NODES 100
#define MAX_EDGES 1000

typedef struct {
    int u, v, weight;
} Edge;

typedef struct {
    Edge edges[MAX_EDGES];
    int count;
} EdgeList;

typedef struct {
    int node;
    int distance;
} HeapNode;

typedef struct {
    HeapNode nodes[MAX_NODES];
    int size;
    int capacity;
} MinHeap;

void heap_init(MinHeap *heap, int capacity) {
    heap->size = 0;
    heap->capacity = capacity;
}

void heap_swap(HeapNode *a, HeapNode *b) {
    HeapNode temp = *a;
    *a = *b;
    *b = temp;
}

void heap_up(MinHeap *heap, int idx) {
    while (idx > 0) {
        int parent = (idx - 1) / 2;
        if (heap->nodes[parent].distance <= heap->nodes[idx].distance) break;
        heap_swap(&heap->nodes[parent], &heap->nodes[idx]);
        idx = parent;
    }
}

void heap_down(MinHeap *heap, int idx) {
    while (1) {
        int left = 2 * idx + 1;
        int right = 2 * idx + 2;
        int smallest = idx;
        
        if (left < heap->size && heap->nodes[left].distance < heap->nodes[smallest].distance) {
            smallest = left;
        }
        if (right < heap->size && heap->nodes[right].distance < heap->nodes[smallest].distance) {
            smallest = right;
        }
        if (smallest == idx) break;
        heap_swap(&heap->nodes[idx], &heap->nodes[smallest]);
        idx = smallest;
    }
}

void heap_push(MinHeap *heap, int node, int distance) {
    if (heap->size >= heap->capacity) return;
    heap->nodes[heap->size].node = node;
    heap->nodes[heap->size].distance = distance;
    heap_up(heap, heap->size);
    heap->size++;
}

HeapNode heap_pop(MinHeap *heap) {
    HeapNode empty = {-1, -1};
    if (heap->size <= 0) return empty;
    HeapNode result = heap->nodes[0];
    heap->size--;
    if (heap->size > 0) {
        heap->nodes[0] = heap->nodes[heap->size];
        heap_down(heap, 0);
    }
    return result;
}

int heap_empty(MinHeap *heap) {
    return heap->size == 0;
}

void dijkstra(EdgeList *graph, int start, int n, int distances[]) {
    MinHeap heap;
    heap_init(&heap, n);
    
    for (int i = 0; i < n; i++) {
        distances[i] = INT_MAX;
    }
    distances[start] = 0;
    heap_push(&heap, start, 0);
    
    while (!heap_empty(&heap)) {
        HeapNode current = heap_pop(&heap);
        int u = current.node;
        if (current.distance != distances[u]) continue;
        
        for (int i = 0; i < graph->count; i++) {
            Edge edge = graph->edges[i];
            if (edge.u == u) {
                int v = edge.v;
                int new_dist = distances[u] + edge.weight;
                if (new_dist >= 0 && new_dist < distances[v]) {
                    distances[v] = new_dist;
                    heap_push(&heap, v, new_dist);
                }
            }
        }
    }
}

int main() {
    EdgeList graph;
    graph.count = 0;
    
    int n, m;
    printf("Enter number of nodes and edges: ");
    if (scanf("%d %d", &n, &m) != 2 || n <= 0 || n > MAX_NODES || m < 0 || m > MAX_EDGES) {
        printf("Invalid input\n");
        return 1;
    }
    
    printf("Enter edges (u v weight):\n");
    for (int i = 0; i < m; i++) {
        int u, v, w;
        if (scanf("%d %d %d", &u, &v, &w) != 3 || u < 0 || u >= n || v < 0 || v >= n || w < 0) {
            printf("Invalid edge\n");
            return 1;
        }
        if (graph.count >= MAX_EDGES) {
            printf("Too many edges\n");
            return 1;
        }
        graph.edges[graph.count].u = u;
        graph.edges[graph.count].v = v;
        graph.edges[graph.count].weight = w;
        graph.count++;
    }
    
    int start;
    printf("Enter start node: ");
    if (scanf("%d", &start) != 1 || start < 0 || start >= n) {
        printf("Invalid start node\n");
        return 1;
    }
    
    int distances[MAX_NODES];
    dijkstra(&graph, start, n, distances);
    
    printf("Shortest distances from node %d:\n", start);
    for (int i = 0; i < n; i++) {
        if (distances[i] == INT_MAX)