//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
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
    if (strcmp(level, "DEBUG") == 0) return 0;
    if (strcmp(level, "INFO") == 0) return 1;
    if (strcmp(level, "WARNING") == 0) return 2;
    if (strcmp(level, "ERROR") == 0) return 3;
    if (strcmp(level, "CRITICAL") == 0) return 4;
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
        
        char timestamp[32], level[16], message[256];
        int parsed = sscanf(buffer, "%31s %15s %255[^\n]", timestamp, level, message);
        
        if (parsed != 3) {
            fprintf(stderr, "Invalid log format: %s\n", buffer);
            continue;
        }
        
        if (!validate_timestamp(timestamp)) {
            fprintf(stderr, "Invalid timestamp: %s\n", timestamp);
            continue;
        }
        
        int severity = parse_severity(level);
        if (severity == -1) {
            fprintf(stderr, "Invalid log level: %s\n", level);
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
    
    printf("\nLog Analysis Results:\n");
    printf("====================\n");
    printf("Total entries: %d\n", entry_count);
    
    if (entry_count > 0) {
        int severity_count[5] = {0};
        char* severity_names[] = {"DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL"};
        
        for (int i = 0; i < entry_count; i++) {
            if (entries[i].severity >= 0 && entries[i].severity < 5) {
                severity_count[entries[i].severity]++;
            }
        }
        
        printf("\nSeverity Distribution:\n");
        for (int i = 0; i < 5; i++) {
            printf("%-8s: %d (%.1f%%)\n", 
                   severity_names[i], 
                   severity_count[i],
                   entry_count > 0 ? (100.0 * severity_count[i] / entry_count) : 0.0);
        }
        
        printf("\nRecent entries (last 5):\n");
        int start = entry_count > 5 ? entry_count - 5 : 0;
        for (int i = start; i < entry_count; i++) {
            printf("[%s] %-8s: %s\n", 
                   entries[i].timestamp,
                   entries[i].level,
                   entries[i].message);
        }
    }
    
    free(entries);
    return 0;
}