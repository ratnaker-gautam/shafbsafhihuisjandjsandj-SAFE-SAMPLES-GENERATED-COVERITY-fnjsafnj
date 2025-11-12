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
    if (heap_entries == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    char filename[256];
    printf("Enter log file name: ");
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        free(heap_entries);
        return 1;
    }
    
    size_t len = strlen(filename);
    if (len > 0 && filename[len - 1] == '\n') {
        filename[len - 1] = '\0';
    }
    
    if (strlen(filename) == 0) {
        fprintf(stderr, "Filename cannot be empty\n");
        free(heap_entries);
        return 1;
    }

    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Cannot open file: %s\n", filename);
        free(heap_entries);
        return 1;
    }

    char line[MAX_LINE_LENGTH];
    int entry_count = 0;
    LogEntry stack_entry;

    while (fgets(line, sizeof(line), file) != NULL && entry_count < MAX_ENTRIES) {
        size_t line_len = strlen(line);
        if (line_len > 0 && line[line_len - 1] == '\n') {
            line[line_len - 1] = '\0';
        }

        if (strlen(line) == 0) continue;

        char timestamp[32] = {0};
        char level[16] = {0};
        char message[256] = {0};

        int parsed = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
        if (parsed != 3) {
            continue;
        }

        if (!validate_timestamp(timestamp)) {
            continue;
        }

        int severity = parse_log_level(level);
        if (severity == 0) {
            continue;
        }

        strncpy(stack_entry.timestamp, timestamp, sizeof(stack_entry.timestamp) - 1);
        stack_entry.timestamp[sizeof(stack_entry.timestamp) - 1] = '\0';
        strncpy(stack_entry.level, level, sizeof(stack_entry.level) - 1);
        stack_entry.level[sizeof(stack_entry.level) - 1] = '\0';
        strncpy(stack_entry.message, message, sizeof(stack_entry.message) - 1);
        stack_entry.message[sizeof(stack_entry.message) - 1] = '\0';
        stack_entry.severity = severity;

        heap_entries[entry_count] = stack_entry;
        entry_count++;
    }

    fclose(file);

    if (entry_count == 0) {
        printf("No valid log entries found\n");
        free(heap_entries);
        return 0;
    }

    qsort(heap_entries, entry_count, sizeof(LogEntry), compare_entries);

    printf("\nLog entries sorted by severity:\n");
    printf("===============================\n");
    
    for (int i = 0; i < entry_count; i++) {
        printf("%s [%s] %s\n", 
               heap_entries[i].timestamp,
               heap_entries[i].level,
               heap_entries[i].message);
    }

    int severity_counts[6] = {0};
    for (int i = 0; i < entry_count; i++) {
        if (heap_entries[i].severity >= 1 && heap_entries[i].severity <= 5) {
            severity_counts[heap_entries[i].severity]++;
        }