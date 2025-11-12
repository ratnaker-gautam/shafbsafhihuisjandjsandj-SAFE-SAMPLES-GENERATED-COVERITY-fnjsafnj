//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 100
#define INF INT_MAX

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
    if (!heap) return NULL;
    heap->arr = (HeapNode*)malloc(capacity * sizeof(HeapNode));
    if (!heap->arr) {
        free(heap);
        return NULL;
    }
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

    if (left < heap->size && heap->arr[left].dist < heap->arr[smallest].dist)
        smallest = left;
    if (right < heap->size && heap->arr[right].dist < heap->arr[smallest].dist)
        smallest = right;

    if (smallest != idx) {
        swapHeapNodes(&heap->arr[idx], &heap->arr[smallest]);
        minHeapify(heap, smallest);
    }
}

HeapNode extractMin(MinHeap* heap) {
    if (heap->size <= 0) {
        HeapNode empty = {-1, INF};
        return empty;
    }
    if (heap->size == 1) {
        heap->size--;
        return heap->arr[0];
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
            break;
        }
    }

    while (i != 0 && heap->arr[(i - 1) / 2].dist > heap->arr[i].dist) {
        swapHeapNodes(&heap->arr[i], &heap->arr[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

int isInHeap(MinHeap* heap, int node) {
    for (int i = 0; i < heap->size; i++) {
        if (heap->arr[i].node == node)
            return 1;
    }
    return 0;
}

void dijkstra(int graph[MAX_NODES][MAX_NODES], int n, int src) {
    if (n <= 0 || n > MAX_NODES || src < 0 || src >= n) {
        printf("Invalid input parameters\n");
        return;
    }

    int dist[MAX_NODES];
    MinHeap* heap = createMinHeap(n);
    if (!heap) {
        printf("Memory allocation failed\n");
        return;
    }

    for (int i = 0; i < n; i++) {
        dist[i] = INF;
        heap->arr[i].node = i;
        heap->arr[i].dist = INF;
    }
    heap->size = n;

    dist[src] = 0;
    decreaseKey(heap, src, dist[src]);

    while (heap->size != 0) {
        HeapNode minNode = extractMin(heap);
        int u = minNode.node;

        for (int v = 0; v < n; v++) {
            if (graph[u][v] != 0) {
                if (dist[u] != INF && graph[u][v] > 0) {
                    if (dist[u] <= INF - graph[u][v]) {
                        int new_dist = dist[u] + graph[u][v];
                        if (new_dist > 0 && new_dist < dist[v]) {
                            dist[v] = new_dist;
                            decreaseKey(heap, v, dist[v]);
                        }
                    }
                }
            }
        }
    }

    printf("Vertex Distance from Source %d:\n", src);
    for (int i = 0; i < n; i++) {
        if (dist[i] == INF)
            printf("%d \t INF\n", i);
        else
            printf("%d \t %d\n", i, dist[i]);
    }

    free(heap->arr);
    free(heap);
}

int main() {
    int n, src;
    printf("Enter number of vertices (1-%d): ", MAX_NODES);
    if (scanf("%d", &n) != 1 || n < 1 || n > MAX_NODES) {
        printf("Invalid number of vertices\n");
        return 1;
    }

    int graph[MAX_NODES][MAX_NODES];
    printf("Enter adjacency matrix (%dx%d):\n", n, n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (scanf("%d", &graph[i][j])