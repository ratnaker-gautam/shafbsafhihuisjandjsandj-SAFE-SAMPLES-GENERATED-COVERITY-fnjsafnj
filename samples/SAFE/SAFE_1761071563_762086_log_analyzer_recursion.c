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
    if (current_depth >= max_entries || feof(file) || current_depth >= MAX_LEVELS) {
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
    
    if (parsed == 3 && timestamp >= 0 && timestamp <= 86400) {
        strncpy(entry->level, level, sizeof(entry->level) - 1);
        entry->level[sizeof(entry->level) - 1] = '\0';
        strncpy(entry->message, message, sizeof(entry->message) - 1);
        entry->message[sizeof(entry->message) - 1] = '\0';
        entry->timestamp = timestamp;
        
        return parse_log_recursive(file, entries, max_entries, current_depth + 1);
    }

    return parse_log_recursive(file, entries, max_entries, current_depth);
}

void analyze_log_levels_recursive(struct LogEntry *entries, int count, int *level_counts, int index) {
    if (index >= count) {
        return;
    }

    const char *level = entries[index].level;
    if (strcmp(level, "INFO") == 0) {
        level_counts[0]++;
    } else if (strcmp(level, "WARNING") == 0) {
        level_counts[1]++;
    } else if (strcmp(level, "ERROR") == 0) {
        level_counts[2]++;
    } else if (strcmp(level, "DEBUG") == 0) {
        level_counts[3]++;
    } else {
        level_counts[4]++;
    }

    analyze_log_levels_recursive(entries, count, level_counts, index + 1);
}

void print_analysis_recursive(int *level_counts, const char **level_names, int index) {
    if (index >= 5) {
        return;
    }

    printf("%s: %d\n", level_names[index], level_counts[index]);
    print_analysis_recursive(level_counts, level_names, index + 1);
}

int main(void) {
    struct LogEntry entries[100];
    int level_counts[5] = {0};
    const char *level_names[] = {"INFO", "WARNING", "ERROR", "DEBUG", "OTHER"};
    
    printf("Enter log entries (format: timestamp LEVEL message):\n");
    printf("Example: 3600 INFO System started\n");
    printf("Enter 'quit' on a separate line to finish.\n");
    
    int count = parse_log_recursive(stdin, entries, 100, 0);
    
    if (count > 0) {
        analyze_log_levels_recursive(entries, count, level_counts, 0);
        printf("\nLog Analysis Results:\n");
        print_analysis_recursive(level_counts, level_names, 0);
        printf("Total entries: %d\n", count);
    } else {
        printf("No valid log entries found.\n");
    }
    
    return 0;
}