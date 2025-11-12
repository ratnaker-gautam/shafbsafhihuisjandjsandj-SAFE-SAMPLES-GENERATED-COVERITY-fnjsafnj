//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000

typedef struct {
    char timestamp[64];
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
    char line[MAX_LINE_LEN];
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL MESSAGE):\n");
    printf("Enter 'END' to finish input\n");

    while (heap_count < MAX_ENTRIES && stack_count < 100) {
        if (fgets(line, sizeof(line), stdin) == NULL) {
            break;
        }
        
        line[strcspn(line, "\n")] = 0;
        
        if (strcmp(line, "END") == 0) {
            break;
        }

        char timestamp[64];
        char level[16];
        char message[256];
        
        int parsed = sscanf(line, "%63s %15s %255[^\n]", timestamp, level, message);
        if (parsed != 3) {
            fprintf(stderr, "Invalid log format\n");
            continue;
        }

        if (!validate_timestamp(timestamp)) {
            fprintf(stderr, "Invalid timestamp format\n");
            continue;
        }

        int severity = parse_log_level(level);
        if (severity == -1) {
            fprintf(stderr, "Invalid log level\n");
            continue;
        }

        if (severity >= 2) {
            if (heap_count < MAX_ENTRIES) {
                strncpy(heap_logs[heap_count].timestamp, timestamp, sizeof(heap_logs[heap_count].timestamp) - 1);
                strncpy(heap_logs[heap_count].level, level, sizeof(heap_logs[heap_count].level) - 1);
                strncpy(heap_logs[heap_count].message, message, sizeof(heap_logs[heap_count].message) - 1);
                heap_logs[heap_count].severity = severity;
                heap_logs[heap_count].timestamp[sizeof(heap_logs[heap_count].timestamp) - 1] = '\0';
                heap_logs[heap_count].level[sizeof(heap_logs[heap_count].level) - 1] = '\0';
                heap_logs[heap_count].message[sizeof(heap_logs[heap_count].message) - 1] = '\0';
                heap_count++;
            }
        } else {
            if (stack_count < 100) {
                strncpy(stack_logs[stack_count].timestamp, timestamp, sizeof(stack_logs[stack_count].timestamp) - 1);
                strncpy(stack_logs[stack_count].level, level, sizeof(stack_logs[stack_count].level) - 1);
                strncpy(stack_logs[stack_count].message, message, sizeof(stack_logs[stack_count].message) - 1);
                stack_logs[stack_count].severity = severity;
                stack_logs[stack_count].timestamp[sizeof(stack_logs[stack_count].timestamp) - 1] = '\0';
                stack_logs[stack_count].level[sizeof(stack_logs[stack_count].level) - 1] = '\0';
                stack_logs[stack_count].message[sizeof(stack_logs[stack_count].message) - 1] = '\0';
                stack_count++;
            }
        }
    }

    printf("\nHigh severity logs (heap):\n");
    for (int i = 0; i < heap_count; i++) {
        printf("%s %s %s\n", heap_logs[i].timestamp, heap_logs[i].level, heap_logs[i].message);
    }

    printf("\nLow severity logs (stack):\n");
    for (int i = 0; i < stack_count; i++) {