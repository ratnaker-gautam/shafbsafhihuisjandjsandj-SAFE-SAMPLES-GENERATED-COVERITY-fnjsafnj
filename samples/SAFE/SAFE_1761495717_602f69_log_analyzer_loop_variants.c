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
    
    while (fgets(line, sizeof(line), stdin) != NULL && count < max_entries) {
        char timestamp[32] = {0};
        char level[16] = {0};
        char message[256] = {0};
        
        int parsed = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
        if (parsed == 3) {
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

void analyze_by_severity(struct LogEntry* entries, int count) {
    int severity_counts[5] = {0};
    int i = 0;
    
    do {
        if (entries[i].severity >= 0 && entries[i].severity < 5) {
            severity_counts[entries[i].severity]++;
        }
        i++;
    } while (i < count);
    
    printf("Severity Analysis:\n");
    const char* levels[] = {"DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL"};
    for (int j = 0; j < 5; j++) {
        printf("  %s: %d\n", levels[j], severity_counts[j]);
    }
}

void find_most_common_level(struct LogEntry* entries, int count) {
    int level_counts[5] = {0};
    
    for (int i = 0; i < count; i++) {
        if (entries[i].severity >= 0 && entries[i].severity < 5) {
            level_counts[entries[i].severity]++;
        }
    }
    
    int max_count = 0;
    int max_level = -1;
    int k = 0;
    while (k < 5) {
        if (level_counts[k] > max_count) {
            max_count = level_counts[k];
            max_level = k;
        }
        k++;
    }
    
    if (max_level >= 0) {
        const char* levels[] = {"DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL"};
        printf("Most common level: %s (%d occurrences)\n", levels[max_level], max_count);
    }
}

void show_recent_entries(struct LogEntry* entries, int count) {
    printf("Recent entries (last 5):\n");
    int start = (count > 5) ? count - 5 : 0;
    
    int m = start;
    while (m < count) {
        printf("  [%s] %s: %s\n", entries[m].timestamp, entries[m].level, entries[m].message);
        m++;
    }
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Example: 2024-01-15T10:30:00 INFO User logged in\n");
    printf("Press Ctrl+D (Unix) or Ctrl+Z (Windows) to finish input\n");
    
    int count = read_log_entries(entries, MAX_ENTRIES);
    
    if (count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    printf("\nAnalysis Results:\n");
    printf("Total entries: %d\n", count);
    
    analyze_by_severity(entries, count);
    printf("\n");
    find_most_common_level(entries, count);
    printf("\n");
    show_recent_entries(entries, count);
    
    return 0;
}