//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
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

int read_log_entries(LogEntry* entries, int max_entries) {
    char buffer[MAX_LINE_LENGTH];
    int count = 0;
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message):\n");
    printf("Enter 'END' on a separate line to finish.\n");
    
    while (count < max_entries) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }
        
        buffer[strcspn(buffer, "\n")] = 0;
        
        if (strcmp(buffer, "END") == 0) {
            break;
        }
        
        if (strlen(buffer) >= MAX_LINE_LENGTH - 1) {
            printf("Line too long, skipping.\n");
            continue;
        }
        
        char timestamp[32] = {0};
        char level[16] = {0};
        char message[256] = {0};
        
        int parsed = sscanf(buffer, "%31s %15s %255[^\n]", timestamp, level, message);
        if (parsed != 3) {
            printf("Invalid format, skipping: %s\n", buffer);
            continue;
        }
        
        if (!validate_timestamp(timestamp)) {
            printf("Invalid timestamp, skipping: %s\n", timestamp);
            continue;
        }
        
        int severity = parse_log_level(level);
        if (severity == -1) {
            printf("Unknown log level, skipping: %s\n", level);
            continue;
        }
        
        if (strlen(message) == 0) {
            printf("Empty message, skipping.\n");
            continue;
        }
        
        strncpy(entries[count].timestamp, timestamp, sizeof(entries[count].timestamp) - 1);
        strncpy(entries[count].level, level, sizeof(entries[count].level) - 1);
        strncpy(entries[count].message, message, sizeof(entries[count].message) - 1);
        entries[count].timestamp[sizeof(entries[count].timestamp) - 1] = '\0';
        entries[count].level[sizeof(entries[count].level) - 1] = '\0';
        entries[count].message[sizeof(entries[count].message) - 1] = '\0';
        entries[count].severity = severity;
        count++;
    }
    
    return count;
}

void analyze_logs(LogEntry* entries, int count) {
    if (count == 0) {
        printf("No valid log entries to analyze.\n");
        return;
    }
    
    int severity_count[5] = {0};
    const char* level_names[] = {"DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL"};
    
    for (int i = 0; i < count; i++) {
        if (entries[i].severity >= 0 && entries[i].severity < 5) {
            severity_count[entries[i].severity]++;
        }
    }
    
    printf("\nLog Analysis Results:\n");
    printf("====================\n");
    printf("Total entries: %d\n", count);
    
    for (int i = 0; i < 5; i++) {
        printf("%-8s: %d entries (%.1f%%)\n", 
               level_names[i], 
               severity_count[i],
               count > 0 ? (severity_count[i] * 100.0) / count : 0.0);
    }
    
    int max_severity = 0;
    for (int i = 0; i < count; i++) {
        if (entries[i].severity > max_severity) {
            max_severity = entries[i].severity;
        }
    }
    
    printf("\nHighest severity level: %s\n", level_names[max_severity]);
    
    if (max_sever