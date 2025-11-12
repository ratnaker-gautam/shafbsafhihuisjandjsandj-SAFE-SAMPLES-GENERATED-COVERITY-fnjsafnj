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

int validate_log_level(const char *level) {
    return strcmp(level, "INFO") == 0 || strcmp(level, "WARN") == 0 ||
           strcmp(level, "ERROR") == 0 || strcmp(level, "DEBUG") == 0;
}

int add_log_entry(LogBuffer *buffer, const char *timestamp, const char *level, const char *message) {
    if (buffer->count >= buffer->capacity) {
        int new_capacity = buffer->capacity * 2;
        if (new_capacity <= 0) new_capacity = 10;
        LogEntry *new_entries = realloc(buffer->entries, new_capacity * sizeof(LogEntry));
        if (!new_entries) return 0;
        buffer->entries = new_entries;
        buffer->capacity = new_capacity;
    }

    LogEntry *entry = &buffer->entries[buffer->count];
    strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    strncpy(entry->level, level, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
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
    LogBuffer buffer = {0};
    buffer.capacity = 10;
    buffer.entries = malloc(buffer.capacity * sizeof(LogEntry));
    if (!buffer.entries) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    char line[MAX_LINE_LENGTH];
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL Message)\n");
    printf("Enter 'END' on a separate line to finish input\n");

    while (fgets(line, sizeof(line), stdin)) {
        if (strncmp(line, "END", 3) == 0 && (line[3] == '\n' || line[3] == '\0')) {
            break;
        }

        char timestamp[32], level[16], message[256];
        struct tm tm = {0};
        
        int parsed = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
        if (parsed < 3) {
            printf("Invalid format. Skipping line.\n");
            continue;
        }

        if (!parse_timestamp(timestamp, &tm)) {
            printf("Invalid timestamp format. Skipping line.\n");
            continue;
        }

        if (!validate_log_level(level)) {
            printf("Invalid log level. Skipping line.\n");
            continue;
        }

        if (!add_log_entry(&buffer, timestamp, level, message)) {
            fprintf(stderr, "Failed to add log entry\n");
            break;
        }
    }

    if (buffer.count > 0) {
        analyze_logs(&buffer);
    } else {
        printf("No valid log entries to analyze.\n");
    }

    free(buffer.entries);
    return 0;
}