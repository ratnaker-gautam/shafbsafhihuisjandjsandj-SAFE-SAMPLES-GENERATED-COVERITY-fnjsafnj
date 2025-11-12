//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[32];
    char level[16];
    char message[256];
    int severity;
};

int parse_log_level(const char* level) {
    if (strcmp(level, "DEBUG") == 0) return 1;
    if (strcmp(level, "INFO") == 0) return 2;
    if (strcmp(level, "WARNING") == 0) return 3;
    if (strcmp(level, "ERROR") == 0) return 4;
    if (strcmp(level, "CRITICAL") == 0) return 5;
    return 0;
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
    struct LogEntry entries_stack[MAX_ENTRIES];
    struct LogEntry* entries_heap = malloc(MAX_ENTRIES * sizeof(struct LogEntry));
    if (entries_heap == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    int entry_count = 0;
    char line[MAX_LINE_LEN];
    
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
        if (severity == 0) {
            fprintf(stderr, "Unknown log level. Skipping: %s\n", level);
            continue;
        }

        if (entry_count < MAX_ENTRIES / 2) {
            strncpy(entries_stack[entry_count].timestamp, timestamp, sizeof(entries_stack[entry_count].timestamp) - 1);
            strncpy(entries_stack[entry_count].level, level, sizeof(entries_stack[entry_count].level) - 1);
            strncpy(entries_stack[entry_count].message, message, sizeof(entries_stack[entry_count].message) - 1);
            entries_stack[entry_count].severity = severity;
            entries_stack[entry_count].timestamp[31] = '\0';
            entries_stack[entry_count].level[15] = '\0';
            entries_stack[entry_count].message[255] = '\0';
        } else {
            int heap_index = entry_count - (MAX_ENTRIES / 2);
            strncpy(entries_heap[heap_index].timestamp, timestamp, sizeof(entries_heap[heap_index].timestamp) - 1);
            strncpy(entries_heap[heap_index].level, level, sizeof(entries_heap[heap_index].level) - 1);
            strncpy(entries_heap[heap_index].message, message, sizeof(entries_heap[heap_index].message) - 1);
            entries_heap[heap_index].severity = severity;
            entries_heap[heap_index].timestamp[31] = '\0';
            entries_heap[heap_index].level[15] = '\0';
            entries_heap[heap_index].message[255] = '\0';
        }
        
        entry_count++;
    }

    printf("\nLog Analysis Results:\n");
    printf("Total entries: %d\n", entry_count);
    
    int severity_counts[6] = {0};
    for (int i = 0; i < entry_count; i++) {
        int severity;
        if (i < MAX_ENTRIES / 2) {
            severity = entries_stack[i].severity;
        } else {
            severity = entries_heap[i - (MAX_ENTRIES / 2)].severity;
        }
        if (severity >= 1 && severity <= 5) {
            severity_counts[severity]++;
        }
    }
    
    printf("DEBUG: %d\n", severity_counts[1]);
    printf("INFO: %d\n", severity