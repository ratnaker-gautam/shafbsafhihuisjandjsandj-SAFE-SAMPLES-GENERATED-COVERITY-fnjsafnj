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
    
    int severity = parse_severity(level);
    if (severity == -1) return 0;
    
    if (strlen(message) >= 256) return 0;
    
    LogEntry* entry = &collection->entries[collection->count];
    strncpy(entry->timestamp, timestamp, 31);
    entry->timestamp[31] = '\0';
    strncpy(entry->level, level, 15);
    entry->level[15] = '\0';
    strncpy(entry->message, message, 255);
    entry->message[255] = '\0';
    entry->severity = severity;
    
    collection->count++;
    return 1;
}

void analyze_logs(LogCollection* collection) {
    if (!collection || collection->count == 0) return;
    
    int severity_count[5] = {0};
    char most_recent[32] = "";
    
    for (int i = 0; i < collection->count; i++) {
        LogEntry* entry = &collection->entries[i];
        if (entry->severity >= 0 && entry->severity < 5) {
            severity_count[entry->severity]++;
        }
        if (strcmp(entry->timestamp, most_recent) > 0) {
            strncpy(most_recent, entry->timestamp, 31);
            most_recent[31] = '\0';
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", collection->count);
    printf("DEBUG: %d\n", severity_count[0]);
    printf("INFO: %d\n", severity_count[1]);
    printf("WARNING: %d\n", severity_count[2]);
    printf("ERROR: %d\n", severity_count[3]);
    printf("CRITICAL: %d\n", severity_count[4]);
    printf("Most recent: %s\n", most_recent);
}

int main() {
    LogCollection* logs = create_log_collection(MAX_ENTRIES);
    if (!logs) {
        printf("Failed to create log collection\n");
        return 1;
    }
    
    LogEntry sample_logs[] = {
        {"2024-01-15 10:30:25", "INFO", "System started successfully", 1},
        {"2024-01-15 10:31:10", "DEBUG", "Initializing modules", 0},
        {"2024-01-15 10:32:45", "WARNING", "High memory usage detected", 2},
        {"2024-01-15 10:33:20", "ERROR", "Failed to connect to database", 3},
        {"2024-01-15 10:34:05", "INFO", "Retrying connection", 1},
        {"2024-01-15 10:35:30", "CR