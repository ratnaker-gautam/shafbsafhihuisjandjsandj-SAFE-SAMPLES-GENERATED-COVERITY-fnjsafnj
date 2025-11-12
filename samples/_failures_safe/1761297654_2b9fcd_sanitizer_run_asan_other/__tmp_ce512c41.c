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

void initialize_graph(Graph* graph, int node_count) {
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

void add_edge(Graph* graph, int from, int to) {
    if (graph == NULL || from < 0 || from >= graph->node_count || 
        to < 0 || to >= graph->node_count || from == to) {
        return;
    }
    graph->adjacency[from][to] = 1;
    graph->adjacency[to][from] = 1;
}

void dfs_traversal(Graph* graph, int start, int visited[]) {
    if (graph == NULL || start < 0 || start >= graph->node_count || visited == NULL) {
        return;
    }
    visited[start] = 1;
    printf("%d ", start);
    for (int i = 0; i < graph->node_count; i++) {
        if (graph->adjacency[start][i] && !visited[i]) {
            dfs_traversal(graph, i, visited);
        }
    }
}

void bfs_traversal(Graph* graph, int start, int visited[]) {
    if (graph == NULL || start < 0 || start >= graph->node_count || visited == NULL) {
        return;
    }
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    visited[start] = 1;
    queue[rear++] = start;
    while (front < rear) {
        int current = queue[front++];
        printf("%d ", current);
        for (int i = 0; i < graph->node_count; i++) {
            if (graph->adjacency[current][i] && !visited[i]) {
                visited[i] = 1;
                queue[rear++] = i;
                if (rear >= MAX_NODES) {
                    rear = 0;
                }
            }
        }
    }
}

void execute_operation(Graph* graph, GraphOperation operation, int start) {
    if (graph == NULL || operation == NULL || start < 0 || start >= graph->node_count) {
        return;
    }
    int visited[MAX_NODES];
    for (int i = 0; i < graph->node_count; i++) {
        visited[i] = 0;
    }
    operation(graph, start, visited);
    printf("\n");
}

void print_menu() {
    printf("Graph Operations:\n");
    printf("1. DFS Traversal\n");
    printf("2. BFS Traversal\n");
    printf("3. Exit\n");
    printf("Enter choice: ");
}

int main() {
    Graph graph;
    initialize_graph(&graph, 6);
    add_edge(&graph, 0, 1);
    add_edge(&graph, 0, 2);
    add_edge(&graph, 1, 3);
    add_edge(&graph, 1, 4);
    add_edge(&graph, 2, 4);
    add_edge(&graph, 3, 5);
    add_edge(&graph, 4, 5);
    
    GraphOperation operations[] = {dfs_traversal, bfs_traversal};
    
    int choice;
    while (1) {
        print_menu();
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        if (choice == 3) {
            break;
        }
        
        if (choice < 1 || choice > 2) {
            printf("Invalid choice. Please try again.\n");
            continue;
        }
        
        printf("Enter starting node (0-5): ");
        int start_node;
        if (scanf("%d", &start_node) != 1) {
            while (getchar() != '\n');
            printf("Invalid input.\n");
            continue;
        }
        
        if (start_node < 0 || start_node >= graph.node_count) {
            printf("Invalid node. Must be between 0 and %d.\n", graph.node_count - 1);
            continue;
        }
        
        printf("Traversal: ");
        execute_operation(&graph, operations[choice - 1], start_node);
    }
    
    return 0;
}