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
} LogEntry;

typedef struct {
    LogEntry *entries;
    size_t count;
    size_t capacity;
} LogHeap;

typedef struct {
    LogEntry entries[MAX_ENTRIES];
    size_t count;
} LogStack;

void heap_init(LogHeap *heap) {
    heap->entries = malloc(sizeof(LogEntry) * 100);
    if (heap->entries == NULL) {
        exit(EXIT_FAILURE);
    }
    heap->count = 0;
    heap->capacity = 100;
}

void heap_push(LogHeap *heap, const LogEntry *entry) {
    if (heap->count >= heap->capacity) {
        size_t new_capacity = heap->capacity * 2;
        LogEntry *new_entries = realloc(heap->entries, sizeof(LogEntry) * new_capacity);
        if (new_entries == NULL) {
            exit(EXIT_FAILURE);
        }
        heap->entries = new_entries;
        heap->capacity = new_capacity;
    }
    if (heap->count < heap->capacity) {
        heap->entries[heap->count] = *entry;
        heap->count++;
    }
}

void stack_push(LogStack *stack, const LogEntry *entry) {
    if (stack->count < MAX_ENTRIES) {
        stack->entries[stack->count] = *entry;
        stack->count++;
    }
}

int parse_log_line(const char *line, LogEntry *entry) {
    if (line == NULL || entry == NULL) {
        return 0;
    }
    char level[16];
    char timestamp[32];
    char message[256];
    int result = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
    if (result != 3) {
        return 0;
    }
    if (strlen(timestamp) > 31 || strlen(level) > 15 || strlen(message) > 255) {
        return 0;
    }
    strncpy(entry->timestamp, timestamp, 31);
    entry->timestamp[31] = '\0';
    strncpy(entry->level, level, 15);
    entry->level[15] = '\0';
    strncpy(entry->message, message, 255);
    entry->message[255] = '\0';
    return 1;
}

void analyze_logs(LogHeap *heap, LogStack *stack) {
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    for (size_t i = 0; i < heap->count; i++) {
        if (strcmp(heap->entries[i].level, "ERROR") == 0) {
            error_count++;
        } else if (strcmp(heap->entries[i].level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(heap->entries[i].level, "INFO") == 0) {
            info_count++;
        }
    }
    for (size_t i = 0; i < stack->count; i++) {
        if (strcmp(stack->entries[i].level, "ERROR") == 0) {
            error_count++;
        } else if (strcmp(stack->entries[i].level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(stack->entries[i].level, "INFO") == 0) {
            info_count++;
        }
    }
    printf("Log Analysis Results:\n");
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    printf("Total entries: %zu\n", heap->count + stack->count);
}

int main(void) {
    LogHeap heap_logs;
    LogStack stack_logs;
    heap_init(&heap_logs);
    stack_logs.count = 0;
    char line[MAX_LINE_LENGTH];
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Enter 'END' on a separate line to finish input.\n");
    while (fgets(line, sizeof(line), stdin) != NULL) {
        if (strncmp(line, "END", 3) == 0 && (line[3] == '\n' || line[3] == '\0')) {
            break;
        }
        LogEntry entry;
        if (parse_log_line(line, &entry)) {
            if (heap_logs.count < 50) {
                heap_push(&heap_logs, &entry);
            } else {
                stack_push(&stack_logs, &entry);
            }
        } else {
            printf("Invalid log format. Skipping.\n");
        }
    }
    analyze_logs(&heap_logs, &stack_logs);
    free(heap_logs.entries);
    return 0;
}