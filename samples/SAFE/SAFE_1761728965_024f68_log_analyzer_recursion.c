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
    if (current_depth >= max_entries || feof(file)) {
        return 0;
    }

    char buffer[MAX_LINE_LENGTH];
    if (fgets(buffer, sizeof(buffer), file) == NULL) {
        return 0;
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

    if (timestamp < 0 || timestamp > 999999999) {
        return parse_log_recursive(file, entries, max_entries, current_depth);
    }

    int valid_level = 0;
    const char *valid_levels[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    for (int i = 0; i < 4; i++) {
        if (strcmp(level, valid_levels[i]) == 0) {
            valid_level = 1;
            break;
        }
    }

    if (!valid_level) {
        return parse_log_recursive(file, entries, max_entries, current_depth);
    }

    strncpy(entry->level, level, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    entry->timestamp = timestamp;

    int count = parse_log_recursive(file, entries, max_entries, current_depth + 1);
    return count + 1;
}

void print_entries_recursive(struct LogEntry *entries, int count, int index) {
    if (index >= count) {
        return;
    }

    printf("Time: %d | Level: %-6s | Message: %s\n", 
           entries[index].timestamp, 
           entries[index].level, 
           entries[index].message);

    print_entries_recursive(entries, count, index + 1);
}

int count_level_recursive(struct LogEntry *entries, int count, int index, const char *level) {
    if (index >= count) {
        return 0;
    }

    int matches = 0;
    if (strcmp(entries[index].level, level) == 0) {
        matches = 1;
    }

    return matches + count_level_recursive(entries, count, index + 1, level);
}

int main(void) {
    struct LogEntry entries[MAX_LEVELS];
    int entry_count = 0;

    printf("Enter log entries (format: timestamp LEVEL message):\n");
    printf("Example: 123456789 INFO System started\n");
    printf("Press Ctrl+D (Linux) or Ctrl+Z (Windows) to finish input.\n");

    entry_count = parse_log_recursive(stdin, entries, MAX_LEVELS, 0);

    if (entry_count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }

    printf("\nParsed %d log entries:\n", entry_count);
    print_entries_recursive(entries, entry_count, 0);

    printf("\nStatistics:\n");
    const char *levels[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    for (int i = 0; i < 4; i++) {
        int count = count_level_recursive(entries, entry_count, 0, levels[i]);
        printf("%s: %d\n", levels[i], count);
    }

    return 0;
}