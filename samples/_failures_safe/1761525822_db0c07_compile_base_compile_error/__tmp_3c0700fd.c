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
    int node;
    int dist;
} HeapNode;

typedef struct {
    HeapNode* arr;
    int size;
    int capacity;
} MinHeap;

MinHeap* createMinHeap(int capacity) {
    MinHeap* heap = (MinHeap*)malloc(sizeof(MinHeap));
    heap->arr = (HeapNode*)malloc(capacity * sizeof(HeapNode));
    heap->size = 0;
    heap->capacity = capacity;
    return heap;
}

void swapHeapNodes(HeapNode* a, HeapNode* b) {
    HeapNode temp = *a;
    *a = *b;
    *b = temp;
}

void minHeapify(MinHeap* heap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;
    if (left < heap->size && heap->arr[left].dist < heap->arr[smallest].dist) smallest = left;
    if (right < heap->size && heap->arr[right].dist < heap->arr[smallest].dist) smallest = right;
    if (smallest != idx) {
        swapHeapNodes(&heap->arr[idx], &heap->arr[smallest]);
        minHeapify(heap, smallest);
    }
}

HeapNode extractMin(MinHeap* heap) {
    if (heap->size <= 0) {
        HeapNode empty = {-1, INT_MAX};
        return empty;
    }
    HeapNode root = heap->arr[0];
    heap->arr[0] = heap->arr[heap->size - 1];
    heap->size--;
    minHeapify(heap, 0);
    return root;
}

void decreaseKey(MinHeap* heap, int node, int dist) {
    int i;
    for (i = 0; i < heap->size; i++) {
        if (heap->arr[i].node == node) {
            heap->arr[i].dist = dist;
            while (i && heap->arr[i].dist < heap->arr[(i - 1) / 2].dist) {
                swapHeapNodes(&heap->arr[i], &heap->arr[(i - 1) / 2]);
                i = (i - 1) / 2;
            }
            break;
        }
    }
}

int isInHeap(MinHeap* heap, int node) {
    for (int i = 0; i < heap->size; i++) {
        if (heap->arr[i].node == node) return 1;
    }
    return 0;
}

void dijkstra(int graph[MAX_NODES][MAX_NODES], int n, int src) {
    int dist[MAX_NODES];
    MinHeap* heap = createMinHeap(n);
    for (int i = 0; i < n; i++) {
        dist[i] = INT_MAX;
        heap->arr[i].node = i;
        heap->arr[i].dist = dist[i];
    }
    heap->size = n;
    dist[src] = 0;
    decreaseKey(heap, src, dist[src]);
    while (heap->size > 0) {
        HeapNode minNode = extractMin(heap);
        int u = minNode.node;
        for (int v = 0; v < n; v++) {
            if (graph[u][v] != 0 && isInHeap(heap, v)) {
                int weight = graph[u][v];
                if (dist[u] != INT_MAX && dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    decreaseKey(heap, v, dist[v]);
                }
            }
        }
    }
    printf("Shortest distances from node %d:\n", src);
    for (int i = 0; i < n; i++) {
        if (dist[i] == INT_MAX) printf("Node %d: unreachable\n", i);
        else printf("Node %d: %d\n", i, dist[i]);
    }
    free(heap->arr);
    free(heap);
}

int main() {
    int n, e;
    printf("Enter number of nodes (max %d): ", MAX_NODES);
    if (scanf("%d", &n) != 1 || n <= 0 || n > MAX_NODES) {
        printf("Invalid number of nodes\n");
        return 1;
    }
    printf("Enter number of edges: ");
    if (scanf("%d", &e) != 1 || e < 0 || e > MAX_EDGES) {
        printf("Invalid number of edges\n");
        return 1;
    }
    int graph[MAX_NODES][MAX_NODES];
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            graph[i][j] = 0;
        }
    }
    printf("Enter edges (u v weight):\n");
    for (int i = 0; i < e; i++) {
        int u, v, w;
        if (scanf("%d %d %d", &u, &v, &w) != 3 || u < 0 || u >= n || v < 0 || v >= n