//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <ctype.h>

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
    size_t count;
    size_t capacity;
} LogCollection;

int parse_severity(const char *level) {
    if (strcmp(level, "DEBUG") == 0) return 0;
    if (strcmp(level, "INFO") == 0) return 1;
    if (strcmp(level, "WARNING") == 0) return 2;
    if (strcmp(level, "ERROR") == 0) return 3;
    if (strcmp(level, "CRITICAL") == 0) return 4;
    return -1;
}

int validate_timestamp(const char *timestamp) {
    if (strlen(timestamp) != 19) return 0;
    for (int i = 0; i < 19; i++) {
        if (i == 4 || i == 7) {
            if (timestamp[i] != '-') return 0;
        } else if (i == 10) {
            if (timestamp[i] != ' ') return 0;
        } else if (i == 13 || i == 16) {
            if (timestamp[i] != ':') return 0;
        } else {
            if (!isdigit(timestamp[i])) return 0;
        }
    }
    return 1;
}

int parse_log_line(const char *line, LogEntry *entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char timestamp[32], level[16], message[256];
    int parsed = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
    
    if (parsed != 3) return 0;
    if (!validate_timestamp(timestamp)) return 0;
    
    int severity = parse_severity(level);
    if (severity == -1) return 0;
    
    strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    strncpy(entry->level, level, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    entry->severity = severity;
    
    return 1;
}

int init_log_collection(LogCollection *collection, size_t initial_capacity) {
    if (collection == NULL || initial_capacity == 0) return 0;
    
    collection->entries = malloc(initial_capacity * sizeof(LogEntry));
    if (collection->entries == NULL) return 0;
    
    collection->count = 0;
    collection->capacity = initial_capacity;
    return 1;
}

int add_log_entry(LogCollection *collection, const LogEntry *entry) {
    if (collection == NULL || entry == NULL) return 0;
    
    if (collection->count >= collection->capacity) {
        size_t new_capacity = collection->capacity * 2;
        if (new_capacity <= collection->capacity) return 0;
        
        LogEntry *new_entries = realloc(collection->entries, new_capacity * sizeof(LogEntry));
        if (new_entries == NULL) return 0;
        
        collection->entries = new_entries;
        collection->capacity = new_capacity;
    }
    
    collection->entries[collection->count] = *entry;
    collection->count++;
    return 1;
}

void free_log_collection(LogCollection *collection) {
    if (collection != NULL) {
        free(collection->entries);
        collection->entries = NULL;
        collection->count = 0;
        collection->capacity = 0;
    }
}

void analyze_logs(const LogCollection *collection) {
    if (collection == NULL || collection->count == 0) {
        printf("No logs to analyze.\n");
        return;
    }
    
    int severity_count[5] = {0};
    char latest_timestamp[32] = "";
    
    for (size_t i = 0; i < collection->count; i++) {
        const LogEntry *entry = &collection->entries[i];
        if (entry->severity >= 0 && entry->severity < 5) {
            severity_count[entry->severity]++;
        }
        if (strcmp(entry->timestamp, latest_timestamp) > 0) {
            strncpy(latest_timestamp, entry->timestamp, sizeof(latest_timestamp) - 1);
            latest_timestamp[sizeof(latest_timestamp) - 1] = '\0';
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %zu\n", collection->count);
    printf("DEBUG: %d\n", severity_count[0]);
    printf("INFO: %d\n", severity_count[1]);
    printf("WARNING: %d\n", severity_count[2]);
    printf("ERROR: %d\n", severity_count[3