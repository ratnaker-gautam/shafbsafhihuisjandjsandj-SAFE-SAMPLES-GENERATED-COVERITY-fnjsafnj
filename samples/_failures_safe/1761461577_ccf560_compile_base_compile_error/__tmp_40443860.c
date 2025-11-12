//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

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
} LogCollection;

int parse_log_line(const char *line, LogEntry *entry) {
    if (!line || !entry) return 0;
    
    char temp_line[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH) return 0;
    strcpy(temp_line, line);
    
    char *timestamp = strtok(temp_line, " ");
    char *level = strtok(NULL, " ");
    char *message = strtok(NULL, "\n");
    
    if (!timestamp || !level || !message) return 0;
    
    if (strlen(timestamp) >= sizeof(entry->timestamp) ||
        strlen(level) >= sizeof(entry->level) ||
        strlen(message) >= sizeof(entry->message)) {
        return 0;
    }
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    
    return 1;
}

int init_log_collection(LogCollection *collection, size_t initial_capacity) {
    if (!collection || initial_capacity == 0) return 0;
    
    collection->entries = malloc(initial_capacity * sizeof(LogEntry));
    if (!collection->entries) return 0;
    
    collection->count = 0;
    collection->capacity = initial_capacity;
    return 1;
}

int add_log_entry(LogCollection *collection, const LogEntry *entry) {
    if (!collection || !entry) return 0;
    
    if (collection->count >= collection->capacity) {
        size_t new_capacity = collection->capacity * 2;
        if (new_capacity > MAX_ENTRIES) new_capacity = MAX_ENTRIES;
        
        LogEntry *new_entries = realloc(collection->entries, new_capacity * sizeof(LogEntry));
        if (!new_entries) return 0;
        
        collection->entries = new_entries;
        collection->capacity = new_capacity;
    }
    
    if (collection->count >= collection->capacity) return 0;
    
    collection->entries[collection->count] = *entry;
    collection->count++;
    return 1;
}

void count_log_levels(const LogCollection *collection, int *error_count, int *warning_count, int *info_count) {
    if (!collection || !error_count || !warning_count || !info_count) return;
    
    *error_count = 0;
    *warning_count = 0;
    *info_count = 0;
    
    for (size_t i = 0; i < collection->count; i++) {
        if (strcmp(collection->entries[i].level, "ERROR") == 0) {
            (*error_count)++;
        } else if (strcmp(collection->entries[i].level, "WARNING") == 0) {
            (*warning_count)++;
        } else if (strcmp(collection->entries[i].level, "INFO") == 0) {
            (*info_count)++;
        }
    }
}

void cleanup_log_collection(LogCollection *collection) {
    if (collection && collection->entries) {
        free(collection->entries);
        collection->entries = NULL;
        collection->count = 0;
        collection->capacity = 0;
    }
}

int main() {
    LogCollection logs;
    if (!init_log_collection(&logs, 10)) {
        fprintf(stderr, "Failed to initialize log collection\n");
        return 1;
    }
    
    char sample_logs[][MAX_LINE_LENGTH] = {
        "2023-10-01T10:30:00 INFO System started successfully",
        "2023-10-01T10:31:15 WARNING High memory usage detected",
        "2023-10-01T10:32:00 INFO User login: john_doe",
        "2023-10-01T10:33:45 ERROR Database connection failed",
        "2023-10-01T10:34:10 INFO Backup process started",
        "2023-10-01T10:35:20 WARNING Disk space below threshold",
        "2023-10-01T10:36:05 INFO User logout: john_doe",
        "2023-10-01T10:37:30 ERROR File not found: config.xml",
        "2023-10-01T10:38:00 INFO System shutdown initiated"
    };
    
    size_t num_sample_logs = sizeof(sample_logs) / sizeof(sample_logs[0]);
    
    for (size_t i = 0; i < num_sample_logs; i++) {
        LogEntry entry;
        if (parse_log_line(sample_logs[i], &entry)) {
            if (!add_log_entry(&logs, &entry)) {
                fprintf(stderr, "Failed to add log entry: %s\n", sample_logs[i]);
            }
        } else {
            fprintf(stderr, "Failed to parse log line: %s\n", sample_logs[i]);
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total log entries: %zu\n", logs.count);