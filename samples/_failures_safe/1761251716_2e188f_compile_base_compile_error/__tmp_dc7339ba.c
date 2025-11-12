//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    if (entry_b->severity > entry_a->severity) return 1;
    if (entry_b->severity < entry_a->severity) return -1;
    return 0;
}

int main(void) {
    LogEntry* heap_entries = malloc(MAX_ENTRIES * sizeof(LogEntry));
    if (!heap_entries) {
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
    if (!file) {
        fprintf(stderr, "Cannot open file: %s\n", filename);
        free(heap_entries);
        return 1;
    }

    int entry_count = 0;
    char line[MAX_LINE_LEN];
    
    while (fgets(line, sizeof(line), file) != NULL && entry_count < MAX_ENTRIES) {
        size_t line_len = strlen(line);
        if (line_len > 0 && line[line_len - 1] == '\n') {
            line[line_len - 1] = '\0';
            line_len--;
        }
        
        if (line_len == 0) continue;

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

        if (strlen(message) == 0) {
            continue;
        }

        strncpy(heap_entries[entry_count].timestamp, timestamp, sizeof(heap_entries[entry_count].timestamp) - 1);
        strncpy(heap_entries[entry_count].level, level, sizeof(heap_entries[entry_count].level) - 1);
        strncpy(heap_entries[entry_count].message, message, sizeof(heap_entries[entry_count].message) - 1);
        heap_entries[entry_count].timestamp[sizeof(heap_entries[entry_count].timestamp) - 1] = '\0';
        heap_entries[entry_count].level[sizeof(heap_entries[entry_count].level) - 1] = '\0';
        heap_entries[entry_count].message[sizeof(heap_entries[entry_count].message) - 1] = '\0';
        heap_entries[entry_count].severity = severity;
        entry_count++;
    }

    fclose(file);

    if (entry_count == 0) {
        printf("No valid log entries found\n");
        free(heap_entries);
        return 0;
    }

    LogEntry stack_entries[MAX_ENTRIES];
    if (entry_count > MAX_ENTRIES) {
        entry_count = MAX_ENTRIES;
    }
    
    for (int i = 0; i < entry_count; i++) {
        stack_entries[i] = heap_entries[i];
    }

    qsort