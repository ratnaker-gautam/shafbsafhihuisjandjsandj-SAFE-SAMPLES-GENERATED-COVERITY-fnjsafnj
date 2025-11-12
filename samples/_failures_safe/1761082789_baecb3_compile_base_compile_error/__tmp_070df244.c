//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[32];
    char level[16];
    char message[256];
    int severity;
};

int parse_severity(const char* level) {
    if (strcmp(level, "DEBUG") == 0) return 1;
    if (strcmp(level, "INFO") == 0) return 2;
    if (strcmp(level, "WARNING") == 0) return 3;
    if (strcmp(level, "ERROR") == 0) return 4;
    if (strcmp(level, "CRITICAL") == 0) return 5;
    return 0;
}

int is_valid_timestamp(const char* ts) {
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
    struct LogEntry* entries = malloc(MAX_ENTRIES * sizeof(struct LogEntry));
    if (entries == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    int entry_count = 0;
    char buffer[MAX_LINE_LEN];
    
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
        
        char timestamp[32] = {0};
        char level[16] = {0};
        char message[256] = {0};
        
        int parsed = sscanf(buffer, "%31s %15s %255[^\n]", timestamp, level, message);
        if (parsed < 3) {
            fprintf(stderr, "Invalid log format: %s\n", buffer);
            continue;
        }
        
        if (!is_valid_timestamp(timestamp)) {
            fprintf(stderr, "Invalid timestamp: %s\n", timestamp);
            continue;
        }
        
        int severity = parse_severity(level);
        if (severity == 0) {
            fprintf(stderr, "Invalid log level: %s\n", level);
            continue;
        }
        
        if (entry_count < MAX_ENTRIES) {
            strncpy(entries[entry_count].timestamp, timestamp, sizeof(entries[entry_count].timestamp) - 1);
            entries[entry_count].timestamp[sizeof(entries[entry_count].timestamp) - 1] = '\0';
            strncpy(entries[entry_count].level, level, sizeof(entries[entry_count].level) - 1);
            entries[entry_count].level[sizeof(entries[entry_count].level) - 1] = '\0';
            strncpy(entries[entry_count].message, message, sizeof(entries[entry_count].message) - 1);
            entries[entry_count].message[sizeof(entries[entry_count].message) - 1] = '\0';
            entries[entry_count].severity = severity;
            entry_count++;
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries to analyze.\n");
        free(entries);
        return 0;
    }
    
    int severity_counts[6] = {0};
    char earliest[32] = "9999-99-99 99:99:99";
    char latest[32] = "0000-00-00 00:00:00";
    
    for (int i = 0; i < entry_count; i++) {
        if (entries[i].severity >= 1 && entries[i].severity <= 5) {
            severity_counts[entries[i].severity]++;
        }
        
        if (strcmp(entries[i].timestamp, earliest) < 0) {
            strncpy(earliest, entries[i].timestamp, sizeof(earliest) - 1);
            earliest[sizeof(earliest) - 1] = '\0';
        }
        
        if (strcmp(entries[i].timestamp, latest) > 0) {
            strncpy(latest, entries[i].timestamp, sizeof(latest) - 1);
            latest[sizeof(latest) - 1] = '\0';
        }
    }
    
    printf("\nLog Analysis Results:\n");
    printf("Total entries: %d\n", entry_count);
    printf("Time range: %s to %s\n", earliest, latest);
    printf("\nSeverity breakdown:\n");
    printf