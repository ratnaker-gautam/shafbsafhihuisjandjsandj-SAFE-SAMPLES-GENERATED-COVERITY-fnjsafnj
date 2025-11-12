//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

typedef struct {
    char timestamp[32];
    char level[16];
    char message[256];
    int severity;
} LogEntry;

typedef struct {
    LogEntry *entries;
    int count;
    int capacity;
} LogHeap;

typedef struct {
    LogEntry entries[MAX_ENTRIES];
    int top;
} LogStack;

void init_heap(LogHeap *heap, int capacity) {
    if (capacity <= 0 || capacity > 10000) capacity = 100;
    heap->entries = malloc(capacity * sizeof(LogEntry));
    if (heap->entries == NULL) exit(1);
    heap->count = 0;
    heap->capacity = capacity;
}

void free_heap(LogHeap *heap) {
    if (heap->entries != NULL) {
        free(heap->entries);
        heap->entries = NULL;
    }
    heap->count = 0;
    heap->capacity = 0;
}

int add_to_heap(LogHeap *heap, const char *timestamp, const char *level, const char *message) {
    if (heap == NULL || timestamp == NULL || level == NULL || message == NULL) return 0;
    if (heap->count >= heap->capacity) return 0;
    
    LogEntry *entry = &heap->entries[heap->count];
    
    strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp)-1);
    entry->timestamp[sizeof(entry->timestamp)-1] = '\0';
    
    strncpy(entry->level, level, sizeof(entry->level)-1);
    entry->level[sizeof(entry->level)-1] = '\0';
    
    strncpy(entry->message, message, sizeof(entry->message)-1);
    entry->message[sizeof(entry->message)-1] = '\0';
    
    if (strcmp(level, "ERROR") == 0) entry->severity = 3;
    else if (strcmp(level, "WARN") == 0) entry->severity = 2;
    else if (strcmp(level, "INFO") == 0) entry->severity = 1;
    else entry->severity = 0;
    
    heap->count++;
    return 1;
}

void init_stack(LogStack *stack) {
    stack->top = -1;
}

int push_to_stack(LogStack *stack, const LogEntry *entry) {
    if (stack == NULL || entry == NULL) return 0;
    if (stack->top >= MAX_ENTRIES - 1) return 0;
    
    stack->top++;
    stack->entries[stack->top] = *entry;
    return 1;
}

int pop_from_stack(LogStack *stack, LogEntry *entry) {
    if (stack == NULL || entry == NULL) return 0;
    if (stack->top < 0) return 0;
    
    *entry = stack->entries[stack->top];
    stack->top--;
    return 1;
}

void analyze_logs(LogHeap *heap, LogStack *stack) {
    if (heap == NULL || stack == NULL) return;
    
    for (int i = 0; i < heap->count; i++) {
        if (heap->entries[i].severity >= 2) {
            if (!push_to_stack(stack, &heap->entries[i])) {
                break;
            }
        }
    }
}

void print_recent_errors(LogStack *stack) {
    if (stack == NULL) return;
    
    printf("Recent high severity logs (most recent first):\n");
    printf("=============================================\n");
    
    LogStack temp;
    init_stack(&temp);
    
    LogEntry entry;
    while (pop_from_stack(stack, &entry)) {
        printf("Time: %s | Level: %s | Message: %s\n", 
               entry.timestamp, entry.level, entry.message);
        if (!push_to_stack(&temp, &entry)) break;
    }
    
    while (pop_from_stack(&temp, &entry)) {
        push_to_stack(stack, &entry);
    }
}

int main() {
    LogHeap heap;
    LogStack stack;
    
    init_heap(&heap, 50);
    init_stack(&stack);
    
    add_to_heap(&heap, "2024-01-15 10:30:15", "INFO", "System started successfully");
    add_to_heap(&heap, "2024-01-15 10:35:22", "WARN", "High memory usage detected");
    add_to_heap(&heap, "2024-01-15 10:40:05", "ERROR", "Database connection failed");
    add_to_heap(&heap, "2024-01-15 10:45:18", "INFO", "Backup completed");
    add_to_heap(&heap, "2024-01-15 10:50:33", "ERROR", "File system error");
    add_to_heap(&heap, "2024-01-15 10:55:47", "WARN", "CPU temperature high");
    add_to_heap(&heap, "2024-01-15 11:00:12", "INFO", "User login successful");
    
    analyze_logs(&heap, &stack);
    print_recent_errors(&stack);
    
    printf("\nTotal logs processed: %d\n", heap.count