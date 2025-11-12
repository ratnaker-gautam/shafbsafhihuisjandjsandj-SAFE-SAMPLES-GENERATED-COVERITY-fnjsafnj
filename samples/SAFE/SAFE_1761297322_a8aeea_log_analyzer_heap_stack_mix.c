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
    LogEntry* entries;
    int count;
    int capacity;
} LogHeap;

void init_log_heap(LogHeap* heap, int capacity) {
    if (capacity <= 0 || capacity > 10000) capacity = 100;
    heap->entries = malloc(capacity * sizeof(LogEntry));
    if (!heap->entries) exit(1);
    heap->count = 0;
    heap->capacity = capacity;
}

void free_log_heap(LogHeap* heap) {
    if (heap->entries) free(heap->entries);
    heap->entries = NULL;
    heap->count = 0;
    heap->capacity = 0;
}

int add_log_entry(LogHeap* heap, const char* timestamp, const char* level, const char* message) {
    if (!heap || !timestamp || !level || !message) return 0;
    if (heap->count >= heap->capacity) return 0;
    
    if (strlen(timestamp) >= 31) return 0;
    if (strlen(level) >= 15) return 0;
    if (strlen(message) >= 255) return 0;
    
    LogEntry* entry = &heap->entries[heap->count];
    strncpy(entry->timestamp, timestamp, 31);
    entry->timestamp[31] = '\0';
    strncpy(entry->level, level, 15);
    entry->level[15] = '\0';
    strncpy(entry->message, message, 255);
    entry->message[255] = '\0';
    
    heap->count++;
    return 1;
}

void analyze_logs(LogHeap* heap) {
    if (!heap || heap->count == 0) return;
    
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
    init_log_heap(&heap, 50);
    
    LogEntry sample_logs[] = {
        {"2024-01-15 10:30:15", "INFO", "System started successfully"},
        {"2024-01-15 10:35:22", "WARNING", "High memory usage detected"},
        {"2024-01-15 10:40:05", "ERROR", "Database connection failed"},
        {"2024-01-15 10:45:18", "INFO", "Backup completed"},
        {"2024-01-15 10:50:33", "ERROR", "File not found"},
        {"2024-01-15 10:55:47", "WARNING", "CPU temperature high"},
        {"2024-01-15 11:00:12", "INFO", "User login successful"}
    };
    
    int sample_count = sizeof(sample_logs) / sizeof(sample_logs[0]);
    
    for (int i = 0; i < sample_count; i++) {
        if (!add_log_entry(&heap, sample_logs[i].timestamp, sample_logs[i].level, sample_logs[i].message)) {
            printf("Failed to add log entry %d\n", i);
        }
    }
    
    analyze_logs(&heap);
    
    char search_level[16];
    printf("\nEnter log level to search (INFO/WARNING/ERROR): ");
    if (fgets(search_level, sizeof(search_level), stdin)) {
        search_level[strcspn(search_level, "\n")] = '\0';
        
        printf("\nMatching entries for '%s':\n", search_level);
        int found = 0;
        for (int i = 0; i < heap.count; i++) {
            if (strcmp(heap.entries[i].level, search_level) == 0) {
                printf("%s [%s] %s\n", heap.entries[i].timestamp, heap.entries[i].level, heap.entries[i].message);
                found = 1;
            }
        }
        if (!found) {
            printf("No entries found for level '%s'\n", search_level);
        }
    }
    
    free_log_heap(&heap);
    return 0;
}