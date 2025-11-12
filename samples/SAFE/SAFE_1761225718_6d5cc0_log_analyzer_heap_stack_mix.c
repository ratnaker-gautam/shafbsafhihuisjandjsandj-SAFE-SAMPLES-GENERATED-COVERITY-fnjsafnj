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

void init_log_heap(LogHeap *heap, size_t capacity) {
    if (capacity == 0 || capacity > MAX_ENTRIES) capacity = MAX_ENTRIES;
    heap->entries = malloc(capacity * sizeof(LogEntry));
    if (heap->entries == NULL) exit(EXIT_FAILURE);
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
    if (heap->count >= heap->capacity) return 0;
    if (timestamp == NULL || level == NULL || message == NULL) return 0;
    
    size_t ts_len = strlen(timestamp);
    size_t lvl_len = strlen(level);
    size_t msg_len = strlen(message);
    
    if (ts_len >= 32 || lvl_len >= 16 || msg_len >= 256) return 0;
    
    LogEntry *entry = &heap->entries[heap->count];
    strncpy(entry->timestamp, timestamp, 31);
    entry->timestamp[31] = '\0';
    strncpy(entry->level, level, 15);
    entry->level[15] = '\0';
    strncpy(entry->message, message, 255);
    entry->message[255] = '\0';
    
    heap->count++;
    return 1;
}

void analyze_logs(LogHeap *heap) {
    if (heap->count == 0) {
        printf("No logs to analyze.\n");
        return;
    }
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    for (size_t i = 0; i < heap->count; i++) {
        const LogEntry *entry = &heap->entries[i];
        if (strcmp(entry->level, "ERROR") == 0) {
            error_count++;
        } else if (strcmp(entry->level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(entry->level, "INFO") == 0) {
            info_count++;
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %zu\n", heap->count);
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    
    if (error_count > 0) {
        printf("\nRecent ERROR entries:\n");
        int printed = 0;
        for (size_t i = heap->count - 1; i < heap->count && printed < 3; i--) {
            const LogEntry *entry = &heap->entries[i];
            if (strcmp(entry->level, "ERROR") == 0) {
                printf("- %s: %s\n", entry->timestamp, entry->message);
                printed++;
            }
        }
    }
}

int main(void) {
    LogHeap heap;
    init_log_heap(&heap, 50);
    
    LogEntry sample_logs[] = {
        {"2024-01-15 10:30:00", "INFO", "System started successfully"},
        {"2024-01-15 10:31:15", "WARNING", "High memory usage detected"},
        {"2024-01-15 10:32:30", "ERROR", "Database connection failed"},
        {"2024-01-15 10:33:45", "INFO", "User login successful"},
        {"2024-01-15 10:34:20", "ERROR", "File not found: config.xml"},
        {"2024-01-15 10:35:10", "WARNING", "CPU temperature above threshold"},
        {"2024-01-15 10:36:25", "INFO", "Backup completed"},
        {"2024-01-15 10:37:40", "ERROR", "Network timeout"},
        {"2024-01-15 10:38:55", "INFO", "Cache cleared"},
        {"2024-01-15 10:39:30", "WARNING", "Disk space low"}
    };
    
    size_t sample_count = sizeof(sample_logs) / sizeof(sample_logs[0]);
    for (size_t i = 0; i < sample_count; i++) {
        if (!add_log_entry(&heap, sample_logs[i].timestamp, sample_logs[i].level, sample_logs[i].message)) {
            printf("Failed to add log entry %zu\n", i);
            break;
        }
    }
    
    analyze_logs(&heap);
    
    free_log_heap(&heap);
    return 0;
}