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

int is_valid_timestamp(const char *timestamp) {
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
    char temp_line[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH - 1) return 0;
    strcpy(temp_line, line);
    
    char *timestamp = strtok(temp_line, " ");
    if (!timestamp || !is_valid_timestamp(timestamp)) return 0;
    strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    
    char *level = strtok(NULL, " ");
    if (!level) return 0;
    strncpy(entry->level, level, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    
    entry->severity = parse_severity(entry->level);
    if (entry->severity == -1) return 0;
    
    char *message = strtok(NULL, "\n");
    if (!message) return 0;
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

int init_log_collection(LogCollection *collection, size_t initial_capacity) {
    if (initial_capacity == 0 || initial_capacity > MAX_ENTRIES) return 0;
    collection->entries = malloc(initial_capacity * sizeof(LogEntry));
    if (!collection->entries) return 0;
    collection->count = 0;
    collection->capacity = initial_capacity;
    return 1;
}

void free_log_collection(LogCollection *collection) {
    if (collection->entries) {
        free(collection->entries);
        collection->entries = NULL;
    }
    collection->count = 0;
    collection->capacity = 0;
}

int add_log_entry(LogCollection *collection, const LogEntry *entry) {
    if (collection->count >= collection->capacity) {
        size_t new_capacity = collection->capacity * 2;
        if (new_capacity > MAX_ENTRIES) new_capacity = MAX_ENTRIES;
        if (new_capacity <= collection->capacity) return 0;
        
        LogEntry *new_entries = realloc(collection->entries, new_capacity * sizeof(LogEntry));
        if (!new_entries) return 0;
        collection->entries = new_entries;
        collection->capacity = new_capacity;
    }
    
    if (collection->count < collection->capacity) {
        collection->entries[collection->count] = *entry;
        collection->count++;
        return 1;
    }
    return 0;
}

void analyze_logs(const LogCollection *collection) {
    int severity_count[5] = {0};
    char most_recent_timestamp[32] = "";
    
    for (size_t i = 0; i < collection->count; i++) {
        const LogEntry *entry = &collection->entries[i];
        if (entry->severity >= 0 && entry->severity < 5) {
            severity_count[entry->severity]++;
        }
        if (strcmp(entry->timestamp, most_recent_timestamp) > 0) {
            strncpy(most_recent_timestamp, entry->timestamp, sizeof(most_recent_timestamp) - 1);
            most_recent_timestamp[sizeof(most_recent_timestamp) - 1] = '\0';
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %zu\n", collection->count);
    printf("DEBUG: %d\n", severity_count[0]);
    printf("INFO: %d\n", severity_count[1]);