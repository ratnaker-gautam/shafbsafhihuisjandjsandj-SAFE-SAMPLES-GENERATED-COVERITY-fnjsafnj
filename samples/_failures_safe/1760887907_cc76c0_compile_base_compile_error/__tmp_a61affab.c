//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

typedef struct {
    char timestamp[32];
    char level[16];
    char message[256];
    int severity;
} LogEntry;

int parse_log_level(const char* level) {
    if (strcmp(level, "DEBUG") == 0) return 0;
    if (strcmp(level, "INFO") == 0) return 1;
    if (strcmp(level, "WARNING") == 0) return 2;
    if (strcmp(level, "ERROR") == 0) return 3;
    if (strcmp(level, "CRITICAL") == 0) return 4;
    return -1;
}

int validate_timestamp(const char* timestamp) {
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
    LogEntry* log_heap = malloc(MAX_ENTRIES * sizeof(LogEntry));
    if (log_heap == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    LogEntry log_stack[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LENGTH];
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL MESSAGE):\n");
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

        char timestamp[32], level[16], message[256];
        int parsed = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
        
        if (parsed != 3) {
            fprintf(stderr, "Invalid log format. Skipping: %s\n", line);
            continue;
        }

        if (!validate_timestamp(timestamp)) {
            fprintf(stderr, "Invalid timestamp format. Skipping: %s\n", timestamp);
            continue;
        }

        int severity = parse_log_level(level);
        if (severity == -1) {
            fprintf(stderr, "Unknown log level. Skipping: %s\n", level);
            continue;
        }

        strncpy(log_stack[entry_count].timestamp, timestamp, sizeof(log_stack[entry_count].timestamp) - 1);
        log_stack[entry_count].timestamp[sizeof(log_stack[entry_count].timestamp) - 1] = '\0';
        strncpy(log_stack[entry_count].level, level, sizeof(log_stack[entry_count].level) - 1);
        log_stack[entry_count].level[sizeof(log_stack[entry_count].level) - 1] = '\0';
        strncpy(log_stack[entry_count].message, message, sizeof(log_stack[entry_count].message) - 1);
        log_stack[entry_count].message[sizeof(log_stack[entry_count].message) - 1] = '\0';
        log_stack[entry_count].severity = severity;

        memcpy(&log_heap[entry_count], &log_stack[entry_count], sizeof(LogEntry));
        entry_count++;
    }

    printf("\nLog Analysis Results:\n");
    printf("Total entries: %d\n", entry_count);
    
    int level_counts[5] = {0};
    const char* level_names[] = {"DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL"};
    
    for (int i = 0; i < entry_count; i++) {
        if (log_heap[i].severity >= 0 && log_heap[i].severity < 5) {
            level_counts[log_heap[i].severity]++;
        }
    }
    
    printf("\nBreakdown by level:\n");
    for (int i = 0; i < 5; i++) {
        printf("%-8s: %d\n", level_names[i], level_counts[i]);
    }
    
    if (entry_count > 0) {
        int max_severity = 0;
        int max_index = 0;
        for (int i = 0; i < entry_count; i++) {
            if (log_heap[i].severity > max_severity) {
                max_severity = log_heap[i].severity;
                max_index = i;
            }
        }
        
        printf("\nMost severe entry:\n");
        printf("Timestamp: %s\n", log_heap[max_index].timestamp);
        printf("Level: %s\n", log_heap[max_index].level