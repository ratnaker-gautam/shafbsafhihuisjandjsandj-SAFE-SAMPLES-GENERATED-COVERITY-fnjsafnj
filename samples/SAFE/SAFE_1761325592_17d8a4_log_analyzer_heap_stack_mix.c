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
        } else if (i == 13 || i == 16) {
            if (timestamp[i] != ':') return 0;
        } else {
            if (!isdigit(timestamp[i])) return 0;
        }
    }
    return 1;
}

int main(void) {
    struct LogEntry* entries = malloc(MAX_ENTRIES * sizeof(struct LogEntry));
    if (entries == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    int entry_count = 0;
    char buffer[MAX_LINE_LENGTH];
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message):\n");
    printf("Enter 'END' on a separate line to finish input.\n");

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

        char timestamp[32], level[16], message[256];
        int parsed = sscanf(buffer, "%31s %15s %255[^\n]", timestamp, level, message);
        
        if (parsed != 3) {
            fprintf(stderr, "Invalid log format. Skipping: %s\n", buffer);
            continue;
        }

        if (!validate_timestamp(timestamp)) {
            fprintf(stderr, "Invalid timestamp format. Skipping: %s\n", timestamp);
            continue;
        }

        int severity = parse_severity(level);
        if (severity == -1) {
            fprintf(stderr, "Unknown log level. Skipping: %s\n", level);
            continue;
        }

        if (entry_count < MAX_ENTRIES) {
            strncpy(entries[entry_count].timestamp, timestamp, sizeof(entries[entry_count].timestamp) - 1);
            strncpy(entries[entry_count].level, level, sizeof(entries[entry_count].level) - 1);
            strncpy(entries[entry_count].message, message, sizeof(entries[entry_count].message) - 1);
            entries[entry_count].severity = severity;
            entries[entry_count].timestamp[31] = '\0';
            entries[entry_count].level[15] = '\0';
            entries[entry_count].message[255] = '\0';
            entry_count++;
        }
    }

    int severity_counts[5] = {0};
    for (int i = 0; i < entry_count; i++) {
        if (entries[i].severity >= 0 && entries[i].severity < 5) {
            severity_counts[entries[i].severity]++;
        }
    }

    printf("\nLog Analysis Summary:\n");
    printf("Total entries: %d\n", entry_count);
    const char* level_names[] = {"DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL"};
    for (int i = 0; i < 5; i++) {
        printf("%s: %d\n", level_names[i], severity_counts[i]);
    }

    printf("\nEntries by severity (highest first):\n");
    for (int sev = 4; sev >= 0; sev--) {
        for (int i = 0; i < entry_count; i++) {
            if (entries[i].severity == sev) {
                printf("[%s] %s: %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
            }
        }
    }

    free(entries);
    return 0;
}