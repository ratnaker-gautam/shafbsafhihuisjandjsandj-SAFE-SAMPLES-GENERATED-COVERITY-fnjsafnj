//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: recursion ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_LEVELS 100

struct LogEntry {
    char level[16];
    char message[MAX_LINE_LENGTH];
    int timestamp;
};

int parse_log_level(const char* level_str) {
    if (strcmp(level_str, "DEBUG") == 0) return 0;
    if (strcmp(level_str, "INFO") == 0) return 1;
    if (strcmp(level_str, "WARNING") == 0) return 2;
    if (strcmp(level_str, "ERROR") == 0) return 3;
    if (strcmp(level_str, "CRITICAL") == 0) return 4;
    return -1;
}

void analyze_log_recursive(struct LogEntry* logs, int start, int end, int* level_counts) {
    if (start > end) return;
    
    int mid = start + (end - start) / 2;
    if (mid < 0 || mid > end) return;
    
    int level = parse_log_level(logs[mid].level);
    if (level >= 0 && level < 5) {
        level_counts[level]++;
    }
    
    analyze_log_recursive(logs, start, mid - 1, level_counts);
    analyze_log_recursive(logs, mid + 1, end, level_counts);
}

int read_log_file(struct LogEntry* logs, int max_entries) {
    printf("Enter log entries (format: LEVEL MESSAGE TIMESTAMP). Enter 'END' to finish.\n");
    
    int count = 0;
    char line[MAX_LINE_LENGTH];
    
    while (count < max_entries) {
        printf("Log entry %d: ", count + 1);
        
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        
        line[strcspn(line, "\n")] = 0;
        
        if (strcmp(line, "END") == 0) break;
        
        char level[16];
        char message[MAX_LINE_LENGTH];
        int timestamp;
        
        int parsed = sscanf(line, "%15s %1023s %d", level, message, &timestamp);
        if (parsed != 3) {
            printf("Invalid format. Use: LEVEL MESSAGE TIMESTAMP\n");
            continue;
        }
        
        if (parse_log_level(level) == -1) {
            printf("Invalid log level. Use: DEBUG, INFO, WARNING, ERROR, CRITICAL\n");
            continue;
        }
        
        if (timestamp < 0) {
            printf("Timestamp must be non-negative\n");
            continue;
        }
        
        strncpy(logs[count].level, level, sizeof(logs[count].level) - 1);
        logs[count].level[sizeof(logs[count].level) - 1] = '\0';
        strncpy(logs[count].message, message, sizeof(logs[count].message) - 1);
        logs[count].message[sizeof(logs[count].message) - 1] = '\0';
        logs[count].timestamp = timestamp;
        
        count++;
    }
    
    return count;
}

int main(void) {
    struct LogEntry logs[100];
    int level_counts[5] = {0};
    
    int entry_count = read_log_file(logs, 100);
    
    if (entry_count == 0) {
        printf("No log entries to analyze.\n");
        return 1;
    }
    
    printf("\nAnalyzing %d log entries...\n", entry_count);
    
    analyze_log_recursive(logs, 0, entry_count - 1, level_counts);
    
    printf("\nLog Level Analysis:\n");
    printf("DEBUG: %d\n", level_counts[0]);
    printf("INFO: %d\n", level_counts[1]);
    printf("WARNING: %d\n", level_counts[2]);
    printf("ERROR: %d\n", level_counts[3]);
    printf("CRITICAL: %d\n", level_counts[4]);
    
    int total = level_counts[0] + level_counts[1] + level_counts[2] + level_counts[3] + level_counts[4];
    if (total > 0) {
        printf("\nDistribution:\n");
        const char* levels[] = {"DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL"};
        for (int i = 0; i < 5; i++) {
            double percentage = (double)level_counts[i] / total * 100.0;
            printf("%s: %.1f%%\n", levels[i], percentage);
        }
    }
    
    return 0;
}