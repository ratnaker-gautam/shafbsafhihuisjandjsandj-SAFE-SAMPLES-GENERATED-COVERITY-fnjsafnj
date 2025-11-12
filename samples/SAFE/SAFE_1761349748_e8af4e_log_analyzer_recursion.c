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

    char input[MAX_LINE_LENGTH];
    FILE *temp_file = tmpfile();
    if (temp_file == NULL) {
        printf("Error: Cannot create temporary file\n");
        return 1;
    }

    while (fgets(input, sizeof(input), stdin) != NULL) {
        if (strncmp(input, "END", 3) == 0 && (input[3] == '\n' || input[3] == '\0')) {
            break;
        }
        if (fputs(input, temp_file) == EOF) {
            printf("Error: Cannot write to temporary file\n");
            fclose(temp_file);
            return 1;
        }
    }

    rewind(temp_file);
    entry_count = parse_log_recursive(temp_file, entries, 100, 0);
    fclose(temp_file);

    if (entry_count == 0) {
        printf("No valid log entries found.\n");
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