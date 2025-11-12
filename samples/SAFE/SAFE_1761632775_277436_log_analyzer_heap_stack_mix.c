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
    int heap_count = 0;
    int stack_count = 0;
    char buffer[MAX_LINE_LENGTH];
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL MESSAGE):\n");
    printf("Enter 'END' on a separate line to finish input.\n");

    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        if (strncmp(buffer, "END", 3) == 0 && (buffer[3] == '\n' || buffer[3] == '\0')) {
            break;
        }

        char timestamp[32] = {0};
        char level[16] = {0};
        char message[256] = {0};
        
        int parsed = sscanf(buffer, "%31s %15s %255[^\n]", timestamp, level, message);
        if (parsed != 3) {
            fprintf(stderr, "Invalid log format. Skipping line.\n");
            continue;
        }

        if (!validate_timestamp(timestamp)) {
            fprintf(stderr, "Invalid timestamp format. Skipping line.\n");
            continue;
        }

        int severity = parse_log_level(level);
        if (severity == -1) {
            fprintf(stderr, "Invalid log level. Skipping line.\n");
            continue;
        }

        if (severity >= 2) {
            if (heap_count < MAX_ENTRIES) {
                strncpy(log_heap[heap_count].timestamp, timestamp, sizeof(log_heap[heap_count].timestamp) - 1);
                strncpy(log_heap[heap_count].level, level, sizeof(log_heap[heap_count].level) - 1);
                strncpy(log_heap[heap_count].message, message, sizeof(log_heap[heap_count].message) - 1);
                log_heap[heap_count].severity = severity;
                heap_count++;
            } else {
                fprintf(stderr, "Heap storage full. Skipping line.\n");
            }
        } else {
            if (stack_count < MAX_ENTRIES) {
                strncpy(log_stack[stack_count].timestamp, timestamp, sizeof(log_stack[stack_count].timestamp) - 1);
                strncpy(log_stack[stack_count].level, level, sizeof(log_stack[stack_count].level) - 1);
                strncpy(log_stack[stack_count].message, message, sizeof(log_stack[stack_count].message) - 1);
                log_stack[stack_count].severity = severity;
                stack_count++;
            } else {
                fprintf(stderr, "Stack storage full. Skipping line.\n");
            }
        }
    }

    printf("\n=== HIGH SEVERITY LOGS (Heap Storage) ===\n");
    for (int i = 0; i < heap_count; i++) {
        printf("%s %s %s\n", log_heap[i].timestamp, log_heap[i].level, log_heap[i].message);
    }

    printf("\n=== LOW SEVERITY LOGS (Stack Storage) ===\n");
    for (int i = 0; i < stack_count; i++) {
        printf("%s %s %s\n", log_stack[i].timestamp, log_stack[i].level, log_stack[i].message);
    }

    printf("\nSummary:\n");
    printf("High severity logs (WARNING+): %d\n", heap_count);
    printf("Low severity logs (DEBUG/INFO): %d\n", stack_count);
    printf("Total logs processed: %d\n", heap_count + stack_count);

    free(log_heap);
    return 0;
}