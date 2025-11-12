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

void init_log_heap(LogHeap *heap, int capacity) {
    heap->entries = malloc(capacity * sizeof(LogEntry));
    if (heap->entries == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    heap->count = 0;
    heap->capacity = capacity;
}

void free_log_heap(LogHeap *heap) {
    free(heap->entries);
    heap->entries = NULL;
    heap->count = 0;
    heap->capacity = 0;
}

int add_log_entry(LogHeap *heap, const char *timestamp, const char *level, const char *message) {
    if (heap->count >= heap->capacity) {
        return 0;
    }
    
    LogEntry *entry = &heap->entries[heap->count];
    
    strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    
    strncpy(entry->level, level, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    if (strcmp(level, "ERROR") == 0) {
        entry->severity = 3;
    } else if (strcmp(level, "WARN") == 0) {
        entry->severity = 2;
    } else if (strcmp(level, "INFO") == 0) {
        entry->severity = 1;
    } else {
        entry->severity = 0;
    }
    
    heap->count++;
    return 1;
}

void analyze_logs(LogHeap *heap) {
    int error_count = 0;
    int warn_count = 0;
    int info_count = 0;
    int other_count = 0;
    
    for (int i = 0; i < heap->count; i++) {
        LogEntry *entry = &heap->entries[i];
        if (entry->severity == 3) {
            error_count++;
        } else if (entry->severity == 2) {
            warn_count++;
        } else if (entry->severity == 1) {
            info_count++;
        } else {
            other_count++;
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", heap->count);
    printf("ERROR entries: %d\n", error_count);
    printf("WARN entries: %d\n", warn_count);
    printf("INFO entries: %d\n", info_count);
    printf("OTHER entries: %d\n", other_count);
}

void display_recent_logs(LogHeap *heap, int count) {
    if (count <= 0 || count > heap->count) {
        count = heap->count;
    }
    
    printf("Recent %d log entries:\n", count);
    printf("Timestamp | Level | Message\n");
    printf("----------------------------------------\n");
    
    int start = heap->count - count;
    if (start < 0) start = 0;
    
    for (int i = start; i < heap->count; i++) {
        LogEntry *entry = &heap->entries[i];
        printf("%s | %s | %s\n", entry->timestamp, entry->level, entry->message);
    }
}

int main() {
    LogHeap heap;
    init_log_heap(&heap, MAX_ENTRIES);
    
    char timestamp[32];
    char level[16];
    char message[256];
    int entry_count = 0;
    
    printf("Log Analyzer - Enter log entries (timestamp level message)\n");
    printf("Type 'analyze' to show analysis or 'quit' to exit\n");
    printf("Example: 2024-01-15T10:30:00 INFO System started\n");
    
    while (entry_count < MAX_ENTRIES) {
        printf("Log entry %d: ", entry_count + 1);
        
        if (fgets(timestamp, sizeof(timestamp), stdin) == NULL) {
            break;
        }
        
        timestamp[strcspn(timestamp, "\n")] = '\0';
        
        if (strcmp(timestamp, "quit") == 0) {
            break;
        }
        
        if (strcmp(timestamp, "analyze") == 0) {
            analyze_logs(&heap);
            display_recent_logs(&heap, 5);
            continue;
        }
        
        printf("Level: ");
        if (fgets(level, sizeof(level), stdin) == NULL) {
            break;
        }
        level[strcspn(level, "\n")] = '\0';
        
        printf("Message: ");
        if (fgets(message, sizeof(message), stdin) == NULL) {
            break;
        }
        message[strcspn(message, "\n")] = '\0';