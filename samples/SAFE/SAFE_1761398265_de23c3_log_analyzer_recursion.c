//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: log_analyzer
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
    
    int level = parse_log_level(logs[mid].level);
    if (level >= 0 && level < 5) {
        level_counts[level]++;
    }
    
    analyze_log_recursive(logs, start, mid - 1, level_counts);
    analyze_log_recursive(logs, mid + 1, end, level_counts);
}

int read_log_file(struct LogEntry* logs, int max_entries) {
    printf("Enter log entries (format: LEVEL MESSAGE TIMESTAMP). Enter 'END' to finish:\n");
    
    int count = 0;
    char line[MAX_LINE_LENGTH];
    
    while (count < max_entries) {
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        
        line[strcspn(line, "\n")] = 0;
        
        if (strcmp(line, "END") == 0) break;
        if (strlen(line) == 0) continue;
        
        char level[16];
        char message[MAX_LINE_LENGTH];
        int timestamp;
        
        int parsed = sscanf(line, "%15s %1023s %d", level, message, &timestamp);
        if (parsed != 3) {
            printf("Invalid format. Skipping: %s\n", line);
            continue;
        }
        
        if (parse_log_level(level) == -1) {
            printf("Invalid log level. Skipping: %s\n", level);
            continue;
        }
        
        if (timestamp < 0) {
            printf("Invalid timestamp. Skipping: %d\n", timestamp);
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
    struct LogEntry logs[1000];
    int level_counts[5] = {0};
    const char* level_names[] = {"DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL"};
    
    int log_count = read_log_file(logs, 1000);
    
    if (log_count == 0) {
        printf("No valid log entries to analyze.\n");
        return 1;
    }
    
    analyze_log_recursive(logs, 0, log_count - 1, level_counts);
    
    printf("\nLog Analysis Results:\n");
    printf("====================\n");
    for (int i = 0; i < 5; i++) {
        printf("%-8s: %d entries\n", level_names[i], level_counts[i]);
    }
    printf("Total   : %d entries\n", log_count);
    
    return 0;
}