//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[32];
    char level[16];
    char message[256];
    int severity;
};

int parse_log_level(const char* level) {
    if (strcmp(level, "ERROR") == 0) return 3;
    if (strcmp(level, "WARN") == 0) return 2;
    if (strcmp(level, "INFO") == 0) return 1;
    if (strcmp(level, "DEBUG") == 0) return 0;
    return -1;
}

int is_valid_timestamp(const char* timestamp) {
    if (strlen(timestamp) != 19) return 0;
    for (int i = 0; i < 19; i++) {
        if (i == 4 || i == 7) {
            if (timestamp[i] != '-') return 0;
        } else if (i == 10) {
            if (timestamp[i] != ' ') return 0;
        } else if (i == 13 || i == 16) {
            if (timestamp[i] != ':') return 0;
        } else {
            if (!isdigit(timestamp[i])) return 0;
        }
    }
    return 1;
}

int main(void) {
    struct LogEntry* heap_entries = malloc(MAX_ENTRIES * sizeof(struct LogEntry));
    if (heap_entries == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    int entry_count = 0;
    char line[MAX_LINE_LENGTH];
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message):\n");
    printf("Enter 'END' on a separate line to finish input.\n");

    while (entry_count < MAX_ENTRIES) {
        if (fgets(line, sizeof(line), stdin) == NULL) {
            break;
        }
        
        line[strcspn(line, "\n")] = 0;
        
        if (strcmp(line, "END") == 0) {
            break;
        }
        
        if (strlen(line) == 0) {
            continue;
        }

        struct LogEntry stack_entry;
        int parsed = sscanf(line, "%31s %15s %255[^\n]", 
                           stack_entry.timestamp, 
                           stack_entry.level, 
                           stack_entry.message);
        
        if (parsed != 3) {
            fprintf(stderr, "Invalid log format: %s\n", line);
            continue;
        }
        
        if (!is_valid_timestamp(stack_entry.timestamp)) {
            fprintf(stderr, "Invalid timestamp: %s\n", stack_entry.timestamp);
            continue;
        }
        
        stack_entry.severity = parse_log_level(stack_entry.level);
        if (stack_entry.severity == -1) {
            fprintf(stderr, "Invalid log level: %s\n", stack_entry.level);
            continue;
        }
        
        heap_entries[entry_count] = stack_entry;
        entry_count++;
    }

    if (entry_count == 0) {
        printf("No valid log entries to analyze.\n");
        free(heap_entries);
        return 0;
    }

    int severity_counts[4] = {0};
    for (int i = 0; i < entry_count; i++) {
        if (heap_entries[i].severity >= 0 && heap_entries[i].severity <= 3) {
            severity_counts[heap_entries[i].severity]++;
        }
    }

    printf("\nLog Analysis Results:\n");
    printf("Total entries: %d\n", entry_count);
    printf("DEBUG: %d\n", severity_counts[0]);
    printf("INFO: %d\n", severity_counts[1]);
    printf("WARN: %d\n", severity_counts[2]);
    printf("ERROR: %d\n", severity_counts[3]);

    printf("\nRecent entries (last 5):\n");
    int start = (entry_count > 5) ? entry_count - 5 : 0;
    for (int i = start; i < entry_count; i++) {
        printf("%s [%s] %s\n", 
               heap_entries[i].timestamp, 
               heap_entries[i].level, 
               heap_entries[i].message);
    }

    free(heap_entries);
    return 0;
}