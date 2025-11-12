//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000

typedef struct {
    char timestamp[32];
    char level[16];
    char message[256];
    int severity;
} LogEntry;

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

int compare_entries(const void* a, const void* b) {
    const LogEntry* entry_a = (const LogEntry*)a;
    const LogEntry* entry_b = (const LogEntry*)b;
    return entry_b->severity - entry_a->severity;
}

int main(void) {
    LogEntry* heap_entries = malloc(MAX_ENTRIES * sizeof(LogEntry));
    if (!heap_entries) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    LogEntry stack_entries[100];
    int heap_count = 0;
    int stack_count = 0;
    char buffer[MAX_LINE_LEN];

    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL MESSAGE):\n");
    printf("Enter 'END' on a separate line to finish input.\n");

    while (heap_count < MAX_ENTRIES && stack_count < 100) {
        if (!fgets(buffer, sizeof(buffer), stdin)) {
            break;
        }

        buffer[strcspn(buffer, "\n")] = 0;

        if (strcmp(buffer, "END") == 0) {
            break;
        }

        char timestamp[32], level[16], message[256];
        if (sscanf(buffer, "%31s %15s %255[^\n]", timestamp, level, message) != 3) {
            fprintf(stderr, "Invalid log format. Skipping line.\n");
            continue;
        }

        if (!validate_timestamp(timestamp)) {
            fprintf(stderr, "Invalid timestamp format. Skipping line.\n");
            continue;
        }

        int severity = parse_log_level(level);
        if (severity == 0) {
            fprintf(stderr, "Unknown log level. Skipping line.\n");
            continue;
        }

        if (severity >= 4) {
            if (heap_count < MAX_ENTRIES) {
                strncpy(heap_entries[heap_count].timestamp, timestamp, sizeof(heap_entries[heap_count].timestamp) - 1);
                strncpy(heap_entries[heap_count].level, level, sizeof(heap_entries[heap_count].level) - 1);
                strncpy(heap_entries[heap_count].message, message, sizeof(heap_entries[heap_count].message) - 1);
                heap_entries[heap_count].severity = severity;
                heap_entries[heap_count].timestamp[31] = '\0';
                heap_entries[heap_count].level[15] = '\0';
                heap_entries[heap_count].message[255] = '\0';
                heap_count++;
            }
        } else {
            if (stack_count < 100) {
                strncpy(stack_entries[stack_count].timestamp, timestamp, sizeof(stack_entries[stack_count].timestamp) - 1);
                strncpy(stack_entries[stack_count].level, level, sizeof(stack_entries[stack_count].level) - 1);
                strncpy(stack_entries[stack_count].message, message, sizeof(stack_entries[stack_count].message) - 1);
                stack_entries[stack_count].severity = severity;
                stack_entries[stack_count].timestamp[31] = '\0';
                stack_entries[stack_count].level[15] = '\0';
                stack_entries[stack_count].message[255] = '\0';
                stack_count++;
            }
        }
    }

    printf("\n=== HIGH SEVERITY LOGS (Heap) ===\n");
    if (heap_count > 0) {
        qsort(heap_entries, heap_count, sizeof(LogEntry), compare_entries);
        for (int i = 0; i < heap_count; i++) {
            printf("%s %s: %s\n", heap_entries[i].timestamp, heap_entries[i].level, heap_entries[i].message);
        }
    } else {
        printf("