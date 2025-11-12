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
} LogEntry;

typedef struct {
    LogEntry *entries;
    int count;
    int capacity;
} LogBuffer;

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

int add_log_entry(LogBuffer *buffer, const char *line) {
    if (buffer->count >= buffer->capacity) {
        int new_capacity = buffer->capacity * 2;
        if (new_capacity <= 0) new_capacity = 16;
        LogEntry *new_entries = realloc(buffer->entries, new_capacity * sizeof(LogEntry));
        if (!new_entries) return 0;
        buffer->entries = new_entries;
        buffer->capacity = new_capacity;
    }

    LogEntry *entry = &buffer->entries[buffer->count];
    struct tm tm = {0};
    
    if (sscanf(line, "%31s %15s %255[^\n]", 
               entry->timestamp, entry->level, entry->message) != 3) {
        return 0;
    }

    if (!parse_timestamp(entry->timestamp, &tm)) {
        return 0;
    }

    if (!is_valid_level(entry->level)) {
        return 0;
    }

    buffer->count++;
    return 1;
}

void analyze_logs(LogBuffer *buffer) {
    int info_count = 0, warn_count = 0, error_count = 0, debug_count = 0;
    
    for (int i = 0; i < buffer->count; i++) {
        const char *level = buffer->entries[i].level;
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
    printf("Total entries: %d\n", buffer->count);
}

int main(void) {
    LogBuffer buffer_stack;
    LogBuffer *buffer = &buffer_stack;
    buffer->entries = NULL;
    buffer->count = 0;
    buffer->capacity = 0;

    char line[MAX_LINE_LENGTH];
    int line_count = 0;
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message):\n");
    printf("Enter empty line to finish.\n");

    while (fgets(line, sizeof(line), stdin)) {
        if (line[0] == '\n') break;
        
        line_count++;
        if (line_count > MAX_ENTRIES) {
            printf("Maximum entries reached.\n");
            break;
        }

        size_t len = strlen(line);
        if (len > 0 && line[len-1] == '\n') {
            line[len-1] = '\0';
        }

        if (len >= MAX_LINE_LENGTH - 1) {
            printf("Line too long, skipping.\n");
            continue;
        }

        if (!add_log_entry(buffer, line)) {
            printf("Invalid log format, skipping: %s\n", line);
        }
    }

    if (buffer->count > 0) {
        analyze_logs(buffer);
    } else {
        printf("No valid log entries to analyze.\n");
    }

    free(buffer->entries);
    return 0;
}