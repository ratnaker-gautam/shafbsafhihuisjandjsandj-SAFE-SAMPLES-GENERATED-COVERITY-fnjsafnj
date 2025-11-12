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
} LogEntry;

typedef struct {
    LogEntry *entries;
    size_t count;
    size_t capacity;
} LogCollection;

int parse_timestamp(const char *str, struct tm *tm) {
    return sscanf(str, "%d-%d-%d %d:%d:%d",
                  &tm->tm_year, &tm->tm_mon, &tm->tm_mday,
                  &tm->tm_hour, &tm->tm_min, &tm->tm_sec) == 6;
}

int is_valid_level(const char *level) {
    return strcmp(level, "INFO") == 0 ||
           strcmp(level, "WARN") == 0 ||
           strcmp(level, "ERROR") == 0 ||
           strcmp(level, "DEBUG") == 0;
}

int add_log_entry(LogCollection *collection, const char *timestamp,
                  const char *level, const char *message) {
    if (collection->count >= collection->capacity) {
        size_t new_cap = collection->capacity * 2;
        if (new_cap < 16) new_cap = 16;
        LogEntry *new_entries = realloc(collection->entries, new_cap * sizeof(LogEntry));
        if (!new_entries) return 0;
        collection->entries = new_entries;
        collection->capacity = new_cap;
    }

    LogEntry *entry = &collection->entries[collection->count];
    strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    strncpy(entry->level, level, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    collection->count++;
    return 1;
}

void analyze_logs(LogCollection *collection) {
    int info_count = 0, warn_count = 0, error_count = 0, debug_count = 0;
    
    for (size_t i = 0; i < collection->count; i++) {
        const char *level = collection->entries[i].level;
        if (strcmp(level, "INFO") == 0) info_count++;
        else if (strcmp(level, "WARN") == 0) warn_count++;
        else if (strcmp(level, "ERROR") == 0) error_count++;
        else if (strcmp(level, "DEBUG") == 0) debug_count++;
    }
    
    printf("Log Analysis Results:\n");
    printf("INFO: %d\n", info_count);
    printf("WARN: %d\n", warn_count);
    printf("ERROR: %d\n", error_count);
    printf("DEBUG: %d\n", debug_count);
    printf("Total entries: %zu\n", collection->count);
}

int main(void) {
    char line[MAX_LINE_LEN];
    LogCollection collection = {0};
    collection.capacity = 16;
    collection.entries = malloc(collection.capacity * sizeof(LogEntry));
    if (!collection.entries) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL Message)\n");
    printf("Enter 'END' on a separate line to finish input\n");
    
    while (fgets(line, sizeof(line), stdin)) {
        if (strncmp(line, "END", 3) == 0 && (line[3] == '\n' || line[3] == '\0')) {
            break;
        }
        
        char timestamp[32], level[16], message[256];
        struct tm tm = {0};
        
        int parsed = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
        if (parsed != 3) {
            fprintf(stderr, "Invalid log format. Skipping.\n");
            continue;
        }
        
        if (!parse_timestamp(timestamp, &tm)) {
            fprintf(stderr, "Invalid timestamp format. Skipping.\n");
            continue;
        }
        
        if (!is_valid_level(level)) {
            fprintf(stderr, "Invalid log level. Skipping.\n");
            continue;
        }
        
        if (!add_log_entry(&collection, timestamp, level, message)) {
            fprintf(stderr, "Failed to add log entry. Skipping.\n");
            continue;
        }
        
        if (collection.count >= MAX_ENTRIES) {
            printf("Maximum entries reached. Stopping input.\n");
            break;
        }
    }
    
    if (collection.count > 0) {
        analyze_logs(&collection);
    } else {
        printf("No valid log entries to analyze.\n");
    }
    
    free(collection.entries);
    return 0;
}