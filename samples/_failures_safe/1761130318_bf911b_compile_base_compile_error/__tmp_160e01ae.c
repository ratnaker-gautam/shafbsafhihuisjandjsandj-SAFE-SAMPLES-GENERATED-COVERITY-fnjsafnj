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
    if (strcmp(level, "DEBUG") == 0) return 1;
    if (strcmp(level, "INFO") == 0) return 2;
    if (strcmp(level, "WARNING") == 0) return 3;
    if (strcmp(level, "ERROR") == 0) return 4;
    if (strcmp(level, "CRITICAL") == 0) return 5;
    return 0;
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

int add_log_entry(LogCollection* collection, const char* timestamp, const char* level, const char* message) {
    if (!collection || !timestamp || !level || !message) return 0;
    if (collection->count >= collection->capacity) return 0;
    if (!validate_timestamp(timestamp)) return 0;
    if (strlen(level) >= 16 || strlen(message) >= 256) return 0;
    
    LogEntry* entry = &collection->entries[collection->count];
    strncpy(entry->timestamp, timestamp, 31);
    entry->timestamp[31] = '\0';
    strncpy(entry->level, level, 15);
    entry->level[15] = '\0';
    strncpy(entry->message, message, 255);
    entry->message[255] = '\0';
    entry->severity = parse_severity(level);
    
    if (entry->severity == 0) return 0;
    
    collection->count++;
    return 1;
}

void analyze_logs(LogCollection* collection) {
    if (!collection || collection->count == 0) return;
    
    int severity_counts[6] = {0};
    int total_severity = 0;
    
    for (int i = 0; i < collection->count; i++) {
        LogEntry* entry = &collection->entries[i];
        if (entry->severity >= 1 && entry->severity <= 5) {
            severity_counts[entry->severity]++;
            total_severity += entry->severity;
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", collection->count);
    printf("DEBUG: %d\n", severity_counts[1]);
    printf("INFO: %d\n", severity_counts[2]);
    printf("WARNING: %d\n", severity_counts[3]);
    printf("ERROR: %d\n", severity_counts[4]);
    printf("CRITICAL: %d\n", severity_counts[5]);
    
    if (collection->count > 0) {
        double avg_severity = (double)total_severity / collection->count;
        printf("Average severity: %.2f\n", avg_severity);
    }
}

void free_log_collection(LogCollection* collection) {
    if (collection) {
        if (collection->entries) {
            free(collection->entries);
        }
        free(collection);
    }
}

int main() {
    LogCollection* logs = create_log_collection(MAX_ENTRIES);
    if (!logs) {
        printf("Failed to create log collection\n");
        return 1;
    }
    
    const char* sample_timestamps[] = {
        "2024-01-15 10:30:00",
        "2024-01-15 10:31:15",
        "2024-01-15 10:32:30",
        "2024-01-15 10:33:45",
        "2024-01-15 10:34:20",
        "2024-01-15 10:35:10"
    };
    
    const char* sample_levels[] = {
        "INFO",
        "DEBUG",
        "WARNING",
        "ERROR",
        "INFO",
        "CRITICAL"
    };
    
    const char* sample