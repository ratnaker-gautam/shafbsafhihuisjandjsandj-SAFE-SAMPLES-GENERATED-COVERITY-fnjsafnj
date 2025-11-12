//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LINE_LEN 256
#define MAX_ENTRIES 100

typedef struct {
    char timestamp[20];
    char level[10];
    char message[MAX_LINE_LEN - 30];
} LogEntry;

typedef struct {
    LogEntry* entries;
    int count;
    int capacity;
} LogHeap;

void init_log_heap(LogHeap* heap, int capacity) {
    heap->entries = malloc(capacity * sizeof(LogEntry));
    if (heap->entries == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    heap->count = 0;
    heap->capacity = capacity;
}

void free_log_heap(LogHeap* heap) {
    free(heap->entries);
    heap->count = 0;
    heap->capacity = 0;
}

int add_log_entry(LogHeap* heap, const char* timestamp, const char* level, const char* message) {
    if (heap->count >= heap->capacity) {
        return 0;
    }
    if (timestamp == NULL || level == NULL || message == NULL) {
        return 0;
    }
    if (strlen(timestamp) >= 19 || strlen(level) >= 9 || strlen(message) >= (MAX_LINE_LEN - 31)) {
        return 0;
    }
    
    LogEntry* entry = &heap->entries[heap->count];
    strncpy(entry->timestamp, timestamp, 19);
    entry->timestamp[19] = '\0';
    strncpy(entry->level, level, 9);
    entry->level[9] = '\0';
    strncpy(entry->message, message, MAX_LINE_LEN - 31);
    entry->message[MAX_LINE_LEN - 31] = '\0';
    
    heap->count++;
    return 1;
}

void analyze_logs_stack(LogHeap* heap) {
    if (heap == NULL || heap->count == 0) {
        printf("No logs to analyze\n");
        return;
    }
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    for (int i = 0; i < heap->count; i++) {
        const LogEntry* entry = &heap->entries[i];
        if (strcmp(entry->level, "ERROR") == 0) {
            error_count++;
        } else if (strcmp(entry->level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(entry->level, "INFO") == 0) {
            info_count++;
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", heap->count);
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
}

int main() {
    LogHeap heap;
    init_log_heap(&heap, MAX_ENTRIES);
    
    printf("Enter log entries (timestamp level message). Type 'done' to finish.\n");
    printf("Format: YYYY-MM-DD HH:MM:SS LEVEL Message text\n");
    
    char input[MAX_LINE_LEN];
    while (fgets(input, sizeof(input), stdin) != NULL) {
        if (strncmp(input, "done", 4) == 0) {
            break;
        }
        
        char timestamp[20] = {0};
        char level[10] = {0};
        char message[MAX_LINE_LEN - 30] = {0};
        
        if (sscanf(input, "%19s %9s %[^\n]", timestamp, level, message) == 3) {
            if (!add_log_entry(&heap, timestamp, level, message)) {
                printf("Failed to add log entry (limit reached or invalid data)\n");
            }
        } else {
            printf("Invalid format. Use: YYYY-MM-DD HH:MM:SS LEVEL Message\n");
        }
        
        if (heap.count >= MAX_ENTRIES) {
            printf("Maximum log entries reached\n");
            break;
        }
    }
    
    analyze_logs_stack(&heap);
    free_log_heap(&heap);
    
    return 0;
}