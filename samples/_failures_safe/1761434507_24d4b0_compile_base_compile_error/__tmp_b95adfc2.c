//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 100

typedef struct {
    int edges[MAX_NODES][MAX_NODES];
    int node_count;
} Graph;

typedef struct {
    int node;
    int distance;
} HeapNode;

typedef struct {
    HeapNode nodes[MAX_NODES];
    int size;
} MinHeap;

void init_graph(Graph* g, int node_count) {
    if (node_count <= 0 || node_count > MAX_NODES) {
        node_count = MAX_NODES;
    }
    g->node_count = node_count;
    for (int i = 0; i < node_count; i++) {
        for (int j = 0; j < node_count; j++) {
            g->edges[i][j] = (i == j) ? 0 : INT_MAX;
        }
    }
}

void add_edge(Graph* g, int from, int to, int weight) {
    if (from < 0 || from >= g->node_count || to < 0 || to >= g->node_count) {
        return;
    }
    if (weight < 0) {
        return;
    }
    g->edges[from][to] = weight;
}

void heap_init(MinHeap* heap) {
    heap->size = 0;
}

void heap_swap(HeapNode* a, HeapNode* b) {
    HeapNode temp = *a;
    *a = *b;
    *b = temp;
}

void heap_up(MinHeap* heap, int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (heap->nodes[index].distance >= heap->nodes[parent].distance) {
            break;
        }
        heap_swap(&heap->nodes[index], &heap->nodes[parent]);
        index = parent;
    }
}

void heap_down(MinHeap* heap, int index) {
    int left, right, smallest;
    while (1) {
        left = 2 * index + 1;
        right = 2 * index + 2;
        smallest = index;
        
        if (left < heap->size && heap->nodes[left].distance < heap->nodes[smallest].distance) {
            smallest = left;
        }
        if (right < heap->size && heap->nodes[right].distance < heap->nodes[smallest].distance) {
            smallest = right;
        }
        if (smallest == index) {
            break;
        }
        heap_swap(&heap->nodes[index], &heap->nodes[smallest]);
        index = smallest;
    }
}

void heap_push(MinHeap* heap, int node, int distance) {
    if (heap->size >= MAX_NODES) {
        return;
    }
    heap->nodes[heap->size].node = node;
    heap->nodes[heap->size].distance = distance;
    heap_up(heap, heap->size);
    heap->size++;
}

int heap_pop(MinHeap* heap) {
    if (heap->size <= 0) {
        return -1;
    }
    int result = heap->nodes[0].node;
    heap->size--;
    if (heap->size > 0) {
        heap->nodes[0] = heap->nodes[heap->size];
        heap_down(heap, 0);
    }
    return result;
}

int heap_empty(MinHeap* heap) {
    return heap->size == 0;
}

void dijkstra(Graph* g, int start, int distances[]) {
    int visited[MAX_NODES] = {0};
    for (int i = 0; i < g->node_count; i++) {
        distances[i] = INT_MAX;
    }
    distances[start] = 0;
    
    MinHeap heap;
    heap_init(&heap);
    heap_push(&heap, start, 0);
    
    while (!heap_empty(&heap)) {
        int current = heap_pop(&heap);
        if (visited[current]) {
            continue;
        }
        visited[current] = 1;
        
        for (int neighbor = 0; neighbor < g->node_count; neighbor++) {
            if (g->edges[current][neighbor] != INT_MAX && !visited[neighbor]) {
                int new_dist = distances[current] + g->edges[current][neighbor];
                if (new_dist >= 0 && new_dist < distances[neighbor]) {
                    distances[neighbor] = new_dist;
                    heap_push(&heap, neighbor, new_dist);
                }
            }
        }
    }
}

int main() {
    Graph graph;
    int distances[MAX_NODES];
    int node_count, edge_count;
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &node_count) != 1 || node_count <= 0 || node_count > MAX_NODES) {
        printf("Invalid node count\n");
        return 1;
    }
    
    init_graph(&graph, node_count);
    
    printf("Enter number of edges: ");
    if (scanf("%d", &edge_count) != 1 || edge_count < 0) {
        printf("Invalid edge count\n");
        return 1;
    }
    
    printf("Enter edges as 'from to weight' (0-indexed):\n");
    for (int i = 0; i < edge_count; i++) {
        int from, to, weight;
        if (scanf("%d %d %d", &from, &to, &