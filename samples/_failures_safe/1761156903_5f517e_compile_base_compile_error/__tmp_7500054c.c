//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[32];
    char level[16];
    char message[256];
    int severity;
};

int parse_severity(const char* level) {
    if (strcmp(level, "ERROR") == 0) return 3;
    if (strcmp(level, "WARN") == 0) return 2;
    if (strcmp(level, "INFO") == 0) return 1;
    if (strcmp(level, "DEBUG") == 0) return 0;
    return -1;
}

int validate_timestamp(const char* ts) {
    if (strlen(ts) != 19) return 0;
    for (int i = 0; i < 19; i++) {
        if (i == 4 || i == 7) {
            if (ts[i] != '-') return 0;
        } else if (i == 10) {
            if (ts[i] != ' ') return 0;
        } else if (i == 13 || i == 16) {
            if (ts[i] != ':') return 0;
        } else {
            if (!isdigit(ts[i])) return 0;
        }
    }
    return 1;
}

int main(void) {
    struct LogEntry* heap_entries = malloc(MAX_ENTRIES * sizeof(struct LogEntry));
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
    int error_count = 0;
    int warning_count = 0;

    while (fgets(line, sizeof(line), file) != NULL && entry_count < MAX_ENTRIES) {
        size_t line_len = strlen(line);
        if (line_len > 0 && line[line_len - 1] == '\n') {
            line[line_len - 1] = '\0';
            line_len--;
        }

        char timestamp[32] = {0};
        char level[16] = {0};
        char message[256] = {0};

        if (sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message) != 3) {
            continue;
        }

        if (!validate_timestamp(timestamp)) {
            continue;
        }

        int severity = parse_severity(level);
        if (severity == -1) {
            continue;
        }

        if (strlen(message) == 0) {
            continue;
        }

        strncpy(heap_entries[entry_count].timestamp, timestamp, sizeof(heap_entries[entry_count].timestamp) - 1);
        heap_entries[entry_count].timestamp[sizeof(heap_entries[entry_count].timestamp) - 1] = '\0';
        strncpy(heap_entries[entry_count].level, level, sizeof(heap_entries[entry_count].level) - 1);
        heap_entries[entry_count].level[sizeof(heap_entries[entry_count].level) - 1] = '\0';
        strncpy(heap_entries[entry_count].message, message, sizeof(heap_entries[entry_count].message) - 1);
        heap_entries[entry_count].message[sizeof(heap_entries[entry_count].message) - 1] = '\0';
        heap_entries[entry_count].severity = severity;

        if (severity == 3) error_count++;
        if (severity == 2) warning_count++;

        entry_count++;
    }

    fclose(file);

    struct LogEntry stack_entries[10];
    int stack_count = 0;

    for (int i = 0; i < entry_count && stack_count < 10; i++) {
        if (heap_entries[i].severity >= 2) {
            strncpy(stack_entries[stack_count].timestamp, heap_entries[i].timestamp, sizeof(stack_entries[stack_count].timestamp) - 1);
            stack_entries[stack_count].timestamp[sizeof(stack_entries[stack_count].timestamp) - 1] = '\0';
            strncpy(stack_entries[stack_count].level, heap_entries[i].level, sizeof(stack_entries[stack_count].level) - 1);
            stack_entries[stack_count].level[sizeof(stack_entries[stack_count].level) - 1] = '\0';
            strncpy(stack