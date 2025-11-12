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
    if (strcmp(level, "DEBUG") == 0) return 1;
    if (strcmp(level, "INFO") == 0) return 2;
    if (strcmp(level, "WARNING") == 0) return 3;
    if (strcmp(level, "ERROR") == 0) return 4;
    if (strcmp(level, "CRITICAL") == 0) return 5;
    return 0;
}

int read_log_file(struct LogEntry entries[], int max_entries) {
    char line[MAX_LINE_LEN];
    int count = 0;
    
    while (count < max_entries && fgets(line, sizeof(line), stdin) != NULL) {
        char timestamp[32] = {0};
        char level[16] = {0};
        char message[256] = {0};
        
        int parsed = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
        if (parsed >= 2) {
            if (strlen(timestamp) > 0 && strlen(level) > 0) {
                strncpy(entries[count].timestamp, timestamp, sizeof(entries[count].timestamp) - 1);
                strncpy(entries[count].level, level, sizeof(entries[count].level) - 1);
                if (parsed == 3) {
                    strncpy(entries[count].message, message, sizeof(entries[count].message) - 1);
                }
                entries[count].severity = parse_severity(level);
                count++;
            }
        }
    }
    
    return count;
}

void analyze_logs(struct LogEntry entries[], int count) {
    int severity_count[6] = {0};
    int i = 0;
    
    do {
        if (entries[i].severity >= 0 && entries[i].severity <= 5) {
            severity_count[entries[i].severity]++;
        }
        i++;
    } while (i < count);
    
    printf("Log Analysis Summary:\n");
    printf("=====================\n");
    printf("Total entries: %d\n", count);
    
    const char* level_names[] = {"UNKNOWN", "DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL"};
    int j;
    for (j = 0; j < 6; j++) {
        printf("%-8s: %d entries\n", level_names[j], severity_count[j]);
    }
    
    printf("\nRecent entries:\n");
    int start = (count > 5) ? count - 5 : 0;
    int k;
    for (k = start; k < count; k++) {
        printf("%s %s %s\n", entries[k].timestamp, entries[k].level, entries[k].message);
    }
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    
    printf("Enter log entries (format: TIMESTAMP LEVEL [MESSAGE]):\n");
    printf("Press Ctrl+D (Linux) or Ctrl+Z (Windows) to finish input\n");
    
    int count = read_log_file(entries, MAX_ENTRIES);
    
    if (count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    if (count >= MAX_ENTRIES) {
        printf("Warning: Reached maximum entry limit (%d)\n", MAX_ENTRIES);
    }
    
    analyze_logs(entries, count);
    
    return 0;
}