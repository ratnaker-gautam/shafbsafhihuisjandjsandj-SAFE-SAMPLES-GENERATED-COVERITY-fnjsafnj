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
    if (parsed != 3) {
        return parse_log_recursive(file, entries, max_entries, current_depth);
    }

    if (timestamp < 0 || timestamp > 2147483647) {
        return parse_log_recursive(file, entries, max_entries, current_depth);
    }

    int valid_level = 0;
    const char *valid_levels[] = {"INFO", "WARNING", "ERROR", "DEBUG"};
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

int main() {
    printf("Log Analyzer\n");
    printf("Enter log file path: ");
    
    char filename[256];
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }

    size_t len = strlen(filename);
    if (len > 0 && filename[len - 1] == '\n') {
        filename[len - 1] = '\0';
    }

    if (strlen(filename) == 0) {
        printf("Invalid filename\n");
        return 1;
    }

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Cannot open file: %s\n", filename);
        return 1;
    }

    struct LogEntry entries[100];
    int entry_count = parse_log_recursive(file, entries, 100, 0);
    fclose(file);

    if (entry_count == 0) {
        printf("No valid log entries found\n");
        return 0;
    }

    int info_count = 0, warning_count = 0, error_count = 0, debug_count = 0;
    analyze_logs_recursive(entries, entry_count, 0, &info_count, &warning_count, &error_count, &debug_count);

    printf("\nLog Analysis Results:\n");
    printf("Total entries: %d\n", entry_count);
    printf("INFO: %d\n", info_count);
    printf("WARNING: %d\n", warning_count);
    printf("ERROR: %d\n", error_count);
    printf("DEBUG: %d\n", debug_count);

    return 0;
}