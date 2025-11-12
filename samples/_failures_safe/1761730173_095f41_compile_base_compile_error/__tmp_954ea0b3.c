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

void init_graph(Graph* g, int n) {
    g->node_count = n;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            g->edges[i][j] = (i == j) ? 0 : INT_MAX;
        }
    }
}

void add_edge(Graph* g, int u, int v, int weight) {
    if (u < 0 || u >= g->node_count || v < 0 || v >= g->node_count) return;
    if (weight < 0) return;
    g->edges[u][v] = weight;
    g->edges[v][u] = weight;
}

void heap_init(MinHeap* h) {
    h->size = 0;
}

void heap_swap(HeapNode* a, HeapNode* b) {
    HeapNode temp = *a;
    *a = *b;
    *b = temp;
}

void heap_up(MinHeap* h, int idx) {
    while (idx > 0) {
        int parent = (idx - 1) / 2;
        if (h->nodes[parent].distance <= h->nodes[idx].distance) break;
        heap_swap(&h->nodes[parent], &h->nodes[idx]);
        idx = parent;
    }
}

void heap_down(MinHeap* h, int idx) {
    while (1) {
        int left = 2 * idx + 1;
        int right = 2 * idx + 2;
        int smallest = idx;
        
        if (left < h->size && h->nodes[left].distance < h->nodes[smallest].distance) {
            smallest = left;
        }
        if (right < h->size && h->nodes[right].distance < h->nodes[smallest].distance) {
            smallest = right;
        }
        if (smallest == idx) break;
        heap_swap(&h->nodes[idx], &h->nodes[smallest]);
        idx = smallest;
    }
}

void heap_push(MinHeap* h, int node, int distance) {
    if (h->size >= MAX_NODES) return;
    h->nodes[h->size].node = node;
    h->nodes[h->size].distance = distance;
    heap_up(h, h->size);
    h->size++;
}

HeapNode heap_pop(MinHeap* h) {
    HeapNode result = h->nodes[0];
    h->size--;
    h->nodes[0] = h->nodes[h->size];
    heap_down(h, 0);
    return result;
}

int heap_empty(MinHeap* h) {
    return h->size == 0;
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
        HeapNode current = heap_pop(&heap);
        int u = current.node;
        
        if (visited[u]) continue;
        visited[u] = 1;
        
        for (int v = 0; v < g->node_count; v++) {
            if (g->edges[u][v] != INT_MAX && !visited[v]) {
                int alt = distances[u] + g->edges[u][v];
                if (alt >= 0 && alt < distances[v]) {
                    distances[v] = alt;
                    heap_push(&heap, v, alt);
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
    if (scanf("%d", &node_count) != 1 || node_count < 1 || node_count > MAX_NODES) {
        printf("Invalid number of nodes\n");
        return 1;
    }
    
    init_graph(&graph, node_count);
    
    printf("Enter number of edges: ");
    if (scanf("%d", &edge_count) != 1 || edge_count < 0 || edge_count > node_count * (node_count - 1) / 2) {
        printf("Invalid number of edges\n");
        return 1;
    }
    
    printf("Enter edges as 'u v weight' (0-indexed):\n");
    for (int i = 0; i < edge_count; i++) {
        int u, v, weight;
        if (scanf("%d %d %d", &u, &v, &weight) != 3) {
            printf("Invalid edge format\n");
            return 1;
        }
        if (u < 0 || u >= node_count || v < 0 || v >= node_count) {
            printf("Invalid node index\n");
            return 1;
        }
        if (weight < 0) {
            printf