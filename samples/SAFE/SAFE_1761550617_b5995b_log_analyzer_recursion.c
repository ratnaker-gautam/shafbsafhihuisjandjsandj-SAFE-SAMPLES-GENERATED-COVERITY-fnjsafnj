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

    if (timestamp < 0 || timestamp > 2147483647) {
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

void analyze_log_levels_recursive(struct LogEntry *entries, int count, int *level_counts, int current_index) {
    if (current_index >= count) {
        return;
    }

    const char *level = entries[current_index].level;
    if (strcmp(level, "INFO") == 0) {
        level_counts[0]++;
    } else if (strcmp(level, "WARNING") == 0) {
        level_counts[1]++;
    } else if (strcmp(level, "ERROR") == 0) {
        level_counts[2]++;
    } else if (strcmp(level, "DEBUG") == 0) {
        level_counts[3]++;
    }

    analyze_log_levels_recursive(entries, count, level_counts, current_index + 1);
}

void print_analysis_recursive(int *level_counts, const char **level_names, int current_index) {
    if (current_index >= 4) {
        return;
    }

    printf("%s: %d\n", level_names[current_index], level_counts[current_index]);
    print_analysis_recursive(level_counts, level_names, current_index + 1);
}

int main(void) {
    struct LogEntry entries[1000];
    int level_counts[4] = {0};
    const char *level_names[] = {"INFO", "WARNING", "ERROR", "DEBUG"};

    printf("Enter log data (format: timestamp LEVEL message). Empty line to finish:\n");

    int entry_count = parse_log_recursive(stdin, entries, 1000, 0);
    
    if (entry_count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }

    analyze_log_levels_recursive(entries, entry_count, level_counts, 0);
    
    printf("\nLog Analysis Results:\n");
    printf("Total entries: %d\n", entry_count);
    print_analysis_recursive(level_counts, level_names, 0);

    return 0;
}