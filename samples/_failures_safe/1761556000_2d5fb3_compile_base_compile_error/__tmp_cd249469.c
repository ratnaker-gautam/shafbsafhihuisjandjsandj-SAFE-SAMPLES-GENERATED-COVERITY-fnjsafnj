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
    HeapNode* arr;
    int size;
    int capacity;
} MinHeap;

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
        
        if (left < heap->size && heap->arr[left].dist < heap->arr[smallest].dist) {
            smallest = left;
        }
        if (right < heap->size && heap->arr[right].dist < heap->arr[smallest].dist) {
            smallest = right;
        }
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

int is_heap_empty(MinHeap* heap) {
    return heap->size == 0;
}

void dijkstra(int graph[MAX_NODES][MAX_NODES], int n, int start, int distances[MAX_NODES]) {
    int visited[MAX_NODES] = {0};
    for (int i = 0; i < n; i++) {
        distances[i] = INT_MAX;
    }
    distances[start] = 0;
    
    MinHeap* heap = create_heap(MAX_NODES);
    if (!heap) return;
    heap_push(heap, start, 0);
    
    while (!is_heap_empty(heap)) {
        HeapNode current = heap_pop(heap);
        int u = current.node;
        if (visited[u]) continue;
        visited[u] = 1;
        
        for (int v = 0; v < n; v++) {
            if (graph[u][v] > 0 && !visited[v]) {
                int new_dist = distances[u] + graph[u][v];
                if (new_dist < distances[v]) {
                    distances[v] = new_dist;
                    heap_push(heap, v, new_dist);
                }
            }
        }
    }
    
    free(heap->arr);
    free(heap);
}

int main() {
    int graph[MAX_NODES][MAX_NODES] = {0};
    int distances[MAX_NODES];
    int n, m, start;
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &n) != 1 || n < 1 || n > MAX_NODES) {
        printf("Invalid number of nodes\n");
        return 1;
    }
    
    printf("Enter number of edges: ");
    if (scanf("%d", &m) != 1 || m < 0 || m > MAX_EDGES) {
        printf("Invalid number of edges\n");
        return 1;
    }
    
    printf("Enter edges (u v weight):\n");
    for (int i = 0; i < m; i++) {
        int u, v, w;
        if (scanf("%d %d %d", &u, &v, &w) != 3) {
            printf("Invalid edge format\n");
            return 1;
        }
        if (u < 0 || u >= n || v < 0 || v >= n || w < 0) {
            printf("Invalid edge parameters\n");
            return 1;
        }
        graph[u][v] = w;
        graph[v][u] = w;
    }
    
    printf("Enter start node (0-%d): ", n-1);
    if (scanf("%d", &start) != 1 ||