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

void analyze_log_recursive(struct LogEntry* logs, int start, int end, int target_level, int* count) {
    if (start > end || start < 0 || end < 0) {
        return;
    }
    
    if (start == end) {
        int level = parse_log_level(logs[start].level);
        if (level >= 0 && level == target_level) {
            (*count)++;
        }
        return;
    }
    
    int mid = start + (end - start) / 2;
    if (mid < start || mid > end) {
        return;
    }
    
    analyze_log_recursive(logs, start, mid, target_level, count);
    analyze_log_recursive(logs, mid + 1, end, target_level, count);
}

int read_log_file(struct LogEntry* logs, int max_entries) {
    printf("Enter log entries (format: LEVEL MESSAGE TIMESTAMP). Enter 'END' to finish.\n");
    
    int count = 0;
    char line[MAX_LINE_LENGTH];
    
    while (count < max_entries) {
        printf("Log entry %d: ", count + 1);
        
        if (fgets(line, sizeof(line), stdin) == NULL) {
            break;
        }
        
        line[strcspn(line, "\n")] = 0;
        
        if (strcmp(line, "END") == 0) {
            break;
        }
        
        if (strlen(line) == 0) {
            continue;
        }
        
        char level[16];
        char message[MAX_LINE_LENGTH];
        int timestamp;
        
        int parsed = sscanf(line, "%15s %1023s %d", level, message, &timestamp);
        if (parsed != 3) {
            printf("Invalid format. Use: LEVEL MESSAGE TIMESTAMP\n");
            continue;
        }
        
        if (parse_log_level(level) < 0) {
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
    int log_count = read_log_file(logs, 100);
    
    if (log_count == 0) {
        printf("No log entries to analyze.\n");
        return 0;
    }
    
    printf("\nAvailable log levels: DEBUG, INFO, WARNING, ERROR, CRITICAL\n");
    printf("Enter level to analyze: ");
    
    char target_level_str[16];
    if (fgets(target_level_str, sizeof(target_level_str), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    target_level_str[strcspn(target_level_str, "\n")] = 0;
    
    int target_level = parse_log_level(target_level_str);
    if (target_level < 0) {
        printf("Invalid log level.\n");
        return 1;
    }
    
    int count = 0;
    analyze_log_recursive(logs, 0, log_count - 1, target_level, &count);
    
    printf("\nFound %d entries with level %s\n", count, target_level_str);
    
    return 0;
}