//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
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

void init_heap(LogHeap *heap) {
    heap->capacity = 10;
    heap->count = 0;
    heap->entries = malloc(heap->capacity * sizeof(LogEntry));
    if (!heap->entries) exit(1);
}

void push_heap(LogHeap *heap, const LogEntry *entry) {
    if (heap->count >= heap->capacity) {
        int new_cap = heap->capacity * 2;
        if (new_cap > MAX_ENTRIES) new_cap = MAX_ENTRIES;
        LogEntry *new_entries = realloc(heap->entries, new_cap * sizeof(LogEntry));
        if (!new_entries) return;
        heap->entries = new_entries;
        heap->capacity = new_cap;
    }
    if (heap->count < heap->capacity) {
        heap->entries[heap->count] = *entry;
        heap->count++;
    }
}

void init_stack(LogStack *stack) {
    stack->top = -1;
}

int push_stack(LogStack *stack, const LogEntry *entry) {
    if (stack->top >= MAX_ENTRIES - 1) return 0;
    stack->top++;
    stack->entries[stack->top] = *entry;
    return 1;
}

int pop_stack(LogStack *stack, LogEntry *entry) {
    if (stack->top < 0) return 0;
    *entry = stack->entries[stack->top];
    stack->top--;
    return 1;
}

int parse_severity(const char *level) {
    if (strcmp(level, "DEBUG") == 0) return 1;
    if (strcmp(level, "INFO") == 0) return 2;
    if (strcmp(level, "WARNING") == 0) return 3;
    if (strcmp(level, "ERROR") == 0) return 4;
    if (strcmp(level, "CRITICAL") == 0) return 5;
    return 0;
}

int parse_log_line(const char *line, LogEntry *entry) {
    if (!line || !entry) return 0;
    
    char timestamp[32] = {0};
    char level[16] = {0};
    char message[256] = {0};
    
    int result = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
    if (result != 3) return 0;
    
    if (strlen(timestamp) > 31 || strlen(level) > 15 || strlen(message) > 255) return 0;
    
    strncpy(entry->timestamp, timestamp, 31);
    strncpy(entry->level, level, 15);
    strncpy(entry->message, message, 255);
    entry->severity = parse_severity(level);
    
    return entry->severity > 0;
}

void analyze_logs(LogHeap *heap, LogStack *stack) {
    int error_count = 0;
    int warning_count = 0;
    LogEntry temp;
    
    printf("Recent high severity logs (stack):\n");
    while (pop_stack(stack, &temp)) {
        if (temp.severity >= 4) {
            printf("%s %s: %s\n", temp.timestamp, temp.level, temp.message);
            error_count++;
        } else if (temp.severity == 3) {
            warning_count++;
        }
    }
    
    printf("\nAll logs by severity (heap):\n");
    for (int i = 0; i < heap->count; i++) {
        const LogEntry *entry = &heap->entries[i];
        printf("%s %s: %s\n", entry->timestamp, entry->level, entry->message);
    }
    
    printf("\nSummary:\n");
    printf("Total logs: %d\n", heap->count);
    printf("Errors: %d\n", error_count);
    printf("Warnings: %d\n", warning_count);
}

int main() {
    LogHeap heap;
    LogStack stack;
    char line[MAX_LINE_LENGTH];
    
    init_heap(&heap);
    init_stack(&stack);
    
    printf("Enter log lines (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Example: 2024-01-15T10:30:00 INFO System started\n");
    printf("Enter 'quit' to finish.\n");
    
    while (fgets(line, sizeof(line), stdin)) {
        if (strncmp(line, "quit", 4) == 0) break;
        
        line[strcspn(line, "\n")] = 0;
        
        if (strlen(line) == 0) continue;
        
        LogEntry entry;
        if (parse_log_line(line, &entry)) {
            push_heap(&heap, &entry);
            if (!push_stack(&stack, &entry)) {
                printf("Stack full, cannot