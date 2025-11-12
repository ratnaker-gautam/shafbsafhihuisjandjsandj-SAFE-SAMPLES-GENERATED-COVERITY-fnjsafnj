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
    HeapNode* arr;
    int size, capacity;
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

void free_heap(MinHeap* heap) {
    if (heap) {
        free(heap->arr);
        free(heap);
    }
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
    if (!heap || heap->size >= heap->capacity) return;
    heap->arr[heap->size].node = node;
    heap->arr[heap->size].dist = dist;
    heapify_up(heap, heap->size);
    heap->size++;
}

HeapNode heap_pop(MinHeap* heap) {
    HeapNode empty = {-1, -1};
    if (!heap || heap->size <= 0) return empty;
    HeapNode root = heap->arr[0];
    heap->arr[0] = heap->arr[heap->size - 1];
    heap->size--;
    heapify_down(heap, 0);
    return root;
}

int is_heap_empty(MinHeap* heap) {
    return !heap || heap->size == 0;
}

void dijkstra(int graph[MAX_NODES][MAX_NODES], int n, int start, int dist[MAX_NODES]) {
    int visited[MAX_NODES] = {0};
    for (int i = 0; i < n; i++) dist[i] = INT_MAX;
    dist[start] = 0;
    
    MinHeap* heap = create_heap(MAX_NODES);
    if (!heap) return;
    
    heap_push(heap, start, 0);
    
    while (!is_heap_empty(heap)) {
        HeapNode current = heap_pop(heap);
        int u = current.node;
        if (u == -1) continue;
        if (visited[u]) continue;
        visited[u] = 1;
        
        for (int v = 0; v < n; v++) {
            if (graph[u][v] > 0 && !visited[v]) {
                if (dist[u] < INT_MAX - graph[u][v]) {
                    int new_dist = dist[u] + graph[u][v];
                    if (new_dist < dist[v]) {
                        dist[v] = new_dist;
                        heap_push(heap, v, new_dist);
                    }
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
        if (scanf("%d %d %d", &u, &v, &w) != 3 || u < 0 || u >= n || v < 0 || v >= n || w <= 0) {
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
    dijkstra(graph