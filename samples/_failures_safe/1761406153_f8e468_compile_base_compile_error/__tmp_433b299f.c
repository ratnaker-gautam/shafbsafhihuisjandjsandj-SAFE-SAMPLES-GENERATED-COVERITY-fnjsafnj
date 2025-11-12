//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
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

int parse_timestamp(const char *str, struct tm *tm) {
    return sscanf(str, "%d-%d-%d %d:%d:%d",
                  &tm->tm_year, &tm->tm_mon, &tm->tm_mday,
                  &tm->tm_hour, &tm->tm_min, &tm->tm_sec) == 6;
}

int severity_from_level(const char *level) {
    if (strcmp(level, "DEBUG") == 0) return 0;
    if (strcmp(level, "INFO") == 0) return 1;
    if (strcmp(level, "WARNING") == 0) return 2;
    if (strcmp(level, "ERROR") == 0) return 3;
    if (strcmp(level, "CRITICAL") == 0) return 4;
    return -1;
}

int parse_log_line(const char *line, LogEntry *entry) {
    char timestamp[32] = {0};
    char level[16] = {0};
    char message[256] = {0};
    
    if (sscanf(line, "%31[^|]|%15[^|]|%255[^\n]", timestamp, level, message) != 3) {
        return 0;
    }
    
    struct tm tm = {0};
    if (!parse_timestamp(timestamp, &tm)) {
        return 0;
    }
    
    int severity = severity_from_level(level);
    if (severity < 0) {
        return 0;
    }
    
    strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
    strncpy(entry->level, level, sizeof(entry->level) - 1);
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->severity = severity;
    
    return 1;
}

int init_log_collection(LogCollection *collection, size_t initial_capacity) {
    if (initial_capacity == 0 || initial_capacity > 100000) {
        return 0;
    }
    
    collection->entries = malloc(initial_capacity * sizeof(LogEntry));
    if (!collection->entries) {
        return 0;
    }
    
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
        if (new_capacity > 100000) {
            return 0;
        }
        
        LogEntry *new_entries = realloc(collection->entries, new_capacity * sizeof(LogEntry));
        if (!new_entries) {
            return 0;
        }
        
        collection->entries = new_entries;
        collection->capacity = new_capacity;
    }
    
    collection->entries[collection->count] = *entry;
    collection->count++;
    return 1;
}

void analyze_logs(const LogCollection *collection) {
    int severity_count[5] = {0};
    char *severity_names[] = {"DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL"};
    
    for (size_t i = 0; i < collection->count; i++) {
        const LogEntry *entry = &collection->entries[i];
        if (entry->severity >= 0 && entry->severity < 5) {
            severity_count[entry->severity]++;
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("====================\n");
    printf("Total entries: %zu\n", collection->count);
    printf("Severity breakdown:\n");
    for (int i = 0; i < 5; i++) {
        printf("  %-8s: %d\n", severity_names[i], severity_count[i]);
    }
}

int main(void) {
    LogCollection collection;
    if (!init_log_collection(&collection, 10)) {
        fprintf(stderr, "Failed to initialize log collection\n");
        return 1;
    }
    
    char input_buffer[MAX_LINE_LENGTH];
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS|LEVEL|MESSAGE)\n");
    printf("Enter 'END' on a separate line to finish input\n");
    
    while (fgets(input_buffer, sizeof(input_buffer), stdin)) {
        size_t len = strlen(input_buffer);
        if (len > 0 && input_buffer[len - 1] == '\n') {
            input_buffer[len - 1] = '\0';
        }
        
        if (strcmp(input_buffer, "END") == 0) {
            break;