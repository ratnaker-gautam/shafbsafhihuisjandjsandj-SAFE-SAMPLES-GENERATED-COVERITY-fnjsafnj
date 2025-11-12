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
    if (strcmp(level, "ERROR") == 0) return 3;
    if (strcmp(level, "WARN") == 0) return 2;
    if (strcmp(level, "INFO") == 0) return 1;
    if (strcmp(level, "DEBUG") == 0) return 0;
    return -1;
}

int read_log_entries(struct LogEntry* entries, int max_entries) {
    char line[MAX_LINE_LEN];
    int count = 0;
    
    while (fgets(line, sizeof(line), stdin) != NULL && count < max_entries) {
        if (strlen(line) == 0 || line[0] == '\n') continue;
        
        char timestamp[32] = {0};
        char level[16] = {0};
        char message[256] = {0};
        
        int parsed = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
        if (parsed == 3) {
            strncpy(entries[count].timestamp, timestamp, sizeof(entries[count].timestamp) - 1);
            strncpy(entries[count].level, level, sizeof(entries[count].level) - 1);
            strncpy(entries[count].message, message, sizeof(entries[count].message) - 1);
            entries[count].severity = parse_severity(level);
            if (entries[count].severity >= 0) {
                count++;
            }
        }
    }
    
    return count;
}

void analyze_by_severity(struct LogEntry* entries, int count) {
    int severity_counts[4] = {0};
    int i = 0;
    
    while (i < count) {
        if (entries[i].severity >= 0 && entries[i].severity < 4) {
            severity_counts[entries[i].severity]++;
        }
        i++;
    }
    
    printf("Severity Analysis:\n");
    printf("DEBUG: %d\n", severity_counts[0]);
    printf("INFO:  %d\n", severity_counts[1]);
    printf("WARN:  %d\n", severity_counts[2]);
    printf("ERROR: %d\n", severity_counts[3]);
}

void find_most_common_level(struct LogEntry* entries, int count) {
    int level_counts[4] = {0};
    int max_count = 0;
    char* max_level = "UNKNOWN";
    
    for (int i = 0; i < count; i++) {
        if (entries[i].severity >= 0 && entries[i].severity < 4) {
            level_counts[entries[i].severity]++;
        }
    }
    
    int j = 0;
    while (j < 4) {
        if (level_counts[j] > max_count) {
            max_count = level_counts[j];
            switch (j) {
                case 0: max_level = "DEBUG"; break;
                case 1: max_level = "INFO"; break;
                case 2: max_level = "WARN"; break;
                case 3: max_level = "ERROR"; break;
            }
        }
        j++;
    }
    
    printf("Most common level: %s (%d occurrences)\n", max_level, max_count);
}

void display_recent_entries(struct LogEntry* entries, int count) {
    printf("\nRecent entries (last 5):\n");
    int start = (count > 5) ? count - 5 : 0;
    
    int k = start;
    do {
        if (k < count) {
            printf("%s %s %s\n", entries[k].timestamp, entries[k].level, entries[k].message);
        }
        k++;
    } while (k < count && k < start + 5);
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Press Ctrl+D (Linux) to finish input\n");
    
    int count = read_log_entries(entries, MAX_ENTRIES);
    
    if (count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    printf("\nAnalysis Results:\n");
    printf("Total entries: %d\n", count);
    
    analyze_by_severity(entries, count);
    find_most_common_level(entries, count);
    display_recent_entries(entries, count);
    
    return 0;
}