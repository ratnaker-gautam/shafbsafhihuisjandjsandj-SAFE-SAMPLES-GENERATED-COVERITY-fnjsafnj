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
    LogEntry* entries;
    size_t count;
    size_t capacity;
} LogCollection;

int parse_severity(const char* level) {
    if (strcmp(level, "ERROR") == 0) return 3;
    if (strcmp(level, "WARN") == 0) return 2;
    if (strcmp(level, "INFO") == 0) return 1;
    if (strcmp(level, "DEBUG") == 0) return 0;
    return -1;
}

int is_valid_timestamp(const char* timestamp) {
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

int add_log_entry(LogCollection* collection, const char* timestamp, const char* level, const char* message) {
    if (collection->count >= collection->capacity) {
        size_t new_capacity = collection->capacity * 2;
        if (new_capacity == 0) new_capacity = 16;
        if (new_capacity > MAX_ENTRIES) new_capacity = MAX_ENTRIES;
        LogEntry* new_entries = realloc(collection->entries, new_capacity * sizeof(LogEntry));
        if (!new_entries) return 0;
        collection->entries = new_entries;
        collection->capacity = new_capacity;
    }

    LogEntry* entry = &collection->entries[collection->count];
    strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    strncpy(entry->level, level, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    entry->severity = parse_severity(level);
    
    if (entry->severity == -1) return 0;
    collection->count++;
    return 1;
}

void analyze_logs(const LogCollection* collection) {
    int severity_count[4] = {0};
    char most_common_level[16] = "NONE";
    int max_count = 0;
    
    for (size_t i = 0; i < collection->count; i++) {
        const LogEntry* entry = &collection->entries[i];
        if (entry->severity >= 0 && entry->severity < 4) {
            severity_count[entry->severity]++;
        }
    }
    
    const char* levels[] = {"DEBUG", "INFO", "WARN", "ERROR"};
    for (int i = 0; i < 4; i++) {
        if (severity_count[i] > max_count) {
            max_count = severity_count[i];
            strncpy(most_common_level, levels[i], sizeof(most_common_level) - 1);
            most_common_level[sizeof(most_common_level) - 1] = '\0';
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %zu\n", collection->count);
    printf("DEBUG: %d\n", severity_count[0]);
    printf("INFO: %d\n", severity_count[1]);
    printf("WARN: %d\n", severity_count[2]);
    printf("ERROR: %d\n", severity_count[3]);
    printf("Most common level: %s\n", most_common_level);
}

void cleanup_logs(LogCollection* collection) {
    free(collection->entries);
    collection->entries = NULL;
    collection->count = 0;
    collection->capacity = 0;
}

int main(void) {
    LogCollection collection = {0};
    char line[MAX_LINE_LENGTH];
    int line_count = 0;
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL Message)\n");
    printf("Enter 'END' on a separate line to finish input\n");
    
    while (fgets(line, sizeof(line), stdin)) {
        line_count++;
        if (line_count > MAX_ENTRIES) {
            printf("Maximum entries reached\n");
            break;
        }
        
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (strcmp(line, "END") == 0) {
            break;
        }