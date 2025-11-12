//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
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

int read_log_entries(LogEntry* entries, int max_entries) {
    int count = 0;
    char line[MAX_LINE_LEN];
    
    while (count < max_entries && fgets(line, sizeof(line), stdin) != NULL) {
        if (line[0] == '\n' || line[0] == '\0') continue;
        
        char timestamp[32] = {0};
        char level[16] = {0};
        char message[256] = {0};
        
        int parsed = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
        if (parsed != 3) continue;
        
        if (!validate_timestamp(timestamp)) continue;
        
        int severity = parse_log_level(level);
        if (severity == -1) continue;
        
        if (strlen(message) == 0) continue;
        
        strncpy(entries[count].timestamp, timestamp, sizeof(entries[count].timestamp) - 1);
        strncpy(entries[count].level, level, sizeof(entries[count].level) - 1);
        strncpy(entries[count].message, message, sizeof(entries[count].message) - 1);
        entries[count].severity = severity;
        entries[count].timestamp[31] = '\0';
        entries[count].level[15] = '\0';
        entries[count].message[255] = '\0';
        
        count++;
    }
    
    return count;
}

void analyze_logs(LogEntry* entries, int count) {
    if (count == 0) {
        printf("No valid log entries found.\n");
        return;
    }
    
    int level_counts[5] = {0};
    char* level_names[] = {"DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL"};
    
    for (int i = 0; i < count; i++) {
        if (entries[i].severity >= 0 && entries[i].severity < 5) {
            level_counts[entries[i].severity]++;
        }
    }
    
    printf("Log Analysis Summary:\n");
    printf("Total entries: %d\n", count);
    printf("Breakdown by level:\n");
    for (int i = 0; i < 5; i++) {
        printf("  %s: %d\n", level_names[i], level_counts[i]);
    }
    
    int error_critical_count = level_counts[3] + level_counts[4];
    if (error_critical_count > 0) {
        printf("\nHigh severity entries (ERROR/CRITICAL):\n");
        for (int i = 0; i < count; i++) {
            if (entries[i].severity >= 3) {
                printf("  [%s] %s: %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
            }
        }
    }
}

int main(void) {
    LogEntry* entries = malloc(MAX_ENTRIES * sizeof(LogEntry));
    if (entries == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL MESSAGE)\n");
    printf("Supported levels: DEBUG, INFO, WARNING, ERROR, CRITICAL\n");
    printf("Press Ctrl+D (Unix) or Ctrl+Z (Windows) to finish input\n\n");
    
    int count = read_log_entries(entries, MAX_ENTRIES);
    
    if (count > 0) {
        analyze_logs(entries, count);
    } else {
        printf("No valid log entries to analyze.\n");
    }
    
    free(entries);
    return 0;
}