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

    int parsed = sscanf(line, "%d %15s %1023[^\n]", &timestamp, level, message);
    if (parsed == 3 && timestamp >= 0) {
        if (strlen(level) < sizeof(entry->level) && strlen(message) < sizeof(entry->message)) {
            strncpy(entry->level, level, sizeof(entry->level) - 1);
            strncpy(entry->message, message, sizeof(entry->message) - 1);
            entry->level[sizeof(entry->level) - 1] = '\0';
            entry->message[sizeof(entry->message) - 1] = '\0';
            entry->timestamp = timestamp;
            return parse_log_recursive(file, entries, max_entries, current_depth + 1);
        }
    }

    return parse_log_recursive(file, entries, max_entries, current_depth);
}

void analyze_logs_recursive(struct LogEntry *entries, int count, int index, int *error_count, int *warning_count, int *info_count) {
    if (index >= count) {
        return;
    }

    if (strcmp(entries[index].level, "ERROR") == 0) {
        (*error_count)++;
    } else if (strcmp(entries[index].level, "WARNING") == 0) {
        (*warning_count)++;
    } else if (strcmp(entries[index].level, "INFO") == 0) {
        (*info_count)++;
    }

    analyze_logs_recursive(entries, count, index + 1, error_count, warning_count, info_count);
}

void print_logs_recursive(struct LogEntry *entries, int count, int index) {
    if (index >= count) {
        return;
    }

    printf("Time: %d | Level: %-7s | Message: %s\n", 
           entries[index].timestamp, entries[index].level, entries[index].message);
    print_logs_recursive(entries, count, index + 1);
}

int main(void) {
    struct LogEntry entries[100];
    int entry_count = 0;
    int error_count = 0, warning_count = 0, info_count = 0;

    printf("Enter log entries (format: timestamp LEVEL message)\n");
    printf("Supported levels: ERROR, WARNING, INFO\n");
    printf("Enter 'END' on a separate line to finish input\n");

    entry_count = parse_log_recursive(stdin, entries, 100, 0);

    if (entry_count > 0) {
        printf("\nParsed Log Entries:\n");
        print_logs_recursive(entries, entry_count, 0);

        analyze_logs_recursive(entries, entry_count, 0, &error_count, &warning_count, &info_count);

        printf("\nLog Analysis Summary:\n");
        printf("ERROR entries: %d\n", error_count);
        printf("WARNING entries: %d\n", warning_count);
        printf("INFO entries: %d\n", info_count);
        printf("Total entries: %d\n", entry_count);
    } else {
        printf("No valid log entries found.\n");
    }

    return 0;
}