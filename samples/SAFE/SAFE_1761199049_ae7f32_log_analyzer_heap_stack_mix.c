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
    LogEntry* heap_entries = malloc(MAX_ENTRIES * sizeof(LogEntry));
    if (heap_entries == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    char input_buffer[MAX_LINE_LENGTH];
    int entry_count = 0;
    int error_count = 0;
    int warning_count = 0;

    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL MESSAGE):\n");
    printf("Enter 'END' on a new line to finish input.\n");

    while (entry_count < MAX_ENTRIES) {
        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
            break;
        }

        input_buffer[strcspn(input_buffer, "\n")] = 0;

        if (strcmp(input_buffer, "END") == 0) {
            break;
        }

        if (strlen(input_buffer) == 0) {
            continue;
        }

        char timestamp[32];
        char level[16];
        char message[256];

        int parsed = sscanf(input_buffer, "%31s %15s %255[^\n]", 
                           timestamp, level, message);
        
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
            fprintf(stderr, "Unknown log level. Skipping line.\n");
            continue;
        }

        if (strlen(message) == 0) {
            fprintf(stderr, "Empty message. Skipping line.\n");
            continue;
        }

        LogEntry stack_entry;
        strncpy(stack_entry.timestamp, timestamp, sizeof(stack_entry.timestamp) - 1);
        stack_entry.timestamp[sizeof(stack_entry.timestamp) - 1] = '\0';
        strncpy(stack_entry.level, level, sizeof(stack_entry.level) - 1);
        stack_entry.level[sizeof(stack_entry.level) - 1] = '\0';
        strncpy(stack_entry.message, message, sizeof(stack_entry.message) - 1);
        stack_entry.message[sizeof(stack_entry.message) - 1] = '\0';
        stack_entry.severity = severity;

        heap_entries[entry_count] = stack_entry;
        entry_count++;

        if (severity >= 3) error_count++;
        if (severity == 2) warning_count++;
    }

    printf("\nLog Analysis Summary:\n");
    printf("Total entries: %d\n", entry_count);
    printf("Errors: %d\n", error_count);
    printf("Warnings: %d\n", warning_count);

    if (entry_count > 0) {
        printf("\nRecent entries (last 5):\n");
        int start = (entry_count > 5) ? entry_count - 5 : 0;
        for (int i = start; i < entry_count; i++) {
            printf("%s [%s] %s\n", heap_entries[i].timestamp, 
                   heap_entries[i].level, heap_entries[i].message);
        }
    }

    free(heap_entries);
    return 0;
}