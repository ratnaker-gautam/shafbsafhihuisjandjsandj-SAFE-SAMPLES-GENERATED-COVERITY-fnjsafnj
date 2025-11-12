//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[32];
    char level[16];
    char message[256];
    int severity;
};

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

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char buffer[MAX_LINE_LEN];
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL Message):\n");
    printf("Enter 'END' on a new line to finish input.\n");
    
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
            printf("Invalid log format. Skipping: %s\n", buffer);
            continue;
        }
        
        if (!validate_timestamp(timestamp)) {
            printf("Invalid timestamp format. Skipping: %s\n", timestamp);
            continue;
        }
        
        int severity = parse_log_level(level);
        if (severity == 0) {
            printf("Unknown log level. Skipping: %s\n", level);
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
    
    if (entry_count == 0) {
        printf("No valid log entries to analyze.\n");
        return 0;
    }
    
    int* severity_counts = malloc(6 * sizeof(int));
    if (severity_counts == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }
    
    for (int i = 0; i < 6; i++) {
        severity_counts[i] = 0;
    }
    
    for (int i = 0; i < entry_count; i++) {
        if (entries[i].severity >= 1 && entries[i].severity <= 5) {
            severity_counts[entries[i].severity]++;
        }
    }
    
    printf("\nLog Analysis Results:\n");
    printf("Total entries: %d\n", entry_count);
    printf("DEBUG: %d\n", severity_counts[1]);
    printf("INFO: %d\n", severity_counts[2]);
    printf("WARNING: %d\n", severity_counts[3]);
    printf("ERROR: %d\n", severity_counts[4]);
    printf("CRITICAL: %d\n", severity_counts[5]);
    
    int max_severity = 0;
    for (int i = 0; i < entry_count; i++) {
        if (entries[i].severity > max_severity) {
            max_severity = entries[i].severity;
        }
    }
    
    if (max_severity >= 4) {
        printf("\nHigh severity entries found:\n");
        for (int i = 0; i < entry_count; i++) {
            if (entries[i].severity >= 4) {
                printf("%s %s: %s\n", entries[i].timestamp, entries[i].level, entries[i].message);