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

void find_common_errors(struct LogEntry* entries, int count) {
    char error_patterns[10][64] = {0};
    int pattern_counts[10] = {0};
    int pattern_count = 0;
    
    for (int i = 0; i < count; i++) {
        if (entries[i].severity == 3) {
            char* msg = entries[i].message;
            int found = 0;
            
            for (int j = 0; j < pattern_count; j++) {
                if (strstr(msg, error_patterns[j]) != NULL) {
                    pattern_counts[j]++;
                    found = 1;
                    break;
                }
            }
            
            if (!found && pattern_count < 10) {
                const char* common_errors[] = {"timeout", "connection", "memory", "disk", "network"};
                for (int k = 0; k < 5; k++) {
                    if (strstr(msg, common_errors[k]) != NULL) {
                        strncpy(error_patterns[pattern_count], common_errors[k], 
                                sizeof(error_patterns[pattern_count]) - 1);
                        pattern_counts[pattern_count] = 1;
                        pattern_count++;
                        break;
                    }
                }
            }
        }
    }
    
    if (pattern_count > 0) {
        printf("\nCommon Error Patterns:\n");
        for (int i = 0; i < pattern_count; i++) {
            printf("%s: %d occurrences\n", error_patterns[i], pattern_counts[i]);
        }
    }
}

void show_recent_entries(struct LogEntry* entries, int count) {
    int show_count = (count > 5) ? 5 : count;
    printf("\nMost Recent %d Entries:\n", show_count);
    
    int start = (count > show_count) ? count - show_count : 0;
    for (int i = start; i < count; i++) {
        printf("%s %s %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
    }
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Press Ctrl+D to finish input\n");
    
    int count = read_log_entries(entries, MAX_ENTRIES);
    
    if (count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    printf("\nAnalyzed %d log entries:\n", count);
    
    analyze_by_severity(entries, count);
    find_common_errors(entries, count);
    show_recent_entries(entries, count);
    
    return 0;
}