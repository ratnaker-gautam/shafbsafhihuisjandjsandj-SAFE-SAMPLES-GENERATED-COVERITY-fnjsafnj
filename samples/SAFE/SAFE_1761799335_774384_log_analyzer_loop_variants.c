//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

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

int read_log_file(struct LogEntry entries[], int max_entries) {
    char line[MAX_LINE_LEN];
    int count = 0;
    
    while (count < max_entries && fgets(line, sizeof(line), stdin) != NULL) {
        char timestamp[32] = {0};
        char level[16] = {0};
        char message[256] = {0};
        
        if (sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message) == 3) {
            int sev = parse_severity(level);
            if (sev >= 0) {
                strncpy(entries[count].timestamp, timestamp, sizeof(entries[count].timestamp) - 1);
                strncpy(entries[count].level, level, sizeof(entries[count].level) - 1);
                strncpy(entries[count].message, message, sizeof(entries[count].message) - 1);
                entries[count].severity = sev;
                count++;
            }
        }
    }
    
    return count;
}

void analyze_logs(struct LogEntry entries[], int count) {
    int severity_count[5] = {0};
    int i = 0;
    
    do {
        if (entries[i].severity >= 0 && entries[i].severity < 5) {
            severity_count[entries[i].severity]++;
        }
        i++;
    } while (i < count);
    
    printf("Log Analysis Summary:\n");
    printf("=====================\n");
    
    const char* levels[] = {"DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL"};
    for (int j = 0; j < 5; j++) {
        printf("%-8s: %d entries\n", levels[j], severity_count[j]);
    }
    
    int error_critical = 0;
    int idx = 0;
    while (idx < count) {
        if (entries[idx].severity >= 3) {
            error_critical++;
        }
        idx++;
    }
    printf("ERROR+CRITICAL: %d entries\n", error_critical);
    
    printf("\nRecent high-severity entries:\n");
    int printed = 0;
    for (int k = count - 1; k >= 0 && printed < 3; k--) {
        if (entries[k].severity >= 2) {
            printf("- [%s] %s: %s\n", entries[k].timestamp, entries[k].level, entries[k].message);
            printed++;
        }
    }
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    
    printf("Enter log entries (timestamp level message):\n");
    printf("Example: 2024-01-15T10:30:00 INFO User logged in\n");
    printf("Press Ctrl+D (Unix) or Ctrl+Z (Windows) to finish input\n\n");
    
    int count = read_log_file(entries, MAX_ENTRIES);
    
    if (count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    if (count >= MAX_ENTRIES) {
        printf("Warning: Maximum entry limit reached (%d entries)\n", MAX_ENTRIES);
    }
    
    analyze_logs(entries, count);
    
    return 0;
}