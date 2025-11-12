//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_NODES 100

typedef struct {
    int adjacency[MAX_NODES][MAX_NODES];
    int node_count;
} Graph;

typedef void (*GraphOperation)(Graph*, int, int);

void initialize_graph(Graph *graph, int node_count) {
    if (graph == NULL || node_count <= 0 || node_count > MAX_NODES) {
        return;
    }
    graph->node_count = node_count;
    for (int i = 0; i < node_count; i++) {
        for (int j = 0; j < node_count; j++) {
            graph->adjacency[i][j] = 0;
        }
    }
}

int validate_node(Graph *graph, int node) {
    return (graph != NULL && node >= 0 && node < graph->node_count);
}

void add_edge(Graph *graph, int from, int to) {
    if (!validate_node(graph, from) || !validate_node(graph, to)) {
        return;
    }
    graph->adjacency[from][to] = 1;
    graph->adjacency[to][from] = 1;
}

void dfs_util(Graph *graph, int node, int visited[]) {
    if (graph == NULL || visited == NULL || !validate_node(graph, node)) {
        return;
    }
    visited[node] = 1;
    printf("%d ", node);
    for (int i = 0; i < graph->node_count; i++) {
        if (graph->adjacency[node][i] && !visited[i]) {
            dfs_util(graph, i, visited);
        }
    }
}

void depth_first_search(Graph *graph, int start, int unused) {
    if (graph == NULL || !validate_node(graph, start)) {
        return;
    }
    int visited[MAX_NODES] = {0};
    printf("DFS traversal: ");
    dfs_util(graph, start, visited);
    printf("\n");
}

void breadth_first_search(Graph *graph, int start, int unused) {
    if (graph == NULL || !validate_node(graph, start)) {
        return;
    }
    int visited[MAX_NODES] = {0};
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    visited[start] = 1;
    queue[rear++] = start;
    printf("BFS traversal: ");
    while (front < rear) {
        int current = queue[front++];
        printf("%d ", current);
        for (int i = 0; i < graph->node_count; i++) {
            if (graph->adjacency[current][i] && !visited[i]) {
                visited[i] = 1;
                if (rear < MAX_NODES) {
                    queue[rear++] = i;
                }
            }
        }
    }
    printf("\n");
}

void find_shortest_path(Graph *graph, int start, int end) {
    if (graph == NULL || !validate_node(graph, start) || !validate_node(graph, end)) {
        return;
    }
    int visited[MAX_NODES] = {0};
    int distance[MAX_NODES];
    int previous[MAX_NODES];
    for (int i = 0; i < graph->node_count; i++) {
        distance[i] = INT_MAX;
        previous[i] = -1;
    }
    distance[start] = 0;
    for (int count = 0; count < graph->node_count - 1; count++) {
        int min_distance = INT_MAX;
        int min_node = -1;
        for (int i = 0; i < graph->node_count; i++) {
            if (!visited[i] && distance[i] < min_distance) {
                min_distance = distance[i];
                min_node = i;
            }
        }
        if (min_node == -1) break;
        visited[min_node] = 1;
        for (int i = 0; i < graph->node_count; i++) {
            if (graph->adjacency[min_node][i] && !visited[i]) {
                int new_dist = distance[min_node];
                if (new_dist != INT_MAX) {
                    new_dist += 1;
                    if (new_dist < distance[i]) {
                        distance[i] = new_dist;
                        previous[i] = min_node;
                    }
                }
            }
        }
    }
    if (distance[end] == INT_MAX) {
        printf("No path between %d and %d\n", start, end);
    } else {
        printf("Shortest path from %d to %d: ", start, end);
        int path[MAX_NODES];
        int path_length = 0;
        int current = end;
        while (current != -1) {
            path[path_length++] = current;
            current = previous[current];
        }
        for (int i = path_length - 1; i >= 0; i--) {
            printf("%d", path[i]);
            if (i > 0) printf(" -> ");
        }
        printf(" (distance: %d)\n", distance[end]);
    }
}

void execute_operation(Graph *graph, GraphOperation operation, int param1, int param2) {
    if (graph == NULL || operation == NULL) {
        return;
    }
    operation(graph, param1, param2);
}

int main() {
    Graph graph;
    initialize_graph(&graph, 8);
    add_edge(&graph, 0, 1);
    add