//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
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
            if (!isdigit(timestamp[i])) return 0;
        }
    }
    return 1;
}

int add_log_entry(LogCollection* collection, const char* line) {
    if (collection->count >= collection->capacity) {
        int new_capacity = collection->capacity * 2;
        if (new_capacity <= 0) new_capacity = 10;
        if (new_capacity > MAX_ENTRIES) new_capacity = MAX_ENTRIES;
        LogEntry* new_entries = realloc(collection->entries, new_capacity * sizeof(LogEntry));
        if (!new_entries) return 0;
        collection->entries = new_entries;
        collection->capacity = new_capacity;
    }

    LogEntry* entry = &collection->entries[collection->count];
    char level_buf[16];
    char message_buf[256];
    char timestamp_buf[32];
    
    int parsed = sscanf(line, "%31s %15s %255[^\n]", timestamp_buf, level_buf, message_buf);
    if (parsed != 3) return 0;
    
    if (!validate_timestamp(timestamp_buf)) return 0;
    
    int severity = parse_severity(level_buf);
    if (severity == -1) return 0;
    
    strncpy(entry->timestamp, timestamp_buf, sizeof(entry->timestamp) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    strncpy(entry->level, level_buf, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    strncpy(entry->message, message_buf, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    entry->severity = severity;
    
    collection->count++;
    return 1;
}

void print_statistics(const LogCollection* collection) {
    int severity_counts[5] = {0};
    int total_severity = 0;
    
    for (int i = 0; i < collection->count; i++) {
        const LogEntry* entry = &collection->entries[i];
        if (entry->severity >= 0 && entry->severity < 5) {
            severity_counts[entry->severity]++;
            total_severity += entry->severity;
        }
    }
    
    printf("Log Statistics:\n");
    printf("Total entries: %d\n", collection->count);
    printf("DEBUG: %d\n", severity_counts[0]);
    printf("INFO: %d\n", severity_counts[1]);
    printf("WARNING: %d\n", severity_counts[2]);
    printf("ERROR: %d\n", severity_counts[3]);
    printf("CRITICAL: %d\n", severity_counts[4]);
    
    if (collection->count > 0) {
        double avg_severity = (double)total_severity / collection->count;
        printf("Average severity: %.2f\n", avg_severity);
    }
}

void cleanup_collection(LogCollection* collection) {
    if (collection->entries) {
        free(collection->entries);
        collection->entries = NULL;
    }
    collection->count = 0;
    collection->capacity = 0;
}

int main(void) {
    LogCollection collection = {0};
    collection.capacity = 10;
    collection.entries = malloc(collection.capacity * sizeof(LogEntry));
    if (!collection.entries) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    char line[MAX_LINE_LEN];
    int line_count = 0;
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL MESSAGE):\n");
    printf("Enter empty line to finish.\n");
    
    while