//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

    int entry_count = 0;
    char buffer[MAX_LINE_LENGTH];
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL MESSAGE):\n");
    printf("Enter 'END' on a separate line to finish input\n");

    while (entry_count < MAX_ENTRIES) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }
        
        buffer[strcspn(buffer, "\n")] = 0;
        
        if (strcmp(buffer, "END") == 0) {
            break;
        }
        
        if (strlen(buffer) == 0) {
            continue;
        }

        char timestamp[32];
        char level[16];
        char message[256];
        
        int parsed = sscanf(buffer, "%31s %15s %255[^\n]", timestamp, level, message);
        if (parsed != 3) {
            fprintf(stderr, "Invalid log format. Skipping: %s\n", buffer);
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

        if (entry_count < MAX_ENTRIES) {
            strncpy(heap_entries[entry_count].timestamp, timestamp, sizeof(heap_entries[entry_count].timestamp) - 1);
            strncpy(heap_entries[entry_count].level, level, sizeof(heap_entries[entry_count].level) - 1);
            strncpy(heap_entries[entry_count].message, message, sizeof(heap_entries[entry_count].message) - 1);
            heap_entries[entry_count].severity = severity;
            heap_entries[entry_count].timestamp[31] = '\0';
            heap_entries[entry_count].level[15] = '\0';
            heap_entries[entry_count].message[255] = '\0';
            entry_count++;
        }
    }

    if (entry_count == 0) {
        printf("No valid log entries to analyze.\n");
        free(heap_entries);
        return 0;
    }

    qsort(heap_entries, entry_count, sizeof(LogEntry), compare_entries);

    int level_counts[5] = {0};
    
    for (int i = 0; i < entry_count; i++) {
        if (heap_entries[i].severity >= 0 && heap_entries[i].severity < 5) {
            level_counts[heap_entries[i].severity]++;
        }
    }

    printf("\nLog Analysis Summary (sorted by severity):\n");
    printf("==========================================\n");
    
    for (int i = 0; i < entry_count; i++) {
        printf("%s %-8s %s\n", heap_entries[i].timestamp, heap_entries[i].level, heap_entries[i].message);
    }

    printf("\nSeverity Breakdown:\n");
    printf("CRITICAL: %d\n", level_counts[4]);
    printf("ERROR:    %d\n", level_counts[3]);
    printf("WARNING:  %d\n", level_counts[2]);
    printf("INFO:     %d\n