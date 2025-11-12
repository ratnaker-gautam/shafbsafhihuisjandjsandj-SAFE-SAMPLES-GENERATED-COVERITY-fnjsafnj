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

int parse_severity(const char* level) {
    if (strcmp(level, "DEBUG") == 0) return 0;
    if (strcmp(level, "INFO") == 0) return 1;
    if (strcmp(level, "WARNING") == 0) return 2;
    if (strcmp(level, "ERROR") == 0) return 3;
    if (strcmp(level, "CRITICAL") == 0) return 4;
    return -1;
}

int add_log_entry(LogHeap* heap, const char* timestamp, const char* level, const char* message) {
    if (heap->count >= heap->capacity) {
        return 0;
    }
    
    LogEntry* entry = &heap->entries[heap->count];
    
    if (strlen(timestamp) >= sizeof(entry->timestamp) - 1) {
        return 0;
    }
    strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    
    if (strlen(level) >= sizeof(entry->level) - 1) {
        return 0;
    }
    strncpy(entry->level, level, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    
    if (strlen(message) >= sizeof(entry->message) - 1) {
        return 0;
    }
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    entry->severity = parse_severity(level);
    if (entry->severity == -1) {
        return 0;
    }
    
    heap->count++;
    return 1;
}

void analyze_logs(LogHeap* heap) {
    int severity_count[5] = {0};
    char severity_names[5][16] = {"DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL"};
    
    for (int i = 0; i < heap->count; i++) {
        LogEntry* entry = &heap->entries[i];
        if (entry->severity >= 0 && entry->severity < 5) {
            severity_count[entry->severity]++;
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("====================\n");
    printf("Total entries: %d\n", heap->count);
    printf("Severity breakdown:\n");
    for (int i = 0; i < 5; i++) {
        printf("  %s: %d\n", severity_names[i], severity_count[i]);
    }
}

int main() {
    LogHeap heap;
    init_log_heap(&heap, MAX_ENTRIES);
    
    LogEntry sample_logs[] = {
        {"2024-01-15 10:30:00", "INFO", "System started successfully", 0},
        {"2024-01-15 10:31:15", "DEBUG", "Initializing modules", 0},
        {"2024-01-15 10:32:30", "WARNING", "High memory usage detected", 0},
        {"2024-01-15 10:33:45", "ERROR", "Database connection failed", 0},
        {"2024-01-15 10:35:00", "INFO", "Retrying connection", 0},
        {"2024-01-15 10:36:15", "CRITICAL", "System shutdown required", 0}
    };
    
    int sample_count = sizeof(sample_logs) / sizeof(sample_logs[0]);
    
    for (int i = 0; i < sample_count; i++) {
        if (!add_log_entry(&heap, sample_logs[i].timestamp, sample_logs[i].level, sample_logs[i].message)) {
            fprintf(stderr, "Failed to add log entry\n");
            break;
        }
    }
    
    analyze_logs(&heap);
    
    printf("\nRecent log entries:\n");
    printf("==================\n");
    int display_count = heap.count > 5 ? 5 : heap.count;
    for (int i = 0; i < display_count; i++) {
        LogEntry* entry = &heap.entries[i];
        printf("[%s] %s: %s\n", entry->timestamp, entry->level,