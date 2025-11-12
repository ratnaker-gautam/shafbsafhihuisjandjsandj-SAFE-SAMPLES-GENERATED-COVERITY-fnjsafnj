//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
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

int compare_entries(const void* a, const void* b) {
    const LogEntry* entry_a = (const LogEntry*)a;
    const LogEntry* entry_b = (const LogEntry*)b;
    return strcmp(entry_a->timestamp, entry_b->timestamp);
}

int main(void) {
    LogEntry* entries = malloc(MAX_ENTRIES * sizeof(LogEntry));
    if (entries == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    char buffer[MAX_LINE_LEN];
    int entry_count = 0;
    int error_count = 0;
    int warning_count = 0;

    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL MESSAGE):\n");
    printf("Enter 'END' on a separate line to finish input.\n");

    while (entry_count < MAX_ENTRIES) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }

        buffer[strcspn(buffer, "\n")] = 0;

        if (strcmp(buffer, "END") == 0) {
            break;
        }

        if (strlen(buffer) >= MAX_LINE_LEN - 1) {
            fprintf(stderr, "Line too long, skipping\n");
            continue;
        }

        char timestamp[32];
        char level[16];
        char message[256];

        int parsed = sscanf(buffer, "%31s %15s %255[^\n]", 
                           timestamp, level, message);
        
        if (parsed != 3) {
            fprintf(stderr, "Invalid log format, skipping\n");
            continue;
        }

        if (!validate_timestamp(timestamp)) {
            fprintf(stderr, "Invalid timestamp format, skipping\n");
            continue;
        }

        int severity = parse_log_level(level);
        if (severity == -1) {
            fprintf(stderr, "Unknown log level, skipping\n");
            continue;
        }

        if (strlen(message) == 0) {
            fprintf(stderr, "Empty message, skipping\n");
            continue;
        }

        strncpy(entries[entry_count].timestamp, timestamp, 
                sizeof(entries[entry_count].timestamp) - 1);
        strncpy(entries[entry_count].level, level, 
                sizeof(entries[entry_count].level) - 1);
        strncpy(entries[entry_count].message, message, 
                sizeof(entries[entry_count].message) - 1);
        entries[entry_count].severity = severity;

        entries[entry_count].timestamp[31] = '\0';
        entries[entry_count].level[15] = '\0';
        entries[entry_count].message[255] = '\0';

        if (severity >= 3) error_count++;
        if (severity >= 2) warning_count++;

        entry_count++;
    }

    if (entry_count > 0) {
        qsort(entries, entry_count, sizeof(LogEntry), compare_entries);

        printf("\nSorted log entries:\n");
        printf("===================\n");
        
        for (int i = 0; i < entry_count; i++) {
            printf("%s %-8s %s\n", 
                   entries[i].timestamp, 
                   entries[i].level, 
                   entries[i].message);
        }

        printf("\nSummary:\n");
        printf("Total entries: %d\n", entry_count);
        printf("Errors: %d\n", error_count);
        printf("Warnings: %d\n", warning_count);

        LogEntry stack_entries[10];
        int stack_count = (entry_count < 10) ? entry_count : 10;
        
        for (int i = 0; i < stack_count; i++) {
            strncpy(stack_entries[i].timestamp, entries[i].timestamp, sizeof(stack_entries[i].timestamp) - 1);
            strnc