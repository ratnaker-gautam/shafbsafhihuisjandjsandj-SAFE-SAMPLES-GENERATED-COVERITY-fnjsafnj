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
    
    if (parsed == 3 && timestamp >= 0 && timestamp <= 86400) {
        if (strcmp(level, "INFO") == 0 || strcmp(level, "WARNING") == 0 || 
            strcmp(level, "ERROR") == 0 || strcmp(level, "DEBUG") == 0) {
            strncpy(entry->level, level, sizeof(entry->level) - 1);
            entry->level[sizeof(entry->level) - 1] = '\0';
            strncpy(entry->message, message, sizeof(entry->message) - 1);
            entry->message[sizeof(entry->message) - 1] = '\0';
            entry->timestamp = timestamp;
            return parse_log_recursive(file, entries, max_entries, current_depth + 1);
        }
    }
    
    return parse_log_recursive(file, entries, max_entries, current_depth);
}

void analyze_logs_recursive(struct LogEntry *entries, int count, int index, 
                           int *info_count, int *warning_count, int *error_count, int *debug_count) {
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

void print_analysis_recursive(struct LogEntry *entries, int count, int index) {
    if (index >= count) {
        return;
    }
    
    printf("Time: %05d | Level: %-7s | Message: %s\n", 
           entries[index].timestamp, entries[index].level, entries[index].message);
    print_analysis_recursive(entries, count, index + 1);
}

int main(void) {
    struct LogEntry entries[100];
    int entry_count = 0;
    
    printf("Enter log entries (format: timestamp LEVEL message)\n");
    printf("Supported levels: INFO, WARNING, ERROR, DEBUG\n");
    printf("Timestamp range: 0-86400 (seconds in day)\n");
    printf("Enter empty line to finish input\n");
    
    entry_count = parse_log_recursive(stdin, entries, 100, 0);
    
    if (entry_count == 0) {
        printf("No valid log entries found.\n");
        return 0;
    }
    
    printf("\n=== Log Analysis Results ===\n");
    printf("Total entries processed: %d\n\n", entry_count);
    
    int info_count = 0, warning_count = 0, error_count = 0, debug_count = 0;
    analyze_logs_recursive(entries, entry_count, 0, &info_count, &warning_count, &error_count, &debug_count);
    
    printf("Level Statistics:\n");
    printf("INFO:    %d\n", info_count);
    printf("WARNING: %d\n", warning_count);
    printf("ERROR:   %d\n", error_count);
    printf("DEBUG:   %d\n", debug_count);
    
    printf("\n=== Log Entries ===\n");
    print_analysis_recursive(entries, entry_count, 0);
    
    return 0;
}