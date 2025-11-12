//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 100

struct Graph {
    int numNodes;
    int adjMatrix[MAX_NODES][MAX_NODES];
};

void initializeGraph(struct Graph* graph, int numNodes) {
    if (numNodes <= 0 || numNodes > MAX_NODES) {
        fprintf(stderr, "Invalid number of nodes\n");
        exit(1);
    }
    
    graph->numNodes = numNodes;
    
    for (int i = 0; i < numNodes; i++) {
        for (int j = 0; j < numNodes; j++) {
            graph->adjMatrix[i][j] = 0;
        }
    }
}

void addEdge(struct Graph* graph, int src, int dest, int weight) {
    if (src < 0 || src >= graph->numNodes || dest < 0 || dest >= graph->numNodes) {
        fprintf(stderr, "Invalid node indices\n");
        return;
    }
    
    if (weight < 0) {
        fprintf(stderr, "Negative weights not allowed\n");
        return;
    }
    
    graph->adjMatrix[src][dest] = weight;
    graph->adjMatrix[dest][src] = weight;
}

void dijkstra(struct Graph* graph, int startNode) {
    if (startNode < 0 || startNode >= graph->numNodes) {
        fprintf(stderr, "Invalid start node\n");
        return;
    }
    
    int dist[MAX_NODES];
    int visited[MAX_NODES];
    
    for (int i = 0; i < graph->numNodes; i++) {
        dist[i] = INT_MAX;
        visited[i] = 0;
    }
    dist[startNode] = 0;
    
    int count = 0;
    while (count < graph->numNodes) {
        int minDist = INT_MAX;
        int minNode = -1;
        
        for (int i = 0; i < graph->numNodes; i++) {
            if (!visited[i] && dist[i] < minDist) {
                minDist = dist[i];
                minNode = i;
            }
        }
        
        if (minNode == -1) break;
        
        visited[minNode] = 1;
        count++;
        
        for (int i = 0; i < graph->numNodes; i++) {
            if (!visited[i] && graph->adjMatrix[minNode][i] != 0) {
                int newDist;
                if (dist[minNode] > INT_MAX - graph->adjMatrix[minNode][i]) {
                    continue;
                }
                newDist = dist[minNode] + graph->adjMatrix[minNode][i];
                if (newDist < dist[i]) {
                    dist[i] = newDist;
                }
            }
        }
    }
    
    printf("Shortest distances from node %d:\n", startNode);
    for (int i = 0; i < graph->numNodes; i++) {
        if (dist[i] == INT_MAX) {
            printf("Node %d: unreachable\n", i);
        } else {
            printf("Node %d: %d\n", i, dist[i]);
        }
    }
}

void bfs(struct Graph* graph, int startNode) {
    if (startNode < 0 || startNode >= graph->numNodes) {
        fprintf(stderr, "Invalid start node\n");
        return;
    }
    
    int visited[MAX_NODES] = {0};
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    
    visited[startNode] = 1;
    queue[rear++] = startNode;
    
    printf("BFS traversal from node %d: ", startNode);
    
    while (front < rear) {
        int current = queue[front++];
        printf("%d ", current);
        
        for (int i = 0; i < graph->numNodes; i++) {
            if (graph->adjMatrix[current][i] != 0 && !visited[i]) {
                visited[i] = 1;
                if (rear < MAX_NODES) {
                    queue[rear++] = i;
                }
            }
        }
    }
    printf("\n");
}

int main() {
    struct Graph graph;
    int numNodes, numEdges;
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &numNodes) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    initializeGraph(&graph, numNodes);
    
    printf("Enter number of edges: ");
    if (scanf("%d", &numEdges) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    printf("Enter edges as (source destination weight):\n");
    for (int i = 0; i < numEdges; i++) {
        int src, dest, weight;
        if (scanf("%d %d %d", &src, &dest, &weight) != 3) {
            fprintf(stderr, "Invalid edge input\n");
            return 1;
        }
        addEdge(&graph, src, dest, weight);
    }
    
    int startNode;
    printf("Enter start node for Dijkstra: ");
    if (scanf("%d", &startNode) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    dijkstra(&graph