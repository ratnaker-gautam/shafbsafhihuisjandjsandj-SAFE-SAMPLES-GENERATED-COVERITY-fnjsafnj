//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: recursion ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_LEVELS 100

struct LogEntry {
    char level[16];
    char message[MAX_LINE_LENGTH];
    int timestamp;
};

int parse_log_recursive(FILE *file, struct LogEntry *entries, int max_entries, int current_depth) {
    if (current_depth >= max_entries || current_depth >= MAX_LEVELS) {
        return current_depth;
    }

    char buffer[MAX_LINE_LENGTH];
    if (fgets(buffer, sizeof(buffer), file) == NULL) {
        return current_depth;
    }

    if (strlen(buffer) == 0 || buffer[0] == '\n') {
        return parse_log_recursive(file, entries, max_entries, current_depth);
    }

    struct LogEntry *entry = &entries[current_depth];
    int timestamp;
    char level[16];
    char message[MAX_LINE_LENGTH];

    int parsed = sscanf(buffer, "%d %15s %1023[^\n]", &timestamp, level, message);
    if (parsed != 3) {
        return parse_log_recursive(file, entries, max_entries, current_depth);
    }

    if (timestamp < 0 || timestamp > 2147483647) {
        return parse_log_recursive(file, entries, max_entries, current_depth);
    }

    int valid_level = 0;
    if (strcmp(level, "INFO") == 0 || strcmp(level, "WARNING") == 0 || 
        strcmp(level, "ERROR") == 0 || strcmp(level, "DEBUG") == 0) {
        valid_level = 1;
    }

    if (!valid_level) {
        return parse_log_recursive(file, entries, max_entries, current_depth);
    }

    strncpy(entry->level, level, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    entry->timestamp = timestamp;

    return parse_log_recursive(file, entries, max_entries, current_depth + 1);
}

void analyze_logs_recursive(struct LogEntry *entries, int count, int index, int *info_count, int *warning_count, int *error_count, int *debug_count) {
    if (index >= count) {
        return;
    }

    if (strcmp(entries[index].level, "INFO") == 0) {
        (*info_count)++;
    } else if (strcmp(entries[index].level, "WARNING") == 0) {
        (*warning_count)++;
    } else if (strcmp(entries[index].level, "ERROR") == 0) {
        (*error_count)++;
    } else if (strcmp(entries[index].level, "DEBUG") == 0) {
        (*debug_count)++;
    }

    analyze_logs_recursive(entries, count, index + 1, info_count, warning_count, error_count, debug_count);
}

int main(void) {
    struct LogEntry entries[100];
    int entry_count = 0;

    printf("Enter log entries (format: timestamp LEVEL message). One per line.\n");
    printf("Valid levels: INFO, WARNING, ERROR, DEBUG\n");
    printf("Enter 'END' on a separate line to finish.\n");

    char buffer[MAX_LINE_LENGTH];
    while (entry_count < 100) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }

        buffer[strcspn(buffer, "\n")] = '\0';

        if (strcmp(buffer, "END") == 0) {
            break;
        }

        if (strlen(buffer) == 0) {
            continue;
        }

        if (strlen(buffer) >= MAX_LINE_LENGTH - 1) {
            printf("Line too long, skipping.\n");
            continue;
        }

        int timestamp;
        char level[16];
        char message[MAX_LINE_LENGTH];

        int parsed = sscanf(buffer, "%d %15s %1023[^\n]", &timestamp, level, message);
        if (parsed != 3) {
            printf("Invalid format, skipping.\n");
            continue;
        }

        if (timestamp < 0 || timestamp > 2147483647) {
            printf("Invalid timestamp, skipping.\n");
            continue;
        }

        int valid_level = 0;
        if (strcmp(level, "INFO") == 0 || strcmp(level, "WARNING") == 0 || 
            strcmp(level, "ERROR") == 0 || strcmp(level, "DEBUG") == 0) {
            valid_level = 1;
        }

        if (!valid_level) {
            printf("Invalid level, skipping.\n");
            continue;
        }

        struct LogEntry *entry = &entries[entry_count];
        strncpy(entry->level, level, sizeof(entry->level) - 1);
        entry->level[sizeof(entry->level) - 1] = '\0';
        strncpy(entry->message, message, sizeof(entry->message) - 1);
        entry->message[sizeof(entry->message) - 1] = '\0';
        entry->timestamp = timestamp;