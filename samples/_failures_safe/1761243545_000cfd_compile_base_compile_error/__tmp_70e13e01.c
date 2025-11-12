//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 10000

typedef struct {
    char timestamp[32];
    char level[16];
    char message[256];
    int severity;
} LogEntry;

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

int read_log_entries(LogEntry* entries, int max_entries) {
    char line[MAX_LINE_LEN];
    int count = 0;
    
    while (fgets(line, sizeof(line), stdin) != NULL && count < max_entries) {
        if (strlen(line) >= MAX_LINE_LEN - 1) continue;
        
        char timestamp[32] = {0};
        char level[16] = {0};
        char message[256] = {0};
        
        int parsed = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
        if (parsed != 3) continue;
        
        if (!validate_timestamp(timestamp)) continue;
        
        int severity = parse_log_level(level);
        if (severity == 0) continue;
        
        strncpy(entries[count].timestamp, timestamp, sizeof(entries[count].timestamp) - 1);
        entries[count].timestamp[sizeof(entries[count].timestamp) - 1] = '\0';
        strncpy(entries[count].level, level, sizeof(entries[count].level) - 1);
        entries[count].level[sizeof(entries[count].level) - 1] = '\0';
        strncpy(entries[count].message, message, sizeof(entries[count].message) - 1);
        entries[count].message[sizeof(entries[count].message) - 1] = '\0';
        entries[count].severity = severity;
        count++;
    }
    
    return count;
}

void analyze_logs(LogEntry* entries, int count) {
    int level_counts[6] = {0};
    int total_severity = 0;
    char earliest[32] = "9999-99-99 99:99:99";
    char latest[32] = "0000-00-00 00:00:00";
    
    for (int i = 0; i < count; i++) {
        if (entries[i].severity > 0 && entries[i].severity <= 5) {
            level_counts[entries[i].severity]++;
            total_severity += entries[i].severity;
            
            if (strcmp(entries[i].timestamp, earliest) < 0) {
                strncpy(earliest, entries[i].timestamp, sizeof(earliest) - 1);
                earliest[sizeof(earliest) - 1] = '\0';
            }
            if (strcmp(entries[i].timestamp, latest) > 0) {
                strncpy(latest, entries[i].timestamp, sizeof(latest) - 1);
                latest[sizeof(latest) - 1] = '\0';
            }
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries processed: %d\n", count);
    printf("Time range: %s to %s\n", earliest, latest);
    printf("\nBreakdown by level:\n");
    printf("DEBUG: %d\n", level_counts[1]);
    printf("INFO: %d\n", level_counts[2]);
    printf("WARNING: %d\n", level_counts[3]);
    printf("ERROR: %d\n", level_counts[4]);
    printf("CRITICAL: %d\n", level_counts[5]);
    
    if (count > 0) {
        double avg_severity = (double)total_severity / count;
        printf("Average severity: %.2f\n", avg_severity);
    }
    
    int error_critical = level_counts[4] + level_counts[5];
    printf("Error+Critical entries: %d (%.1f%%)\n", 
           error_critical, count > 0 ? (error_critical * 100.0 / count) : 0.0);
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    
    printf("Enter log entries (format: YYY