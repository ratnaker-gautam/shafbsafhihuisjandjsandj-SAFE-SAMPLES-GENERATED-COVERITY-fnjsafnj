//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
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
    LogEntry* heap_logs = malloc(MAX_ENTRIES * sizeof(LogEntry));
    if (heap_logs == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    LogEntry stack_logs[100];
    int heap_count = 0;
    int stack_count = 0;
    char line[MAX_LINE_LENGTH];
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL MESSAGE):\n");
    printf("Enter 'END' on a new line to finish input.\n");

    while (fgets(line, sizeof(line), stdin) != NULL) {
        if (strncmp(line, "END", 3) == 0 && (line[3] == '\n' || line[3] == '\0')) {
            break;
        }

        line[strcspn(line, "\n")] = 0;
        
        if (strlen(line) == 0) continue;

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

        LogEntry entry;
        strncpy(entry.timestamp, timestamp, sizeof(entry.timestamp) - 1);
        entry.timestamp[sizeof(entry.timestamp) - 1] = '\0';
        strncpy(entry.level, level, sizeof(entry.level) - 1);
        entry.level[sizeof(entry.level) - 1] = '\0';
        strncpy(entry.message, message, sizeof(entry.message) - 1);
        entry.message[sizeof(entry.message) - 1] = '\0';
        entry.severity = severity;

        if (severity >= 2) {
            if (heap_count < MAX_ENTRIES) {
                heap_logs[heap_count] = entry;
                heap_count++;
            } else {
                fprintf(stderr, "Heap log storage full. Skipping: %s\n", line);
            }
        } else {
            if (stack_count < 100) {
                stack_logs[stack_count] = entry;
                stack_count++;
            } else {
                fprintf(stderr, "Stack log storage full. Skipping: %s\n", line);
            }
        }
    }

    printf("\n=== LOG ANALYSIS RESULTS ===\n");
    printf("High severity logs (heap storage): %d\n", heap_count);
    printf("Low severity logs (stack storage): %d\n", stack_count);
    
    if (heap_count > 0) {
        printf("\nHigh severity entries (WARNING and above):\n");
        for (int i = 0; i < heap_count; i++) {
            printf("%s %s %s\n", heap_logs[i].timestamp, heap_logs[i].level, heap_logs[i].message);
        }
    }

    if (stack_count > 0) {
        printf("\nLow severity entries (INFO and DEBUG):\n");
        for (int i = 0; i < stack_count; i++) {
            printf("%s %s %s\n", stack_logs[i].timestamp, stack_logs[i].level, stack_logs[i].message);
        }
    }

    int total_errors = 0;
    for (int i = 0; i < heap_count; i++) {
        if (heap_logs[i].severity >=