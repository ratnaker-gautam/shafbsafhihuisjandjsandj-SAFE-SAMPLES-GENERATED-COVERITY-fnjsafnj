//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: log_analyzer
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

int parse_severity(const char* level) {
    if (strcmp(level, "DEBUG") == 0) return 0;
    if (strcmp(level, "INFO") == 0) return 1;
    if (strcmp(level, "WARNING") == 0) return 2;
    if (strcmp(level, "ERROR") == 0) return 3;
    if (strcmp(level, "CRITICAL") == 0) return 4;
    return -1;
}

int read_log_entries(struct LogEntry* entries, int max_entries) {
    char line[MAX_LINE_LEN];
    int count = 0;
    
    while (count < max_entries && fgets(line, sizeof(line), stdin) != NULL) {
        char timestamp[32] = {0};
        char level[16] = {0};
        char message[256] = {0};
        
        if (sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message) == 3) {
            int severity = parse_severity(level);
            if (severity >= 0) {
                strncpy(entries[count].timestamp, timestamp, sizeof(entries[count].timestamp) - 1);
                strncpy(entries[count].level, level, sizeof(entries[count].level) - 1);
                strncpy(entries[count].message, message, sizeof(entries[count].message) - 1);
                entries[count].severity = severity;
                count++;
            }
        }
    }
    
    return count;
}

void analyze_logs(struct LogEntry* entries, int count) {
    int severity_count[5] = {0};
    int i = 0;
    
    do {
        if (entries[i].severity >= 0 && entries[i].severity < 5) {
            severity_count[entries[i].severity]++;
        }
        i++;
    } while (i < count);
    
    printf("Log Analysis Summary:\n");
    printf("====================\n");
    
    const char* levels[] = {"DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL"};
    int j;
    for (j = 0; j < 5; j++) {
        printf("%-8s: %d entries\n", levels[j], severity_count[j]);
    }
    
    int total_errors = severity_count[3] + severity_count[4];
    printf("Total errors: %d\n", total_errors);
    
    if (count > 0) {
        int k = 0;
        printf("\nRecent critical entries:\n");
        while (k < count) {
            if (entries[k].severity == 4) {
                printf("- %s: %s\n", entries[k].timestamp, entries[k].message);
            }
            k++;
        }
    }
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE)\n");
    printf("Supported levels: DEBUG, INFO, WARNING, ERROR, CRITICAL\n");
    printf("Press Ctrl+D (Unix) or Ctrl+Z (Windows) to finish input\n");
    
    int count = read_log_entries(entries, MAX_ENTRIES);
    
    if (count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    if (count == MAX_ENTRIES) {
        printf("Warning: Reached maximum entry limit (%d)\n", MAX_ENTRIES);
    }
    
    analyze_logs(entries, count);
    
    return 0;
}