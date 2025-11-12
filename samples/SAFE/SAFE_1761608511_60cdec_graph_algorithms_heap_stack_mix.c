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
    int node;
    int dist;
} HeapNode;

typedef struct {
    HeapNode* data;
    int size;
    int capacity;
} MinHeap;

MinHeap* create_heap(int capacity) {
    MinHeap* heap = malloc(sizeof(MinHeap));
    heap->data = malloc(capacity * sizeof(HeapNode));
    heap->size = 0;
    heap->capacity = capacity;
    return heap;
}

void free_heap(MinHeap* heap) {
    free(heap->data);
    free(heap);
}

void heap_swap(HeapNode* a, HeapNode* b) {
    HeapNode temp = *a;
    *a = *b;
    *b = temp;
}

void heapify_up(MinHeap* heap, int idx) {
    while (idx > 0) {
        int parent = (idx - 1) / 2;
        if (heap->data[parent].dist <= heap->data[idx].dist) break;
        heap_swap(&heap->data[parent], &heap->data[idx]);
        idx = parent;
    }
}

void heapify_down(MinHeap* heap, int idx) {
    while (1) {
        int left = 2 * idx + 1;
        int right = 2 * idx + 2;
        int smallest = idx;
        
        if (left < heap->size && heap->data[left].dist < heap->data[smallest].dist) {
            smallest = left;
        }
        if (right < heap->size && heap->data[right].dist < heap->data[smallest].dist) {
            smallest = right;
        }
        if (smallest == idx) break;
        heap_swap(&heap->data[idx], &heap->data[smallest]);
        idx = smallest;
    }
}

void heap_push(MinHeap* heap, int node, int dist) {
    if (heap->size >= heap->capacity) return;
    heap->data[heap->size].node = node;
    heap->data[heap->size].dist = dist;
    heapify_up(heap, heap->size);
    heap->size++;
}

HeapNode heap_pop(MinHeap* heap) {
    HeapNode result = heap->data[0];
    heap->size--;
    heap->data[0] = heap->data[heap->size];
    heapify_down(heap, 0);
    return result;
}

int is_heap_empty(MinHeap* heap) {
    return heap->size == 0;
}

void dijkstra(int graph[MAX_NODES][MAX_NODES], int n, int start, int dist[MAX_NODES]) {
    for (int i = 0; i < n; i++) {
        dist[i] = INT_MAX;
    }
    dist[start] = 0;
    
    MinHeap* heap = create_heap(n);
    heap_push(heap, start, 0);
    
    while (!is_heap_empty(heap)) {
        HeapNode current = heap_pop(heap);
        int u = current.node;
        
        if (current.dist != dist[u]) continue;
        
        for (int v = 0; v < n; v++) {
            if (graph[u][v] != 0) {
                int new_dist = dist[u] + graph[u][v];
                if (new_dist >= 0 && new_dist < dist[v]) {
                    dist[v] = new_dist;
                    heap_push(heap, v, new_dist);
                }
            }
        }
    }
    free_heap(heap);
}

int main() {
    int n, m;
    printf("Enter number of nodes and edges: ");
    if (scanf("%d %d", &n, &m) != 2 || n <= 0 || n > MAX_NODES || m < 0 || m > MAX_EDGES) {
        printf("Invalid input\n");
        return 1;
    }
    
    int graph[MAX_NODES][MAX_NODES] = {0};
    printf("Enter edges (u v weight):\n");
    for (int i = 0; i < m; i++) {
        int u, v, w;
        if (scanf("%d %d %d", &u, &v, &w) != 3 || u < 0 || u >= n || v < 0 || v >= n || w < 0) {
            printf("Invalid edge\n");
            return 1;
        }
        graph[u][v] = w;
        graph[v][u] = w;
    }
    
    int start;
    printf("Enter start node: ");
    if (scanf("%d", &start) != 1 || start < 0 || start >= n) {
        printf("Invalid start node\n");
        return 1;
    }
    
    int dist[MAX_NODES];
    dijkstra(graph, n, start, dist);
    
    printf("Shortest distances from node %d:\n", start);
    for (int i = 0; i < n; i++) {
        if (dist[i] == INT_MAX) {
            printf("Node %d: unreachable\n", i);
        } else {
            printf("Node %d: %d\n", i, dist[i]);
        }
    }
    
    return 0;
}