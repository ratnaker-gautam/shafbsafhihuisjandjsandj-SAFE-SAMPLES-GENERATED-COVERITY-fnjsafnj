//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: function_pointers ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 100
#define MAX_EDGES 1000

typedef struct {
    int from;
    int to;
    int weight;
} Edge;

typedef struct {
    int node_count;
    int edge_count;
    Edge edges[MAX_EDGES];
    int adjacency[MAX_NODES][MAX_NODES];
} Graph;

typedef void (*GraphOperation)(Graph*, int);

void initialize_graph(Graph* graph, int node_count) {
    if (graph == NULL || node_count <= 0 || node_count > MAX_NODES) {
        return;
    }
    graph->node_count = node_count;
    graph->edge_count = 0;
    for (int i = 0; i < node_count; i++) {
        for (int j = 0; j < node_count; j++) {
            graph->adjacency[i][j] = 0;
        }
    }
}

int add_edge(Graph* graph, int from, int to, int weight) {
    if (graph == NULL || from < 0 || from >= graph->node_count || 
        to < 0 || to >= graph->node_count || graph->edge_count >= MAX_EDGES) {
        return 0;
    }
    graph->edges[graph->edge_count].from = from;
    graph->edges[graph->edge_count].to = to;
    graph->edges[graph->edge_count].weight = weight;
    graph->adjacency[from][to] = weight;
    graph->adjacency[to][from] = weight;
    graph->edge_count++;
    return 1;
}

void depth_first_search(Graph* graph, int start_node) {
    if (graph == NULL || start_node < 0 || start_node >= graph->node_count) {
        return;
    }
    int visited[MAX_NODES] = {0};
    int stack[MAX_NODES];
    int top = 0;
    stack[top++] = start_node;
    visited[start_node] = 1;
    printf("DFS traversal: ");
    while (top > 0) {
        int current = stack[--top];
        printf("%d ", current);
        for (int i = graph->node_count - 1; i >= 0; i--) {
            if (graph->adjacency[current][i] != 0 && !visited[i]) {
                visited[i] = 1;
                stack[top++] = i;
                if (top >= MAX_NODES) {
                    return;
                }
            }
        }
    }
    printf("\n");
}

void breadth_first_search(Graph* graph, int start_node) {
    if (graph == NULL || start_node < 0 || start_node >= graph->node_count) {
        return;
    }
    int visited[MAX_NODES] = {0};
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    queue[rear++] = start_node;
    visited[start_node] = 1;
    printf("BFS traversal: ");
    while (front < rear) {
        int current = queue[front++];
        printf("%d ", current);
        for (int i = 0; i < graph->node_count; i++) {
            if (graph->adjacency[current][i] != 0 && !visited[i]) {
                visited[i] = 1;
                queue[rear++] = i;
                if (rear >= MAX_NODES) {
                    return;
                }
            }
        }
    }
    printf("\n");
}

void find_connected_components(Graph* graph, int unused) {
    if (graph == NULL) {
        return;
    }
    int visited[MAX_NODES] = {0};
    int component_count = 0;
    for (int i = 0; i < graph->node_count; i++) {
        if (!visited[i]) {
            component_count++;
            printf("Component %d: ", component_count);
            int stack[MAX_NODES];
            int top = 0;
            stack[top++] = i;
            visited[i] = 1;
            while (top > 0) {
                int current = stack[--top];
                printf("%d ", current);
                for (int j = graph->node_count - 1; j >= 0; j--) {
                    if (graph->adjacency[current][j] != 0 && !visited[j]) {
                        visited[j] = 1;
                        stack[top++] = j;
                        if (top >= MAX_NODES) {
                            return;
                        }
                    }
                }
            }
            printf("\n");
        }
    }
}

void execute_graph_operation(Graph* graph, GraphOperation operation, int start_node) {
    if (graph == NULL || operation == NULL) {
        return;
    }
    operation(graph, start_node);
}

int main() {
    Graph graph;
    initialize_graph(&graph, 6);
    add_edge(&graph, 0, 1, 1);
    add_edge(&graph, 0, 2, 1);
    add_edge(&graph, 1, 3, 1);
    add_edge(&graph, 2, 3, 1);
    add_edge(&graph, 4, 5, 1);
    GraphOperation operations[] = {
        depth_first_search,
        breadth_first_search,
        find_connected_components
    };
    const char* operation_names[] = {
        "Depth First Search",
        "Breadth First Search",
        "Connected Components"
    };
    int operation_count