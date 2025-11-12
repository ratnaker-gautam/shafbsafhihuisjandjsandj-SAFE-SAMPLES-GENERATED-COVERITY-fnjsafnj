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
    heap->entries = NULL;
    heap->count = 0;
    heap->capacity = 0;
}

int add_log_entry(LogHeap* heap, const char* timestamp, const char* level, const char* message) {
    if (heap->count >= heap->capacity) {
        return 0;
    }
    
    LogEntry* entry = &heap->entries[heap->count];
    
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

void analyze_logs(LogHeap* heap) {
    int error_count = 0;
    int warn_count = 0;
    int info_count = 0;
    int other_count = 0;
    
    for (int i = 0; i < heap->count; i++) {
        LogEntry* entry = &heap->entries[i];
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
    
    if (heap->count > 0) {
        printf("\nSample entries:\n");
        int samples = heap->count < 3 ? heap->count : 3;
        for (int i = 0; i < samples; i++) {
            LogEntry* entry = &heap->entries[i];
            printf("[%s] %s: %s\n", entry->timestamp, entry->level, entry->message);
        }
    }
}

int main() {
    LogHeap heap;
    init_log_heap(&heap, MAX_ENTRIES);
    
    LogEntry sample_logs[] = {
        {"2024-01-15 10:30:15", "INFO", "System started successfully", 1},
        {"2024-01-15 10:35:22", "WARN", "High memory usage detected", 2},
        {"2024-01-15 10:40:05", "ERROR", "Database connection failed", 3},
        {"2024-01-15 10:45:18", "INFO", "Backup completed", 1},
        {"2024-01-15 10:50:33", "WARN", "CPU temperature rising", 2}
    };
    
    int sample_count = sizeof(sample_logs) / sizeof(sample_logs[0]);
    
    for (int i = 0; i < sample_count; i++) {
        if (!add_log_entry(&heap, sample_logs[i].timestamp, sample_logs[i].level, sample_logs[i].message)) {
            fprintf(stderr, "Failed to add log entry\n");
            break;
        }
    }
    
    analyze_logs(&heap);
    
    free_log_heap(&heap);
    return 0;
}