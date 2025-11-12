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
} LogCollection;

int parse_severity(const char* level) {
    if (strcmp(level, "DEBUG") == 0) return 0;
    if (strcmp(level, "INFO") == 0) return 1;
    if (strcmp(level, "WARNING") == 0) return 2;
    if (strcmp(level, "ERROR") == 0) return 3;
    if (strcmp(level, "CRITICAL") == 0) return 4;
    return -1;
}

int validate_timestamp(const char* timestamp) {
    if (strlen(timestamp) != 19) return 0;
    for (int i = 0; i < 19; i++) {
        if (i == 4 || i == 7) {
            if (timestamp[i] != '-') return 0;
        } else if (i == 10) {
            if (timestamp[i] != ' ') return 0;
        } else if (i == 13 || i == 16) {
            if (timestamp[i] != ':') return 0;
        } else {
            if (timestamp[i] < '0' || timestamp[i] > '9') return 0;
        }
    }
    return 1;
}

LogCollection* create_log_collection(int initial_capacity) {
    if (initial_capacity <= 0) return NULL;
    LogCollection* collection = malloc(sizeof(LogCollection));
    if (!collection) return NULL;
    collection->entries = malloc(sizeof(LogEntry) * initial_capacity);
    if (!collection->entries) {
        free(collection);
        return NULL;
    }
    collection->count = 0;
    collection->capacity = initial_capacity;
    return collection;
}

void destroy_log_collection(LogCollection* collection) {
    if (collection) {
        free(collection->entries);
        free(collection);
    }
}

int add_log_entry(LogCollection* collection, const char* timestamp, const char* level, const char* message) {
    if (!collection || !timestamp || !level || !message) return 0;
    if (collection->count >= collection->capacity) return 0;
    if (!validate_timestamp(timestamp)) return 0;
    if (parse_severity(level) < 0) return 0;
    if (strlen(message) >= 256) return 0;
    
    LogEntry* entry = &collection->entries[collection->count];
    strncpy(entry->timestamp, timestamp, 31);
    entry->timestamp[31] = '\0';
    strncpy(entry->level, level, 15);
    entry->level[15] = '\0';
    strncpy(entry->message, message, 255);
    entry->message[255] = '\0';
    entry->severity = parse_severity(level);
    collection->count++;
    return 1;
}

void analyze_logs(LogCollection* collection) {
    if (!collection || collection->count == 0) return;
    
    int severity_count[5] = {0};
    for (int i = 0; i < collection->count; i++) {
        if (collection->entries[i].severity >= 0 && collection->entries[i].severity < 5) {
            severity_count[collection->entries[i].severity]++;
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", collection->count);
    printf("DEBUG: %d\n", severity_count[0]);
    printf("INFO: %d\n", severity_count[1]);
    printf("WARNING: %d\n", severity_count[2]);
    printf("ERROR: %d\n", severity_count[3]);
    printf("CRITICAL: %d\n", severity_count[4]);
}

int main() {
    LogCollection* collection = create_log_collection(MAX_ENTRIES);
    if (!collection) {
        printf("Failed to create log collection\n");
        return 1;
    }
    
    LogEntry sample_logs[] = {
        {"2024-01-15 10:30:00", "INFO", "System started successfully", 0},
        {"2024-01-15 10:31:15", "DEBUG", "Initializing modules", 0},
        {"2024-01-15 10:32:30", "WARNING", "High memory usage detected", 0},
        {"2024-01-15 10:33:45", "ERROR", "Failed to connect to database", 0},
        {"2024-01-15 10:34:20", "INFO", "Retrying connection", 0},
        {"2024-01-15 10:35:10", "CRITICAL", "System shutdown required", 0}
    };
    
    int sample_count = sizeof(sample_logs) / sizeof(sample_logs[0]);
    for (int i = 0; i < sample_count; i++) {
        if (!add_log_entry(collection, sample_logs[i].timestamp, 
                          sample_logs[i].level, sample_logs[i