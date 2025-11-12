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

void free_heap(MinHeap* heap) {
    if (heap) {
        free(heap->data);
        free(heap);
    }
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
    int left, right, smallest;
    while (1) {
        left = 2 * idx + 1;
        right = 2 * idx + 2;
        smallest = idx;
        if (left < heap->size && heap->data[left].dist < heap->data[smallest].dist) smallest = left;
        if (right < heap->size && heap->data[right].dist < heap->data[smallest].dist) smallest = right;
        if (smallest == idx) break;
        heap_swap(&heap->data[idx], &heap->data[smallest]);
        idx = smallest;
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
    HeapNode result;
    if (!heap || heap->size == 0) {
        result.node = -1;
        result.dist = -1;
        return result;
    }
    result = heap->data[0];
    heap->size--;
    if (heap->size > 0) {
        heap->data[0] = heap->data[heap->size];
        heapify_down(heap, 0);
    }
    return result;
}

int is_heap_empty(MinHeap* heap) {
    return !heap || heap->size == 0;
}

int read_int(const char* prompt, int min, int max) {
    int value;
    char buffer[100];
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) return -1;
        if (sscanf(buffer, "%d", &value) != 1) {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        if (value < min || value > max) {
            printf("Value must be between %d and %d.\n", min, max);
            continue;
        }
        return value;
    }
}

void dijkstra(int graph[MAX_NODES][MAX_NODES], int n, int start) {
    int dist[MAX_NODES];
    int visited[MAX_NODES] = {0};
    for (int i = 0; i < n; i++) dist[i] = INT_MAX;
    dist[start] = 0;
    MinHeap* heap = create_heap(n * n);
    if (!heap) return;
    heap_push(heap, start, 0);
    while (!is_heap_empty(heap)) {
        HeapNode current = heap_pop(heap);
        if (current.node == -1) break;
        int u = current.node;
        if (visited[u]) continue;
        visited[u] = 1;
        for (int v = 0; v < n; v++) {
            if (graph[u][v] != 0 && !visited[v]) {
                if (dist[u] < INT_MAX - graph[u][v]) {
                    int alt = dist[u] + graph[u][v];
                    if (alt < dist[v] && alt >= 0) {
                        dist[v] = alt;
                        heap_push(heap, v, alt);
                    }
                }
            }
        }
    }
    free_heap(heap);
    printf("Shortest distances from node %d:\n", start);
    for (int i = 0; i < n; i++) {
        if (dist[i] == INT_MAX) printf("Node %d: unreachable\n", i);
        else printf("Node %d: %d\n", i, dist[i]);
    }
}

int main() {
    int graph[MAX_NODES][MAX_NODES] = {0};
    int n = read_int("Enter number of nodes (1-100): ", 1,