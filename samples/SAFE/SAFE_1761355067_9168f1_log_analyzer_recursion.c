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
    if (current_depth >= max_entries || current_depth >= MAX_LEVELS) {
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
    int hour, minute, second;
    char level_buf[16];
    char message_buf[MAX_LINE_LENGTH];

    int parsed = sscanf(line, "%d:%d:%d %15s %1023[^\n]", 
                       &hour, &minute, &second, level_buf, message_buf);

    if (parsed == 5 && hour >= 0 && hour < 24 && minute >= 0 && minute < 60 && second >= 0 && second < 60) {
        entry->timestamp = hour * 3600 + minute * 60 + second;
        strncpy(entry->level, level_buf, sizeof(entry->level) - 1);
        entry->level[sizeof(entry->level) - 1] = '\0';
        strncpy(entry->message, message_buf, sizeof(entry->message) - 1);
        entry->message[sizeof(entry->message) - 1] = '\0';
        return parse_log_recursive(file, entries, max_entries, current_depth + 1);
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

void print_entries_recursive(struct LogEntry *entries, int count, int index) {
    if (index >= count) {
        return;
    }

    int hour = entries[index].timestamp / 3600;
    int minute = (entries[index].timestamp % 3600) / 60;
    int second = entries[index].timestamp % 60;
    printf("%02d:%02d:%02d %-8s %s\n", hour, minute, second, entries[index].level, entries[index].message);
    print_entries_recursive(entries, count, index + 1);
}

int main(void) {
    struct LogEntry entries[100];
    int entry_count = 0;
    int error_count = 0, warning_count = 0, info_count = 0;

    printf("Enter log entries (format: HH:MM:SS LEVEL message). Empty line to finish:\n");

    entry_count = parse_log_recursive(stdin, entries, 100, 0);

    if (entry_count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }

    analyze_logs_recursive(entries, entry_count, 0, &error_count, &warning_count, &info_count);

    printf("\nLog Analysis Results:\n");
    printf("Total entries: %d\n", entry_count);
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);

    printf("\nAll log entries:\n");
    print_entries_recursive(entries, entry_count, 0);

    return 0;
}