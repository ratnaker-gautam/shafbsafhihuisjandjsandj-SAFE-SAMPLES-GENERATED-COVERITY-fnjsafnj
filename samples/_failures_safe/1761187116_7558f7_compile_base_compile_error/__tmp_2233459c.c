//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_NODES 100
#define MAX_EDGES 1000

typedef struct {
    int node, dist;
} HeapNode;

typedef struct {
    HeapNode* data;
    int size, capacity;
} MinHeap;

MinHeap* create_heap(int capacity) {
    MinHeap* heap = malloc(sizeof(MinHeap));
    if (!heap) return NULL;
    heap->data = malloc(capacity * sizeof(HeapNode));
    if (!heap->data) {
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
    while (idx > 0 && heap->data[(idx - 1) / 2].dist > heap->data[idx].dist) {
        swap_nodes(&heap->data[(idx - 1) / 2], &heap->data[idx]);
        idx = (idx - 1) / 2;
    }
}

void heapify_down(MinHeap* heap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;
    
    if (left < heap->size && heap->data[left].dist < heap->data[smallest].dist) {
        smallest = left;
    }
    if (right < heap->size && heap->data[right].dist < heap->data[smallest].dist) {
        smallest = right;
    }
    if (smallest != idx) {
        swap_nodes(&heap->data[idx], &heap->data[smallest]);
        heapify_down(heap, smallest);
    }
}

void heap_push(MinHeap* heap, int node, int dist) {
    if (!heap || heap->size >= heap->capacity) return;
    heap->data[heap->size].node = node;
    heap->data[heap->size].dist = dist;
    heapify_up(heap, heap->size);
    heap->size++;
}

HeapNode heap_pop(MinHeap* heap) {
    HeapNode empty = {-1, -1};
    if (!heap || heap->size <= 0) return empty;
    HeapNode root = heap->data[0];
    heap->data[0] = heap->data[heap->size - 1];
    heap->size--;
    heapify_down(heap, 0);
    return root;
}

int is_heap_empty(MinHeap* heap) {
    return !heap || heap->size == 0;
}

void free_heap(MinHeap* heap) {
    if (!heap) return;
    free(heap->data);
    free(heap);
}

int dijkstra(int graph[MAX_NODES][MAX_NODES], int n, int start, int end) {
    if (n <= 0 || n > MAX_NODES || start < 0 || start >= n || end < 0 || end >= n) {
        return -1;
    }
    
    int dist[MAX_NODES];
    int visited[MAX_NODES];
    
    for (int i = 0; i < n; i++) {
        dist[i] = INT_MAX;
        visited[i] = 0;
    }
    dist[start] = 0;
    
    MinHeap* heap = create_heap(MAX_NODES);
    if (!heap) return -1;
    heap_push(heap, start, 0);
    
    while (!is_heap_empty(heap)) {
        HeapNode current = heap_pop(heap);
        int u = current.node;
        
        if (visited[u]) continue;
        visited[u] = 1;
        
        if (u == end) {
            free_heap(heap);
            return dist[u];
        }
        
        for (int v = 0; v < n; v++) {
            if (graph[u][v] > 0 && !visited[v]) {
                int new_dist = dist[u] + graph[u][v];
                if (new_dist < dist[v] && new_dist >= 0) {
                    dist[v] = new_dist;
                    heap_push(heap, v, new_dist);
                }
            }
        }
    }
    
    free_heap(heap);
    return -1;
}

int main() {
    int graph[MAX_NODES][MAX_NODES];
    int n, m;
    
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
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            graph[i][j] = 0;
        }
    }
    
    printf("Enter edges as 'u v weight' (0-indexed):\n");
    for (int i = 0; i < m; i++) {
        int u, v, w