//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: graph_algorithms
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
    HeapNode* nodes;
    int size;
    int capacity;
} MinHeap;

MinHeap* create_heap(int capacity) {
    MinHeap* heap = malloc(sizeof(MinHeap));
    heap->nodes = malloc(capacity * sizeof(HeapNode));
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
        if (heap->nodes[idx].dist >= heap->nodes[parent].dist) break;
        swap_nodes(&heap->nodes[idx], &heap->nodes[parent]);
        idx = parent;
    }
}

void heapify_down(MinHeap* heap, int idx) {
    while (1) {
        int left = 2 * idx + 1;
        int right = 2 * idx + 2;
        int smallest = idx;
        
        if (left < heap->size && heap->nodes[left].dist < heap->nodes[smallest].dist) {
            smallest = left;
        }
        if (right < heap->size && heap->nodes[right].dist < heap->nodes[smallest].dist) {
            smallest = right;
        }
        if (smallest == idx) break;
        swap_nodes(&heap->nodes[idx], &heap->nodes[smallest]);
        idx = smallest;
    }
}

void heap_push(MinHeap* heap, int node, int dist) {
    if (heap->size >= heap->capacity) return;
    heap->nodes[heap->size].node = node;
    heap->nodes[heap->size].dist = dist;
    heapify_up(heap, heap->size);
    heap->size++;
}

HeapNode heap_pop(MinHeap* heap) {
    HeapNode result = heap->nodes[0];
    heap->size--;
    heap->nodes[0] = heap->nodes[heap->size];
    heapify_down(heap, 0);
    return result;
}

int is_heap_empty(MinHeap* heap) {
    return heap->size == 0;
}

void free_heap(MinHeap* heap) {
    free(heap->nodes);
    free(heap);
}

int dijkstra(int graph[MAX_NODES][MAX_NODES], int n, int start, int end) {
    if (n <= 0 || n > MAX_NODES || start < 0 || start >= n || end < 0 || end >= n) {
        return -1;
    }
    
    int dist[MAX_NODES];
    int visited[MAX_NODES] = {0};
    
    for (int i = 0; i < n; i++) {
        dist[i] = INT_MAX;
    }
    dist[start] = 0;
    
    MinHeap* heap = create_heap(n);
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
                if (new_dist > 0 && new_dist < dist[v]) {
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
    int graph[MAX_NODES][MAX_NODES] = {0};
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
    
    printf("Enter edges as 'u v weight' (0-indexed):\n");
    for (int i = 0; i < m; i++) {
        int u, v, w;
        if (scanf("%d %d %d", &u, &v, &w) != 3) {
            printf("Invalid edge format\n");
            return 1;
        }
        if (u < 0 || u >= n || v < 0 || v >= n || w <= 0) {
            printf("Invalid edge parameters\n");
            return 1;
        }
        graph[u][v] = w;
        graph[v][u] = w;
    }
    
    int start, end;
    printf("