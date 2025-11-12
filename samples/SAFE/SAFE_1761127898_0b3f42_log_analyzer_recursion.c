//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: log_analyzer
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
    if (current_depth >= max_entries || feof(file) || current_depth >= MAX_LEVELS) {
        return current_depth;
    }

    char line[MAX_LINE_LENGTH];
    if (fgets(line, sizeof(line), file) == NULL) {
        return current_depth;
    }

    if (strlen(line) == 0 || line[0] == '\n') {
        return parse_log_recursive(file, entries, max_entries, current_depth);
    }

    struct LogEntry *entry = &entries[current_depth];
    int timestamp;
    char level[16];
    char message[MAX_LINE_LENGTH];

    if (sscanf(line, "%d %15s %1023[^\n]", &timestamp, level, message) == 3) {
        if (timestamp >= 0 && timestamp <= 86400) {
            entry->timestamp = timestamp;
            strncpy(entry->level, level, sizeof(entry->level) - 1);
            entry->level[sizeof(entry->level) - 1] = '\0';
            strncpy(entry->message, message, sizeof(entry->message) - 1);
            entry->message[sizeof(entry->message) - 1] = '\0';
            return parse_log_recursive(file, entries, max_entries, current_depth + 1);
        }
    }

    return parse_log_recursive(file, entries, max_entries, current_depth);
}

void print_logs_recursive(struct LogEntry *entries, int count, int index) {
    if (index >= count) {
        return;
    }

    printf("Time: %d, Level: %s, Message: %s\n", 
           entries[index].timestamp, entries[index].level, entries[index].message);
    print_logs_recursive(entries, count, index + 1);
}

int count_errors_recursive(struct LogEntry *entries, int count, int index) {
    if (index >= count) {
        return 0;
    }

    int current = 0;
    if (strcmp(entries[index].level, "ERROR") == 0) {
        current = 1;
    }
    return current + count_errors_recursive(entries, count, index + 1);
}

int main(void) {
    struct LogEntry entries[100];
    int entry_count = 0;

    printf("Enter log entries (format: timestamp LEVEL message)\n");
    printf("Example: 3600 INFO System started\n");
    printf("Enter 'END' on a separate line to finish input\n");

    entry_count = parse_log_recursive(stdin, entries, 100, 0);

    if (entry_count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }

    printf("\nParsed log entries:\n");
    print_logs_recursive(entries, entry_count, 0);

    int error_count = count_errors_recursive(entries, entry_count, 0);
    printf("\nTotal ERROR entries: %d\n", error_count);

    return 0;
}