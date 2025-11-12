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
    const char *valid_levels[] = {"INFO", "WARNING", "ERROR", "DEBUG"};
    for (int i = 0; i < 4; i++) {
        if (strcmp(level, valid_levels[i]) == 0) {
            return 1;
        }
    }
    return 0;
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
    int level_counts[4] = {0};
    const char *levels[] = {"INFO", "WARNING", "ERROR", "DEBUG"};

    for (int i = 0; i < buffer->count; i++) {
        for (int j = 0; j < 4; j++) {
            if (strcmp(buffer->entries[i].level, levels[j]) == 0) {
                level_counts[j]++;
                break;
            }
        }
    }

    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", buffer->count);
    for (int i = 0; i < 4; i++) {
        printf("%s: %d\n", levels[i], level_counts[i]);
    }
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
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL MESSAGE):\n");
    printf("Enter 'END' on a separate line to finish input.\n");

    while (fgets(line, sizeof(line), stdin)) {
        if (strncmp(line, "END", 3) == 0 && (line[3] == '\n' || line[3] == '\0')) {
            break;
        }

        char timestamp[32], level[16], message[256];
        struct tm tm = {0};

        if (sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message) == 3) {
            if (parse_timestamp(timestamp, &tm) && validate_log_level(level)) {
                if (!add_log_entry(&buffer, timestamp, level, message)) {
                    fprintf(stderr, "Failed to add log entry\n");
                    break;
                }
            } else {
                fprintf(stderr, "Invalid log format\n");
            }
        } else {
            fprintf(stderr, "Failed to parse line\n");
        }
    }

    if (buffer.count > 0) {
        analyze_logs(&buffer);
    } else {
        printf("No valid log entries to analyze\n");
    }

    free(buffer.entries);
    return 0;
}