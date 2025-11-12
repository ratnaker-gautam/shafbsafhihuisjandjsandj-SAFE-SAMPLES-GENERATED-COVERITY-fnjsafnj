//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
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
    if (strcmp(level, "ERROR") == 0) return 3;
    if (strcmp(level, "WARN") == 0) return 2;
    if (strcmp(level, "INFO") == 0) return 1;
    if (strcmp(level, "DEBUG") == 0) return 0;
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
    int line_count = 0;

    while (fgets(line, sizeof(line), stdin) != NULL && line_count < 1000) {
        line_count++;
        if (strlen(line) >= MAX_LINE_LENGTH - 1) {
            fprintf(stderr, "Line too long: %d\n", line_count);
            continue;
        }

        char timestamp[32] = {0};
        char level[16] = {0};
        char message[256] = {0};

        int parsed = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
        if (parsed != 3) {
            fprintf(stderr, "Invalid log format at line %d\n", line_count);
            continue;
        }

        if (!validate_timestamp(timestamp)) {
            fprintf(stderr, "Invalid timestamp at line %d\n", line_count);
            continue;
        }

        int severity = parse_log_level(level);
        if (severity == -1) {
            fprintf(stderr, "Unknown log level at line %d\n", line_count);
            continue;
        }

        LogEntry entry;
        strncpy(entry.timestamp, timestamp, sizeof(entry.timestamp) - 1);
        strncpy(entry.level, level, sizeof(entry.level) - 1);
        strncpy(entry.message, message, sizeof(entry.message) - 1);
        entry.severity = severity;

        if (severity >= 2) {
            if (heap_count < MAX_ENTRIES) {
                heap_logs[heap_count] = entry;
                heap_count++;
            }
        } else {
            if (stack_count < 100) {
                stack_logs[stack_count] = entry;
                stack_count++;
            }
        }
    }

    if (ferror(stdin)) {
        fprintf(stderr, "Error reading input\n");
        free(heap_logs);
        return 1;
    }

    printf("High severity logs (heap): %d\n", heap_count);
    for (int i = 0; i < heap_count; i++) {
        printf("%s %s %s\n", heap_logs[i].timestamp, heap_logs[i].level, heap_logs[i].message);
    }

    printf("\nLow severity logs (stack): %d\n", stack_count);
    for (int i = 0; i < stack_count; i++) {
        printf("%s %s %s\n", stack_logs[i].timestamp, stack_logs[i].level, stack_logs[i].message);
    }

    int error_count = 0;
    int warn_count = 0;
    for (int i = 0; i < heap_count; i++) {
        if (heap_logs[i].severity == 3) error_count++;
        else if (heap_logs[i].severity == 2) warn_count++;
    }

    printf("\nSummary:\n");
    printf("Errors: %d\n", error_count);
    printf("Warnings: %d\n", warn_count);
    printf("Info/Debug: %d\n", stack_count);

    free(heap_logs);
    return 0;
}