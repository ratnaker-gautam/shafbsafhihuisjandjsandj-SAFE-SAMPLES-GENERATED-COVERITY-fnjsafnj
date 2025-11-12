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
        
        if (!is_valid_timestamp(timestamp)) continue;
        
        int severity = parse_log_level(level);
        if (severity == 0) continue;
        
        strncpy(entries[count].timestamp, timestamp, sizeof(entries[count].timestamp) - 1);
        strncpy(entries[count].level, level, sizeof(entries[count].level) - 1);
        strncpy(entries[count].message, message, sizeof(entries[count].message) - 1);
        entries[count].severity = severity;
        count++;
    }
    
    return count;
}

void analyze_logs(LogEntry* entries, int count) {
    int level_counts[6] = {0};
    int total_severity = 0;
    char most_common_level[16] = "UNKNOWN";
    int max_count = 0;
    
    for (int i = 0; i < count; i++) {
        int sev = entries[i].severity;
        if (sev >= 1 && sev <= 5) {
            level_counts[sev]++;
            total_severity += sev;
        }
    }
    
    const char* levels[] = {"", "DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL"};
    for (int i = 1; i <= 5; i++) {
        if (level_counts[i] > max_count) {
            max_count = level_counts[i];
            strncpy(most_common_level, levels[i], sizeof(most_common_level) - 1);
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries processed: %d\n", count);
    printf("DEBUG: %d\n", level_counts[1]);
    printf("INFO: %d\n", level_counts[2]);
    printf("WARNING: %d\n", level_counts[3]);
    printf("ERROR: %d\n", level_counts[4]);
    printf("CRITICAL: %d\n", level_counts[5]);
    printf("Most common level: %s\n", most_common_level);
    
    if (count > 0) {
        double avg_severity = (double)total_severity / count;
        printf("Average severity: %.2f\n", avg_severity);
    }
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL MESSAGE):\n");
    printf("Press Ctrl+D (Linux) or Ctrl+Z (Windows) to end input.\n");
    
    int count = read_log_entries(entries, MAX_ENTRIES);
    
    if (count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    analyze_logs(entries, count);
    
    return 0;
}